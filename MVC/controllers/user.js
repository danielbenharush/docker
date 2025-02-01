const userService = require('../services/user');

const createUser = async (req, res) => {
  try {
    const { name, mail, password, phone , role } = req.body;
    const profilePicture = req.file ? req.file.path : null; // Get the uploaded file's path

    if (!name || !mail || !password || !phone) {
      return res.status(400).json({ error: 'All fields (name, mail, password, phone) are required' });
    }

    const savedUser = await userService.createUser(name, mail, password, phone, profilePicture, role);

    res.status(201).json(savedUser);
  } catch (err) {
    res.status(400).json({ error: err.message || 'Failed to create user' });
  }
};

const getUser = async (req, res) => {
  try {
    const { id } = req.params;
    const user = await userService.getUserById(id);

    if (!user) {
      return res.status(404).json({ error: 'User not found' });
    }

    res.status(200).json(user);
  } catch (err) {
    res.status(400).json({ error: 'Failed to fetch user' });
  }
};

module.exports = { createUser, getUser };
