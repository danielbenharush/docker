const mongoose = require('mongoose');
const Schema = mongoose.Schema;

const user = new Schema({
  name: { type: String, required: true },
  mail: { type: String, required: true },
  password: { type: String, required: true },
  phone: { type: String, required: true },
  profilePicture: { type: String }, // Stores the file path
  role: { type: String, required: true, default: 'user' }, // User role
});

module.exports = mongoose.model('user', user);
