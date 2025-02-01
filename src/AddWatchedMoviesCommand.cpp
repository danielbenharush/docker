#include "AddWatchedMoviesCommand.hpp"
#include <sstream>
#include <algorithm>
#include <string>
#include <iostream>



// Constructor: initialize with a reference to UserManager
AddWatchedMoviesCommand::AddWatchedMoviesCommand(UserManager* manager) : userManager(manager) {}

// Execute the POST command to add a user's watched movies
std::string AddWatchedMoviesCommand::execute(const std::string& input) {
    std::cout << input << std::endl;

    std::istringstream iss(input);
    std::string userId;

    // Parse the user ID from the input
    if (!(iss >> userId)) {
        return "400 Bad Request"; // Invalid input format
    }

    if (userManager->userExists(userId)) {
        return "404 Not Found"; // User does not exist
    }

    std::vector<std::string> movies;
    std::string movieId;

    // Parse the movie IDs from the input
    while (iss >> movieId) {
        if (std::find(movies.begin(), movies.end(), movieId) == movies.end()) {
            movies.push_back(movieId); // Avoid duplicate movie IDs
        }
    }

    // Attempt to add the user and their movies
    if (!userManager->addUser(userId, movies)) {
        return "400 Bad Request"; // User already exists
    }

    return "201 Created"; // Successfully added the user
}
