const jwt = require('jsonwebtoken');
const userService = require('../services/user');

const SECRET_KEY = 'your-secret-key'; // Use a strong secret key

// POST /api/tokens
const authenticateUser = async (req, res) => {
    try {
        const { username, password } = req.body; // Extract username and password from the request body

        // Validate input
        if (!username || !password) {
            return res.status(400).json({ error: 'Username and password are required' });
        }

        // Check if the user exists
        const user = await userService.findUserByCredentials(username, password);
        if (!user) {
            return res.status(401).json({ error: 'Invalid username or password' });
        }

        // Generate a JWT with the user's role
        const token = jwt.sign(
            { id: user.id, username: user.name, role: user.role , profilePicture: user.profilePicture}, // Include the role in the payload
            SECRET_KEY, // Secret key
            { expiresIn: '1h' } // Token expiration time
        );

        // Send the token
        res.status(200).json({ token });
    } catch (err) {
        console.error('Error during authentication:', err.message);
        res.status(500).json({ error: 'Internal server error' });
    }
};

module.exports = { authenticateUser };
