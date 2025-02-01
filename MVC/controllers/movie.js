const mongoose = require('mongoose');
const movieService = require('../services/movie');
const Movie = require('../models/movie'); // Use consistent capitalization

// Get movies by categories
const getMovies = async (req, res) => {
    try {
        const userId = req.header("userId") || 'guest';
        const movies = await movieService.getMoviesByCategories(userId);

        res.status(200).json(movies);
    } catch (err) {
        console.error('Error fetching movies:', err);
        res.status(400).json({ error: 'Failed to fetch movies' });
    }
};



const createMovie = async (req, res) => {
  try {
    const { title, category, cast, description, time, year, subtitlesAvailable} = req.body;

    // Extract files
    let video = null;
    let moviePicture = null;
    
    if (req.files && req.files.video && req.files.video[0]) {
      video = req.files.video[0].path;
    }
    
    if (req.files && req.files.moviePicture && req.files.moviePicture[0]) {
      moviePicture = req.files.moviePicture[0].path;
    }
    


    const newMovie = new Movie({
      title,
      category,
      cast: Array.isArray(cast) ? cast : [cast],
      description,
      time,
      year,
      subtitlesAvailable,
      moviePicture,
      video,
    });

    const savedMovie = await newMovie.save();

    res.status(201).json(savedMovie);
  } catch (err) {
    console.error('Error creating movie:', err.message);
    res.status(400).json({ error: 'Failed to create movie' });
  }
};

module.exports = { createMovie };


module.exports = { createMovie };


// Get a movie by ID
const getMovie = async (req, res) => {
    try {
        const id_name = req.header('id_name'); // Fetch id_name from the header

        if (!id_name) {
            // next exercise
         }

        const movie = await movieService.getMovieById(req.params.id);
        if (!movie) {
            return res.status(404).json({ error: 'Movie not found' }); // Movie not found
        }

        res.status(200).json(movie); // Return movie with 200 OK
    } catch (err) {
        console.error('Error fetching movie by ID:', err);
        res.status(400).json({ error: 'Failed to fetch movie' }); // Return 400 for unexpected errors
    }
};

// Replace a movie by ID

const replaceMovie = async (req, res) => {
  try {
    const { id } = req.params; // Extract movie ID from URL parameters
    const movieData = req.body; // Extract movie data from request body

    // Extract file paths from uploaded files
    let video = null;
    let moviePicture = null;

    if (req.files && req.files.video && req.files.video[0]) {
      video = req.files.video[0].path;
    }
    if (req.files && req.files.moviePicture && req.files.moviePicture[0]) {
      moviePicture = req.files.moviePicture[0].path;
    }

    // Add uploaded file paths to movieData
    if (video) movieData.video = video;
    if (moviePicture) movieData.moviePicture = moviePicture;

    // Find the movie in the database by ID
    const movie = await Movie.findById(id);

    if (!movie) {
      return res.status(404).json({ error: `Movie with ID ${id} not found` });
    }

    // Update the movie with new data
    Object.assign(movie, movieData);
    const updatedMovie = await movie.save();

    res.status(200).json(updatedMovie); // Return the updated movie
  } catch (err) {
    console.error('Error replacing movie:', err.message);
    res.status(500).json({ error: 'Failed to replace movie' });
  }
};




// Delete a movie by ID
const deleteMovie = async (req, res) => {
    try {
        const id_name = req.header('id_name'); // Fetch id_name from the header

        if (!id_name) {
            // next exercise
         }

        const deletedMovie = await movieService.deleteMovieById(req.params.id);
        if (!deletedMovie) {
            return res.status(404).json({ error: 'Movie not found' }); // Movie not found
        }

        res.status(204).send(); // Return 204 No Content for successful deletion
    } catch (err) {
        console.error('Error deleting movie:', err);
        res.status(400).json({ error: 'Failed to delete movie' }); // Return 400 for unexpected errors
    }
};

