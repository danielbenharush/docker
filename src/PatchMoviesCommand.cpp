#include "PatchMoviesCommand.hpp"
#include <sstream>
#include <algorithm>
#include <cctype> 

// Constructor: initialize with a reference to UserManager
PatchMoviesCommand::PatchMoviesCommand(UserManager* manager) : userManager(manager) {}

// Execute the PATCH command to update a user's watched movies
std::string PatchMoviesCommand::execute(const std::string& input) {
    std::istringstream iss(input);
    std::string userId;

    // Parse the user ID from the input
    if (!(iss >> userId)) {
        return "400 Bad Request"; // Invalid input format
    }


    std::vector<std::string> newMovies;
    std::string movieId;

    // Parse the movie IDs from the input
   while (iss >> movieId) {
    newMovies.push_back(movieId);
}

    if (newMovies.empty()) {
        return "400 Bad Request";
    }

      // Check if the user exists
    if (!userManager->userExists(userId)) {
        return "404 Not Found"; // User does not exist
    }

    // Update the user's movie list without duplicating existing movies
    std::vector<std::string>& userMovies = userManager->getMutableMovies(userId); // Access user's movie list
    for (std::string movie : newMovies) {
        if (std::find(userMovies.begin(), userMovies.end(), movie) == userMovies.end()) {
            userMovies.push_back(movie); // Add only unique movies
        }
    }

    userManager->saveToFile(); // Save the updated data
    return "204 No Content"; // Successfully updated the user's movies
}
