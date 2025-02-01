#include "HelpCommand.hpp"
#include "IMenu.hpp"
#include <gtest/gtest.h>
#include <sstream>

// Mock implementation of IMenu
class MockConsoleMenu : public IMenu {
public:
    // Mock implementation of displayMenu
    std::string displayMenu() override {
        return "DELETE, arguments: [userid] [movieid1] [movieid2] ...\n"
               "GET, arguments: [userid] [movieid]\n"
               "PATCH, arguments: [userid] [movieid1] [movieid2] ...\n"
               "POST, arguments: [userid] [movieid1] [movieid2] ...\n"
               "help\n";
    }

    // Mock implementation of nextCommand
    std::string nextCommand() override {
        return "mock command"; // Return a mock command
    }

    // Mock implementation of displayError
    void displayError(const std::string& error) override {
        // Do nothing
    }
};

TEST(HelpCommandTest, ExecuteHelp) {
    // Create a mock menu and HelpCommand instance
    MockConsoleMenu menu;
    HelpCommand help(&menu);

    // Execute the help command
    std::string output = help.execute("");

    // Validate the output matches the menu's display
    EXPECT_NE(output.find("DELETE, arguments: [userid] [movieid1] [movieid2] ..."), std::string::npos);
    EXPECT_NE(output.find("GET, arguments: [userid] [movieid]"), std::string::npos);
    EXPECT_NE(output.find("PATCH, arguments: [userid] [movieid1] [movieid2] ..."), std::string::npos);
    EXPECT_NE(output.find("POST, arguments: [userid] [movieid1] [movieid2] ..."), std::string::npos);
    EXPECT_NE(output.find("help"), std::string::npos);
}
