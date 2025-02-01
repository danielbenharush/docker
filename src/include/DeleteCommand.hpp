#ifndef DELETE_COMMAND_HPP
#define DELETE_COMMAND_HPP

#include "ICommand.hpp"
#include "UserManager.hpp"

class DeleteCommand : public ICommand {
private:
    UserManager* userManager; // Reference to user manager

public:
    explicit DeleteCommand(UserManager* userManager); // Constructor
    std::string execute(const std::string& params) override; // Execute command
};

#endif // DELETE_COMMAND_HPP
