#ifndef HELPCOMMAND_HPP
#define HELPCOMMAND_HPP

#include "ICommand.hpp"
#include "IMenu.hpp"
#include <string>

class HelpCommand : public ICommand {
private:
    IMenu* menu; // Pointer to the menu

public:
    explicit HelpCommand(IMenu* menu); // Constructor
    std::string execute(const std::string& input) override; // Execute method
};

#endif // HELPCOMMAND_HPP
