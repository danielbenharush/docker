#include "ConsoleMenu.hpp"
#include <gtest/gtest.h>
#include <sstream>
#include <iostream>

// Test the output of the displayMenu function
TEST(ConsoleMenuTest, DisplayMenuOutput) {
    // Create an instance of ConsoleMenu
    ConsoleMenu menu;

    // Get the output of displayMenu
    std::string output = menu.displayMenu();

    // Check that the output contains all the correct menu options
    EXPECT_NE(output.find("DELETE, arguments: [userid] [movieid1] [movieid2] ..."), std::string::npos);
    EXPECT_NE(output.find("GET, arguments: [userid] [movieid]"), std::string::npos);
    EXPECT_NE(output.find("PATCH, arguments: [userid] [movieid1] [movieid2] ..."), std::string::npos);
    EXPECT_NE(output.find("POST, arguments: [userid] [movieid1] [movieid2] ..."), std::string::npos);
    EXPECT_NE(output.find("help"), std::string::npos);
}

// Test the nextCommand function of ConsoleMenu
TEST(ConsoleMenuTest, NextCommandInput) {
    // Create an instance of ConsoleMenu
    ConsoleMenu menu;

    // Redirect std::cin to simulate user input
    std::stringstream input("test command\n");
    std::cin.rdbuf(input.rdbuf()); // Redirect std::cin to read from the stringstream

    // Get the command entered by the user
    std::string command = menu.nextCommand();

    // Verify that the command matches the input
    EXPECT_EQ(command, "test command");
}
