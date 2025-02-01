#include "AddWatchedMoviesCommand.hpp"
#include "UserManager.hpp"
#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include <sstream>

// Helper function to clear the default data file
void clearFile() {
    std::ofstream file("data/watched_movies.txt", std::ios::trunc);
    file.close();
}

// Fixture for AddWatchedMoviesCommand tests
class AddCommandTest : public ::testing::Test {
protected:
    UserManager userManager; // Create a UserManager instance
    AddWatchedMoviesCommand command;

    AddCommandTest()
        : userManager("data/watched_movies.txt"), command(&userManager) {}

    void SetUp() override {
        // Clear the file before each test
        clearFile();
    }

    void TearDown() override {
        // Clear the file after each test
        clearFile();
    }
};

// Test: Adding a single movie to a new user
TEST_F(AddCommandTest, AddSingleMovie) {
    std::string response = command.execute("101 2001");
    EXPECT_EQ(response, "201 Created");

    // Check the file content
    std::ifstream file("data/watched_movies.txt");
    ASSERT_TRUE(file.is_open()) << "Failed to open watched_movies.txt";
    std::string line;
    ASSERT_TRUE(std::getline(file, line)) << "Failed to read line from watched_movies.txt";
    EXPECT_EQ(line, "101 2001");
}

// Test: Adding multiple movies to a new user
TEST_F(AddCommandTest, AddMultipleMovies) {
    std::string response = command.execute("101 2001 2002 2003");
    EXPECT_EQ(response, "201 Created");

    // Check the file content
    std::ifstream file("data/watched_movies.txt");
    ASSERT_TRUE(file.is_open()) << "Failed to open watched_movies.txt";
    std::string line;
    ASSERT_TRUE(std::getline(file, line)) << "Failed to read line from watched_movies.txt";
    EXPECT_EQ(line, "101 2001 2002 2003");
}

// Test: Adding movies to an existing user
TEST_F(AddCommandTest, AddMoviesToExistingUser) {
    std::string response = command.execute("101 2001 2002");
    EXPECT_EQ(response, "201 Created");

    // Attempt to add movies to the same user
    response = command.execute("101 2003 2004");
    EXPECT_EQ(response, "400 Bad Request");

    // Check the file content remains unchanged
    std::ifstream file("data/watched_movies.txt");
    ASSERT_TRUE(file.is_open()) << "Failed to open watched_movies.txt";
    std::string line;
    ASSERT_TRUE(std::getline(file, line)) << "Failed to read line from watched_movies.txt";
    EXPECT_EQ(line, "101 2001 2002");
}

// Test: Handling duplicate users
TEST_F(AddCommandTest, HandleDuplicateUsers) {
    std::string response = command.execute("101 2001 2002");
    EXPECT_EQ(response, "201 Created");

    // Attempt to create the same user again
    response = command.execute("101 2003 2004");
    EXPECT_EQ(response, "400 Bad Request");

    // Check the file content remains unchanged
    std::ifstream file("data/watched_movies.txt");
    ASSERT_TRUE(file.is_open()) << "Failed to open watched_movies.txt";
    std::string line;
    ASSERT_TRUE(std::getline(file, line)) << "Failed to read line from watched_movies.txt";
    EXPECT_EQ(line, "101 2001 2002");
}

// Test: Invalid command formats
TEST_F(AddCommandTest, HandleInvalidCommands) {
    std::string response;

    response = command.execute("INVALID 101 2001");
    EXPECT_EQ(response, "400 Bad Request");

    response = command.execute("POST");
    EXPECT_EQ(response, "400 Bad Request");

    response = command.execute("");
    EXPECT_EQ(response, "400 Bad Request");

    // Ensure no data is written
    std::ifstream file("data/watched_movies.txt");
    ASSERT_TRUE(file.is_open()) << "Failed to open watched_movies.txt";
    EXPECT_TRUE(file.peek() == std::ifstream::traits_type::eof()) << "File should be empty";
}
