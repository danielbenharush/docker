const express = require('express');
const bodyParser = require('body-parser');
const cors = require('cors');
const mongoose = require('mongoose');
const path = require('path');
const multer = require('multer'); // File upload middleware
const customEnv = require('custom-env');

// Import routes
const movies = require('./routes/movie');
const categories = require('./routes/category');
const users = require('./routes/user');
const tokensRoute = require('./routes/token');

// Load environment variables
customEnv.env('local', './config');

if (!process.env.CONNECTION_STRING) {
  process.env.CONNECTION_STRING = 'mongodb://127.0.0.1:27017/hello';
  process.env.PORT = 8080;
}

// MongoDB connection
mongoose.set('strictQuery', false);
mongoose
  .connect(process.env.CONNECTION_STRING, {
    useNewUrlParser: true,
    useUnifiedTopology: true,
  })
  .then(() => console.log('Connected to MongoDB'))
  .catch((err) => console.error('MongoDB connection error:', err.message));

// Initialize Express app
const app = express();

// Middleware
app.use(cors());
app.use(bodyParser.json({ limit: '100mb' })); // Increase payload limit
app.use(bodyParser.urlencoded({ extended: true, limit: '100mb' }));
app.use(express.json());

// Static file serving
app.use('/uploads', express.static(path.join(__dirname, 'uploads')));
app.use('/uploads/videos', express.static(path.join(__dirname, 'uploads/videos')));

// Routes
app.use('/api/movies', movies);
app.use('/api/categories', categories);
app.use('/api/users', users);
app.use('/api/tokens', tokensRoute);

// Error-handling middleware
app.use((err, req, res, next) => {
  if (err instanceof multer.MulterError) {
    // Handle multer-specific errors
    return res.status(400).json({ error: err.message });
  }
  if (err) {
    // Handle general errors
    return res.status(500).json({ error: err.message });
  }
  next();
});

// Start the server
const PORT = process.env.PORT || 8080;
app.listen(PORT, () => {
  console.log(`Server is running on port ${PORT}`);
});

module.exports = app;
