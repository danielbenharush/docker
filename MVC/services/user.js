const User = require('../models/user');

// Find user by credentials
const findUserByCredentials = async (name, password) => {
  return await User.findOne({ name, password }); // Match username and plain text password
};

// Create a new user
const createUser = async (name, mail, password, phone, profilePicture, role) => {
  // Check if a user with the same email or name already exists
  const existingUser = await User.findOne({ $or: [{ mail }, { name }] });

  if (existingUser) {
    throw new Error('User with the same email or name already exists');
  }

  // If no user exists, create a new user
  const user = new User({ name, mail, password, phone, profilePicture, role });
  return await user.save();
};

// Get a single user by ID
const getUserById = async (id) => {
  const user = await User.findOne({ _id: id });

  if (user) {
    const { password, ...rest } = user.toObject();
    return rest;
  } else {
    return null;
  }
};

module.exports = { findUserByCredentials, createUser, getUserById };
