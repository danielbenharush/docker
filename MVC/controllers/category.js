const categoryService = require('../services/category');
const mongoose = require('mongoose');

// Fetch all categories
const getCategories = async (req, res) => {
    try {
        // Use service to fetch all categories from the database
        const categories = await categoryService.getAllCategories();
        // Send the categories as a JSON response
        res.status(200).json(categories); // Return 200 OK
    } catch (err) {
        console.error('Error fetching categories:', err.message); // Log the error for debugging
        res.status(400).json({ error: 'Failed to fetch categories' }); // Return 400 Bad Request
    }
};

// Create a new category
const createCategory = async (req, res) => {
    try {
        console.log(req.body);
        // Extract name and promoted values from the request body
        const { name, promoted } = req.body;

        // Validate that the name is provided
        if (!name) {
            return res.status(400).json({ error: 'Name is required' }); // Return a 400 Bad Request
        }

        // Pass the category data to the service to create a new category
        const newCategory = await categoryService.createCategory({ name, promoted });

        // Add Location header for the newly created category
        res.setHeader('Location', `/api/categories/${newCategory._id}`);
        res.status(201).send(); // Return 201 Created with Location header
    } catch (err) {
        console.error('Error creating category:', err.message); // Log the error for debugging
        res.status(400).json({ error: 'Failed to create category' }); // Return 400 Bad Request
    }
};

// Get a category by ID
const getCategory = async (req, res) => {
    try {
        const { id } = req.params; // Extract the category ID from the request parameters

        // Validate if the provided ID is a valid MongoDB ObjectId
        if (!mongoose.Types.ObjectId.isValid(id)) {
            return res.status(404).json({ error: 'Category not found' }); // Return 404 if invalid
        }

        // Fetch the category by ID using the service
        const category = await categoryService.getCategoryById(id);

        // If the category does not exist, return 404
        if (!category) {
            return res.status(404).json({ error: 'Category not found' });
        }

        // Return the category details with a 200 OK status
        res.status(200).json(category);
    } catch (err) {
        console.error('Error fetching category:', err.message); // Log the error for debugging
        res.status(400).json({ error: 'Failed to fetch category' }); // Return 400 Bad Request
    }
};

// Update a category
const updateCategory = async (req, res) => {
    try {

        const { id } = req.params;

        // Check if the provided ID is a valid MongoDB ObjectId
        if (!mongoose.Types.ObjectId.isValid(id)) {
            return res.status(404).json({ error: "Category not found" }); // Return 404 if the ID is invalid
        }

        const updates = req.body;


        // Attempt to update the category in the database
        const updatedCategory = await categoryService.updateCategoryById(id, updates);

        // Check if the category was not found in the database
        if (!updatedCategory) {
            return res.status(404).json({ error: "Category not found" }); // Return 404 if the category does not exist
        }

        res.status(200).json(updatedCategory); // Return the updated category with a 200 OK status
    } catch (err) {
        console.error("Error updating category:", err); // Log the error to the server logs
        res.status(400).json({ error: "Failed to update category" }); // Return a 400 Bad Request
    }
};

// Delete a category by ID
const deleteCategory = async (req, res) => {
    try {
        const { id } = req.params; // Extract the category ID from the request parameters

        // Validate if the provided ID is a valid MongoDB ObjectId
        if (!mongoose.Types.ObjectId.isValid(id)) {
            return res.status(404).json({ error: 'Category not found' }); // Return 404 if invalid
        }

        // Use the service to delete the category from the database
        const deletedCategory = await categoryService.deleteCategoryById(id);

        // If the category was not found, return 404
        if (!deletedCategory) {
            return res.status(404).json({ error: 'Category not found' });
        }

        // Send a 204 No Content status for successful deletion
        res.status(204).send();
    } catch (err) {
        console.error('Error deleting category:', err.message); // Log the error for debugging
        res.status(400).json({ error: 'Failed to delete category' }); // Return 400 Bad Request
    }
};

// Export all the controller functions
module.exports = { getCategories, createCategory, getCategory, updateCategory, deleteCategory };
