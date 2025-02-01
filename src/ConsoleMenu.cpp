#include "ConsoleMenu.hpp"
#include <iostream> // Ensure iostream is included
#include <sstream>  // For std::ostringstream
using namespace std;

// Constructor
ConsoleMenu::ConsoleMenu() {}

// Display the menu options as a string
std::string ConsoleMenu::displayMenu() {
    std::ostringstream output;

    // Add menu options with "arguments" for commands that require them
    output << "DELETE, arguments: [userid] [movieid1] [movieid2] ...\n";
    output << "GET, arguments: [userid] [movieid]\n";
    output << "PATCH, arguments: [userid] [movieid1] [movieid2] ...\n";
    output << "POST, arguments: [userid] [movieid1] [movieid2] ...\n";

    // Add "help" as the last command, without arguments
    output << "help\n";

    return output.str(); // Return the constructed menu as a string
}

// Get the next command from the user
std::string ConsoleMenu::nextCommand() {
    std::string command;
    std::getline(cin, command); // Read the user's input
    return command;
}

// Ignore error messages as per requirements
void ConsoleMenu::displayError(const std::string& error) {
    // No implementation needed
}
