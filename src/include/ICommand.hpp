#ifndef ICOMMAND_HPP
#define ICOMMAND_HPP

#include <string>

class ICommand {
public:
    virtual ~ICommand() = default; // Virtual destructor for cleanup
    virtual std::string execute(const std::string& input) = 0; // Pure virtual method
};

#endif // ICOMMAND_HPP
