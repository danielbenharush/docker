const express = require('express');
const movieController = require('../controllers/movie');
const upload = require('../middleware/uploadMovie'); // Use the combined upload middleware


const router = express.Router();

// Define the route for searching movies
router.get('/search/:query', movieController.searchMovies);

// Get a movie by ID
router.get('/:id', movieController.getMovie);

// Get movies by categories
router.get('/', movieController.getMovies);

// Create a new movie with video and picture uploads
router.post('/', upload, movieController.createMovie);


// Update a movie by ID
router.put('/:id',upload, movieController.replaceMovie);

// Delete a movie by ID
router.delete('/:id', movieController.deleteMovie);

// Recommend movies
router.get('/:id/recommend', movieController.recommend);

// Add movie to user
router.post('/:id/recommend', movieController.addMovie);

module.exports = router;
