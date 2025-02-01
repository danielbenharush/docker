#include <gtest/gtest.h>
#include "RecommendCommand.hpp"
#include <fstream>
#include <sstream>

// Helper function to create a temporary test data file
void createTestDataFile(const std::string& fileName, const std::string& content) {
    std::ofstream file(fileName);
    file << content;
    file.close();
}

// Test for constructor
TEST(RecommendCommandTest, Constructor) {
    RecommendCommand command("testData.txt");
    SUCCEED(); // Ensure the constructor doesn't throw exceptions
}

// Test for parseInput
TEST(RecommendCommandTest, ParseInput) {
    RecommendCommand command("testData.txt");
    std::vector<std::string> result = command.parseInput("1 2 3 4");
    EXPECT_EQ(result, std::vector<std::string>({"1", "2", "3", "4"}));
}

// Test for getUserMovies
TEST(RecommendCommandTest, GetUserMovies) {
    std::string content = "1 101 102 103\n2 104 105\n3 106";
    createTestDataFile("testData.txt", content);

    RecommendCommand command("testData.txt");
    auto movies = command.getUserMovies("1");
    EXPECT_EQ(movies, std::vector<std::string>({"101", "102", "103"}));
}

// Test for calculateCommonMovies
TEST(RecommendCommandTest, CalculateCommonMovies) {
    std::string content = "1 101 102\n2 101 103\n3 104 105";
    createTestDataFile("testData.txt", content);

    RecommendCommand command("testData.txt");
    std::unordered_set<std::string> ourUserMovies = {"101", "102"};
    std::unordered_map<std::string, int> commonMoviesMap;
    std::unordered_set<std::string> relevantUsers;

    command.calculateCommonMovies("1", ourUserMovies, commonMoviesMap, relevantUsers);

    EXPECT_EQ(commonMoviesMap.size(), 1);
    EXPECT_EQ(commonMoviesMap["2"], 1);
    EXPECT_TRUE(relevantUsers.count("2"));
}

// Test for calculateMovieScores
TEST(RecommendCommandTest, CalculateMovieScores) {
    std::string content = "1 101 102\n2 101 103 104\n3 105 106";
    createTestDataFile("testData.txt", content);

    RecommendCommand command("testData.txt");
    std::unordered_set<std::string> relevantUsers = {"2"};
    std::unordered_set<std::string> ourUserMovies = {"101", "102"};
    std::unordered_map<std::string, int> commonMoviesMap = {{"2", 1}};

    auto scores = command.calculateMovieScores(relevantUsers, ourUserMovies, commonMoviesMap, "101");
    EXPECT_EQ(scores["104"], 1);
}

// Test for printTopMovies
TEST(RecommendCommandTest, PrintTopMovies) {
    RecommendCommand command("testData.txt");
    std::unordered_map<std::string, int> scores = {{"101", 5}, {"102", 3}, {"103", 8}};

    std::string output = command.printTopMovies(scores);
    EXPECT_EQ(output, "103 101 102");
}

// Test for execute
TEST(RecommendCommandTest, Execute) {
    std::string content = "1 101 102\n2 101 103 104\n3 105 106";
    createTestDataFile("testData.txt", content);

    RecommendCommand command("testData.txt");

    std::string result = command.execute("1 101");
    EXPECT_EQ(result, "200 Ok\n\n103 104");
}

// Main entry for tests
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
