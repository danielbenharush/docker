#ifndef CONSOLEMENU_HPP
#define CONSOLEMENU_HPP

#include "IMenu.hpp"
#include <string>

class ConsoleMenu : public IMenu {
public:
    ConsoleMenu(); // Constructor
    std::string displayMenu() override; // Display the menu options
    std::string nextCommand() override; // Get the next command from the user
    void displayError(const std::string& error) override; // Display error (not implemented)
};

#endif // CONSOLEMENU_HPP
