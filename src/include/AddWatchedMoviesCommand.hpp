#ifndef ADDWATCHEDMOVIESCOMMAND_HPP
#define ADDWATCHEDMOVIESCOMMAND_HPP

#include "ICommand.hpp"
#include "UserManager.hpp"

// Command to handle the addition of watched movies (POST functionality)
class AddWatchedMoviesCommand : public ICommand {
private:
    UserManager* userManager; // Pointer to the UserManager for user data management

public:
    AddWatchedMoviesCommand(UserManager* manager); // Constructor with UserManager dependency

    std::string execute(const std::string& input) override; // Execute the command
};

#endif
