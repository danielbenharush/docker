const express = require('express');
const userController = require('../controllers/user');
const upload = require('../middleware/upload'); // Import the upload middleware

const router = express.Router();

// Create a new user with profile picture upload
router.post('/', upload.single('profilePicture'), userController.createUser);

// Get a user by ID
router.get('/:id', userController.getUser);

module.exports = router;