// Search for movies
const searchMovies = async (req, res) => {
    try {
        const id_name = req.header('id_name'); // Fetch id_name from the header

        if (!id_name) {
            // next exercise
         }

        const query = req.params.query; // Extract query from the URL

        if (!query) {
            return res.status(400).json({ error: 'Query parameter is required' }); // Bad Request
        }

        const movies = await movieService.searchMovies(query);

        if (movies.length === 0) {
            return res.status(204).send(); // No movies found
        }

        res.status(200).json(movies); // Return movies with 200 OK
    } catch (err) {
        console.error('Error searching movies:', err);
        res.status(400).json({ error: 'Failed to search movies' }); // Return 400 for unexpected errors
    }
};

// Recommend movies
const recommend = async (req, res) => {
    try {
        const id_name = req.header('id_name'); // Fetch id_name from the header
        const { id } = req.params; // Fetch id from the route parameter
        
        // Validate parameters
        if (!id_name || !id) {
            return res.status(400).json({ error: 'Bad Request' }); // Missing required data
        }

        // Fetch recommendation from the service
        let recommendation = await movieService.getRecommendation(id_name, id);

        if (!recommendation) {
            return res.status(404).json({ error: 'Not Found' });
        }

        // Parse and clean the recommendation response
        recommendation = recommendation.replace(/200 Ok\n\n/, '').trim().split(/\s+/);

        // Filter valid ObjectIds
        const validRecommendationIds = recommendation.filter((movieId) =>
            mongoose.Types.ObjectId.isValid(movieId)
        );

        // Fetch movies from the database with additional details
        const movies = await Movie.find(
            { _id: { $in: validRecommendationIds } },
            { title: 1, moviePicture: 1 , video: 1} // Fetch only `title` and `moviePicture`
        );

        if (!movies || movies.length === 0) {
            return res.status(404).json({ error: 'No recommended movies found' });
        }

        console.log(movies); // Verify that `video` is present

        // Format the response
        const recommendedMovies = movies.map((movie) => ({
            _id: movie._id,
            title: movie.title,
            moviePicture: movie.moviePicture,
            video: movie.video
        }));
        console.log(recommendedMovies);
        return res.status(200).json({ recommendedMovies });
    } catch (err) {
        console.error('Error fetching recommendations:', err.message);
        return res.status(500).json({ error: 'Internal Server Error' });
    }
};


const addMovie = async (req, res) => {
    try {
        const id_name = req.header('id_name'); // Extract `id_name` from headers
        const { id } = req.params; // Extract `id` (movie ID) from route parameters

        // Validate the presence of `id_name` and `id`
        if (!id_name || !id) {
            return res.status(400).json({ error: 'id_name and id are required.' });
        }

        // Find the movie by ID
        const movie = await Movie.findById(id);
        if (!movie) {
            return res.status(404).json({ error: 'Movie not found.' });
        }

        // Check if the movie is already in the user's watched list
        if (movie.watchedBy.includes(id_name)) {
            return res.status(200).json({ message: 'Movie already watched.' });
        }

        // Add the user to the watched list for the movie
        movie.watchedBy.push(id_name);
        await movie.save();

        // Add the movie to the user's watched list in the service (if applicable)
        const addResult = await movieService.addMovieToService(id_name, id);

        // Handle potential issues with the external service
        if (!addResult || addResult === '404 Not Found') {
            console.error('Error adding movie to service:', addResult);
            return res.status(500).json({ error: 'Failed to add movie to the service.' });
        }

        // Return success response
        res.status(204).json({ message: 'Movie added to watched list successfully.' });
    } catch (err) {
        console.error('Error in addMovie:', err.message);
        res.status(500).json({ error: 'Internal Server Error.' });
    }
};


// Export all functions
module.exports = { addMovie, recommend, getMovies, createMovie, getMovie, replaceMovie, deleteMovie, searchMovies,  };