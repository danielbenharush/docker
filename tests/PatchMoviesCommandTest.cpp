#include "PatchMoviesCommand.hpp"
#include "UserManager.hpp"
#include <gtest/gtest.h>
#include <algorithm> // for std::count

// Test case: Check if a movie is written twice
TEST(PatchMoviesCommandTest, CheckDuplicateMoviesWritten) {
    UserManager userManager("test_data.txt");
    userManager.addUser(1, {100}); // Add a user with one movie

    PatchMoviesCommand patchCommand(&userManager);
    patchCommand.execute("1 100 200"); // Add existing and new movie

    const auto& movies = userManager.getMutableMovies(1);

    // Debugging: Print movies list for verification
    std::cout << "Movies after PATCH: ";
    for (const auto& movie : movies) {
        std::cout << movie << " ";
    }
    std::cout << std::endl;

    // Check if a movie is written twice
    EXPECT_EQ(std::count(movies.begin(), movies.end(), 100), 1); // Movie 100 should appear only once
    EXPECT_EQ(std::count(movies.begin(), movies.end(), 200), 1); // Movie 200 should appear only once
}
