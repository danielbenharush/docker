#ifndef USERMANAGER_HPP
#define USERMANAGER_HPP

#include <map>
#include <vector>
#include <string>

// Class to manage user data (user-movie mapping) and handle file persistence
class UserManager {
private:
    std::map<std::string, std::vector<std::string>> userMovies; // Map to store user IDs and their watched movies
    std::string dataFile; // Path to the file for saving/loading user data

public:
    UserManager(const std::string& fileName); // Constructor to initialize with the file name

    bool addUser(std::string userId, const std::vector<std::string>& movies); // Add a new user with their movies
    bool userExists(std::string userId) const; // Check if a user exists
    std::vector<std::string>& getMutableMovies(std::string userId); // Get a reference to a user's movie list

    void saveToFile(); // Save all user data to the file
    void loadFromFile(); // Load user data from the file
};

#endif
