const Category = require('../models/category');

// Fetch all categories from the database
const getAllCategories = async () => {
    return await Category.find(); // Return all categories
};

// Create a new category in the database
const createCategory = async (categoryData) => {
    const newCategory = new Category(categoryData); // Create a new category object with all provided data
    return await newCategory.save(); // Save the category and return it
};

// Get a category by ID
const getCategoryById = async (id) => {
    return await Category.findById(id); // Fetch category by ID
};

// Update a category by ID
const updateCategoryById = async (id, updates) => {
    return await Category.findByIdAndUpdate(id, updates, { new: true }); // Update and return the updated document
};

// Delete a category by ID
const deleteCategoryById = async (id) => {
    return await Category.findByIdAndDelete(id); // Delete category by ID
};

module.exports = {
    getAllCategories,
    createCategory,
    getCategoryById,
    updateCategoryById,
    deleteCategoryById
};
