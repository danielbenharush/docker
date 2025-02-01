#include <gtest/gtest.h>
#include "UserManager.hpp"
#include "DeleteCommand.hpp"

// Fixture for DeleteCommand tests
class DeleteCommandTest : public ::testing::Test {
protected:
    std::string testFileName = "test_user_data.txt"; // Temporary test file
    UserManager* userManager;
    DeleteCommand* deleteCommand;

    // Setup test environment
    void SetUp() override {
        // Initialize UserManager with a test file
        userManager = new UserManager(testFileName);

        // Add sample data
        userManager->addUser(1, {101, 102, 103});
        userManager->addUser(2, {201, 202, 203});

        // Create DeleteCommand object
        deleteCommand = new DeleteCommand(userManager);
    }

    // Tear down after tests
    void TearDown() override {
        delete userManager;
        delete deleteCommand;

        // Remove the test file
        std::remove(testFileName.c_str());
    }
};

// Test: Deleting existing movies from a user
TEST_F(DeleteCommandTest, DeleteExistingMovies) {
    std::string result = deleteCommand->execute("1 101 102");
    EXPECT_EQ(result, "204 No Content");

    // Verify the movies are deleted
    std::vector<int> expectedMovies = {103};
    EXPECT_EQ(userManager->getMutableMovies(1), expectedMovies);
}

// Test: Deleting movies that do not exist
TEST_F(DeleteCommandTest, DeleteNonExistentMovies) {
    std::string result = deleteCommand->execute("1 999");
    EXPECT_EQ(result, "400 Bad Request");
}

// Test: Deleting movies from a non-existent user
TEST_F(DeleteCommandTest, DeleteMoviesFromNonExistentUser) {
    std::string result = deleteCommand->execute("999 101");
    EXPECT_EQ(result, "404 Not Found");
}

// Test: Sending an empty movie list
TEST_F(DeleteCommandTest, DeleteEmptyMovieList) {
    std::string result = deleteCommand->execute("1");
    EXPECT_EQ(result, "400 Bad Request");
}

// Test: Deleting all movies from a user
TEST_F(DeleteCommandTest, DeleteAllMovies) {
    std::string result = deleteCommand->execute("2 201 202 203");
    EXPECT_EQ(result, "204 No Content");

    // Verify the user's movie list is empty
    EXPECT_TRUE(userManager->getMutableMovies(2).empty());
}
