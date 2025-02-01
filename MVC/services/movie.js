const Movie = require('../models/movie');
const Category = require('../models/category');
const mongoose = require('mongoose');
const net = require('net');
const CPP_SERVER_HOST = process.env.NODE_ENV === 'docker' ? 'cpp-server' : '127.0.0.1';

// Get movies by categories
const getMoviesByCategories = async (userId) => {
    try {
        // Fetch promoted categories
        const promotedCategories = await Category.find({ promoted: true });
        console.log(promotedCategories);

        // Fetch movies for promoted categories
        const moviesByCategory = await Promise.all(
            promotedCategories.map(async (category) => {
                // Fetch up to 20 random movies the user hasn't watched in this category
                const movies = await Movie.aggregate([
                    { $match: { category: category.name, watchedBy: { $ne: userId } } }, // Exclude movies watched by the user
                    { $sample: { size: 20 } } // Get up to 20 random movies
                ]);
                return { category: category.name, movies };
            })
        );

        // Fetch up to 20 random movies the user has watched
        const watchedMovies = await Movie.aggregate([
            { $match: { watchedBy: userId } }, // Filter movies watched by the user
            { $sample: { size: 20 } } // Get up to 20 random movies
        ]);

        // Return the organized movies grouped by categories
        return {
            promotedCategories: moviesByCategory,
            watchedMovies: { category: 'Recently Watched', movies: watchedMovies }
        };
    } catch (error) {
        console.error('Error fetching movies by categories:', error);
        throw new Error('Failed to fetch movies by categories');
    }
};

// Create a new movie
const createMovie = async (movieData) => {
  try {
    const newMovie = new Movie(movieData);
    return await newMovie.save();
  } catch (error) {
    console.error('Error creating movie:', error.message);
    throw new Error('Failed to create movie.');
  }
};

module.exports = { createMovie };



// Get a movie by ID
const getMovieById = async (id) => {
    try {
        return await Movie.findById(id);
    } catch (error) {
        console.error('Error fetching movie by ID:', error);
        throw new Error('Failed to fetch movie by ID');
    }
};


const replaceMovieById = async (id, movieData) => {
    try {
         // Delete the existing movie
        await deleteMovieById(id);

       // Create a new movie
        return await createMovie(movieData);
    } catch (error) {
        console.error('Error replacing movie:', error.message);
        throw new Error('Failed to replace movie');
    }
};

// Delete a movie by ID
const deleteMovieById = async (id) => {
    try {
        // Fetch the movie to retrieve the `watched_by` list
        const movie = await Movie.findById(id);
        if (!movie) {
            throw new Error('Movie not found');
        }

        // Connect to the remote server using a socket
        const sendDeleteRequest = async (userId) => {
            return new Promise((resolve, reject) => {
                const client = new net.Socket();

                client.connect(9000, CPP_SERVER_HOST , () => {
                    // Send the DELETE message
                    const message = `DELETE ${userId} ${id}`;
                    client.write(message);
                });

                client.on('data', (data) => {
                    console.log(`Response from server: ${data}`);
                    client.destroy(); // Close the connection after the response
                    resolve();
                });

                client.on('error', (err) => {
                    console.error(`Error communicating with server: ${err.message}`);
                    reject(err);
                });

                client.on('close', () => {
                    console.log('Connection to server closed');
                });
            });
        };

        // Iterate over the `watchedBy` list and send the DELETE message for each user
        for (const userId of movie.watchedBy) {
            await sendDeleteRequest(userId);
        }

        // Delete the movie after sending all DELETE requests
        const res = await Movie.findByIdAndDelete(id);

        return ("204 No Content"); // 204 No Content (successful, but no body)    

        } catch (error) {
        console.error('Error deleting movie:', error);
        throw new Error('Failed to delete movie');
    }
};

// Function to search movies by query
const searchMovies = async (query) => {
    try {
        if (!query || typeof query !== 'string' || query.trim() === '') {
            throw new Error('Invalid query parameter');
        }

        // Create search criteria to support searching by title, category, description, cast, and year
        const searchCriteria = {
            $or: [
                { title: { $regex: query, $options: 'i' } },  // Search by title
                { category: { $regex: query, $options: 'i' } },  // Search by category
                { description: { $regex: query, $options: 'i' } },  // Search by description
                { cast: { $regex: query, $options: 'i' } }  // Search by cast (actor's name)
            ]
        };

        // Check if the query is a number (e.g., year)
        if (!isNaN(query)) {
            searchCriteria.$or.push({ year: Number(query) }); // Add year to the search criteria if query is a number
        }

        // Perform the search using the criteria
        return await Movie.find(searchCriteria); // Return movies matching the search criteria
    } catch (error) {
        console.error('Error during search:', error.message);
        throw new Error('Failed to search movies');
    }
};

// Recommendation handler
const getRecommendation = async (id_name, id) => {
    return new Promise((resolve, reject) => {
        const client = net.createConnection({ port: 9000, host: CPP_SERVER_HOST  }, () => {
            client.write(`GET ${id_name} ${id}`);
        });

        client.on('data', (data) => {
            resolve(data.toString().trim());
            client.end();
        });

        client.on('error', (err) => {
            console.error('Error in recommendation connection:', err);
            reject(new Error('Failed to get recommendation'));
        });

        client.on('close', () => {
            console.log('Recommendation connection closed');
        });
    });
};

// Add movie to a service

const addMovieToService = async (id_name, id) => {
    const sendRequest = (method) => {
        return new Promise((resolve, reject) => {
            const command = `${method} ${id_name} ${id}`;
            const client = net.createConnection({ port: 9000, host: CPP_SERVER_HOST  }, () => {
                client.write(command);
            });

            client.on('data', (data) => {
                const response = data.toString().trim();
                client.end(); // Ensure connection is closed
                resolve(response);
            });

            client.on('error', (err) => {
                client.end(); // Ensure connection is closed on error
                reject(new Error(`Failed to ${method.toLowerCase()} movie: ${err.message}`));
            });

            client.on('close', () => {
                console.log('Connection closed');
            });

            // Add a timeout to prevent hanging indefinitely
            client.setTimeout(5000, () => {
                client.end(); // Ensure connection is closed on timeout
                reject(new Error(`Request timed out for ${method.toLowerCase()} movie`));
            });
        });
    };

    try {
        // Attempt to send a POST request
        const response = await sendRequest('POST');
        console.log('Connection closed');

        // Check if the response indicates a 404 and retry with PATCH
        if (response.includes('404')) {
            console.log('POST failed with 404, trying PATCH...');
            return await sendRequest('PATCH');
        }

        // Return the successful response
        return response;
    } catch (error) {
        // Log the error and propagate it
        console.error('Error communicating with service:', error.message);
        throw error;
    }
};

module.exports = {
    getMoviesByCategories,
    createMovie,
    getMovieById,
    replaceMovieById,
    deleteMovieById,
    searchMovies,
    getRecommendation,
    addMovieToService
};
