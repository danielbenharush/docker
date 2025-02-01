#ifndef IMENU_HPP
#define IMENU_HPP

#include <string>

// Interface for the Menu system
class IMenu {
public:

    virtual std::string displayMenu() = 0;

    virtual ~IMenu() = default; // Virtual destructor for proper cleanup

    // Pure virtual function to retrieve the next command from the user.
    
    virtual std::string nextCommand() = 0;

    // pure virtual func
   
    virtual void displayError(const std::string& error) = 0;
};

#endif // IMENU_HPP
