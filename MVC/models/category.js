const mongoose = require('mongoose');
const Schema = mongoose.Schema;

// Define the schema for a category
const categorySchema = new Schema({
    name: {
        type: String,
        required: true // Name is mandatory
    },
    promoted: {
        type: Boolean,
        default: false
    }
    
});

// Export the Category model
module.exports = mongoose.model('Category', categorySchema);
