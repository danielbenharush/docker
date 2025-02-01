#ifndef RECOMMENDCOMMAND_HPP
#define RECOMMENDCOMMAND_HPP

#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <stdexcept>
#include <iterator>
#include <numeric>
#include "ICommand.hpp"

class RecommendCommand : public ICommand {
public:
    explicit RecommendCommand(const std::string& fileName);

    std::string execute(const std::string& input);

private:
    std::string fileName;

    // Helper functions
    std::string cleanLine(const std::string& line);
    std::vector<std::string> parseInput(const std::string& input);
    std::vector<std::string> getUserMovies(const std::string& userId);
    void calculateCommonMovies(
        const std::string& id,
        const std::unordered_set<std::string>& ourUserMovieSet,
        std::unordered_map<std::string, int>& commonMoviesMap,
        std::unordered_set<std::string>& relevantUsers
    );
    std::unordered_map<std::string, int> calculateMovieScores(
        const std::unordered_set<std::string>& relevantUsers,
        const std::unordered_set<std::string>& ourUserMovieSet,
        const std::unordered_map<std::string, int>& commonMoviesMap,
        const std::string& movieId
    );
    std::string printTopMovies(const std::unordered_map<std::string, int>& movieScores);
};

#endif
