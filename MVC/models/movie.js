const mongoose = require('mongoose');
const Schema = mongoose.Schema;

const movieSchema = new Schema({
  title: {
    type: String,
    required: true,
  },
  category: {
    type: String,
    required: true,
  },
  cast: {
    type: [String],
    required: true,
  },
  description: {
    type: String,
    required: true,
  },
  time: {
    type: String,
    required: true,
  },
  year: {
    type: Number,
    required: true,
  },
  subtitlesAvailable: {
    type: String,
    default: '',
  },
  watchedBy: {
    type: [String],
    default: [],
  },
  video: {
    type: String, // Path to the video file
    required: true,
  },
  moviePicture: {
    type: String, // Path to the movie picture
    required: true,
  },
});

module.exports = mongoose.model('Movie', movieSchema);
