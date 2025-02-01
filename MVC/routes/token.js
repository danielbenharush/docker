const express = require('express');
const router = express.Router();
const tokenController = require('../controllers/token');

// Verify user and generate a token
router.post('/', tokenController.authenticateUser);

module.exports = router;
