#include "DeleteCommand.hpp"
#include <sstream>
#include <iterator>
#include <algorithm>
#include <cctype> 

DeleteCommand::DeleteCommand(UserManager* userManager) : userManager(userManager) {}

std::string DeleteCommand::execute(const std::string& params) {
    std::istringstream iss(params);
    std::string userIdStr;
iss >> userIdStr;


    // Check if user exists
    if (!userManager->userExists(userIdStr)) {
        return "404 Not Found";
    }

    // Parse movies to delete from input
    std::vector<std::string> moviesToDelete {
        std::istream_iterator<std::string>(iss),
        std::istream_iterator<std::string>()
    };

    if (moviesToDelete.empty()) {
        return "400 Bad Request";
    }

    // Get reference to user's movie list
    std::vector<std::string>& userMovies = userManager->getMutableMovies(userIdStr);

    // Check if all movies exist in user's list
    for (std::string movie : moviesToDelete) {
        if (std::find(userMovies.begin(), userMovies.end(), movie) == userMovies.end()) {
            return "404 Not Found";
        }
    }

    // Remove movies from user's list
    for (std::string movie : moviesToDelete) {
        userMovies.erase(std::remove(userMovies.begin(), userMovies.end(), movie), userMovies.end());
    }

    // If user's list is empty, clear it
    if (userMovies.empty()) {
        userManager->getMutableMovies(userIdStr).clear();
    }

    userManager->saveToFile();
    return "204 No Content";
}
