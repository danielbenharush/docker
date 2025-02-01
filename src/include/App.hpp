#ifndef APP_HPP
#define APP_HPP

#include <map>
#include <string>
#include "ICommand.hpp"
#include "IMenu.hpp"
#include "UserManager.hpp"

class App {
private:
    IMenu* menu;
    std::map<std::string, ICommand*> commands;
    UserManager* userManager;

public:
    App(IMenu* menu, std::map<std::string, ICommand*> commands, UserManager* userManager);
    std::string processCommand(const std::string& fullInput);
};

#endif // APP_HPP
