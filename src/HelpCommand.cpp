#include "HelpCommand.hpp"
#include <sstream>

// Constructor
HelpCommand::HelpCommand(IMenu* menu) : menu(menu) {}

// Execute method
std::string HelpCommand::execute(const std::string& input) {
    std::istringstream iss(input);
    std::string command;
    std::string extra;

    // Extract only the command and check for remaining input
    iss >> command;
    if (iss >> extra) { // If there is any extra input, return 400 Bad Request
        return "400 Bad Request";
    }

    // If the command is exactly "help" and no additional words, return the menu
    if (command == "help") {
        return "200 Ok\n\n" + menu->displayMenu();
    }

    // For any other case, return 400 Bad Request
    return "400 Bad Request";
}