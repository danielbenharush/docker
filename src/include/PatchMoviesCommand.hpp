#ifndef PATCHMOVIESCOMMAND_HPP
#define PATCHMOVIESCOMMAND_HPP

#include "ICommand.hpp"
#include "UserManager.hpp"

// Command to handle updating movies for an existing user (PATCH functionality)
class PatchMoviesCommand : public ICommand {
private:
    UserManager* userManager; // Pointer to UserManager instance

public:
    PatchMoviesCommand(UserManager* manager); // Constructor with UserManager dependency

    std::string execute(const std::string& input) override; // Execute the command
};

#endif
