const express = require('express');
const router = express.Router();
const categoryController = require('../controllers/category');

// Get all categories
router.get('/', categoryController.getCategories);

// Create a new category
router.post('/', categoryController.createCategory);

// Get a category by ID
router.get('/:id', categoryController.getCategory);

// Update a category by ID
router.put('/:id', categoryController.updateCategory);

// Delete a category by ID
router.delete('/:id', categoryController.deleteCategory);

module.exports = router;
