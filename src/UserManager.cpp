#include "UserManager.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

// Constructor: initialize UserManager and load data from the file
UserManager::UserManager(const std::string& fileName) : dataFile(fileName) {
    loadFromFile();
}

// Add a user and their movies to the map; return false if the user already exists
bool UserManager::addUser(std::string userId, const std::vector<std::string>& movies) {
    if (movies.empty()) {
        return false; // The userMovies map is empty
    }
    if (userMovies.find(userId) != userMovies.end()) {
        return false; // User already exists
    }
    userMovies[userId] = movies; // Add the user and their movies
    saveToFile(); // Save the updated data to the file
    return true;
}

// Check if a user exists in the map
bool UserManager::userExists(std::string userId) const {
    return userMovies.find(userId) != userMovies.end();
}

std::vector<std::string>& UserManager::getMutableMovies(std::string userId) {
    return userMovies[userId]; // Return a reference to the user's movie list
}

// Save user data to the file (overwrite existing content)
void UserManager::saveToFile() {
    std::ofstream file(dataFile, std::ios_base::trunc); // Open file in truncate mode
    if (!file) return; // Silently fail if file can't be opened

    // Write each user and their movies to the file
    for (const auto& [userId, movies] : userMovies) {
        file << userId;
        for (std::string movieId : movies) {
            file << " " << movieId;
        }
        file << "\n"; // End the line after each user
    }
    file.close();
}

// Load user data from the file
void UserManager::loadFromFile() {
    std::ifstream file(dataFile);
    if (!file) return; // If file doesn't exist, no data to load

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string userId, movieId;

        // Read user ID and their movies
        iss >> userId;
        while (iss >> movieId) {
            userMovies[userId].push_back(movieId);
        }
    }
    file.close();
}
