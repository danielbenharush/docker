#include "App.hpp"
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include "DeleteCommand.hpp"

// Constructor
App::App(IMenu* menu, std::map<std::string, ICommand*> commands, UserManager* userManager)
    : menu(menu), commands(commands), userManager(userManager) {
}

// Process a single command string and return a response
std::string App::processCommand(const std::string& fullInput) {
    try {
        // Parse the command
        std::istringstream iss(fullInput);
        std::string command;
        iss >> command; // Extract the command

        // Normalize the command (trim spaces and convert to lowercase)
        command.erase(0, command.find_first_not_of(" \t"));  // Trim leading spaces
        command.erase(command.find_last_not_of(" \t") + 1); // Trim trailing spaces
        std::transform(command.begin(), command.end(), command.begin(), ::tolower); // Convert to lowercase

        // Check if the command exists and is valid
        if (!command.empty() && commands.find(command) != commands.end()) {
            std::string parameters;
            if (fullInput.length() > command.length() + 1) {
                parameters = fullInput.substr(command.length() + 1); // Extract parameters
            }
            // Execute the command with parameters and return a success message
            return commands[command]->execute(parameters); 
        } else {
            // Return error message for unknown command
            return "400 Bad Request";
        }
    } catch (const std::exception& e) {
        // Return error message if an exception is encountered
        return "Error encountered: " + std::string(e.what());
    }
}
