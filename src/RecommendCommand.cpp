
#include "RecommendCommand.hpp"

// Constructor
RecommendCommand::RecommendCommand(const std::string& fileName) : fileName(fileName) {}

// Utility function to clean a line
std::string RecommendCommand::cleanLine(const std::string& line) {
    std::string cleanedLine;
    for (char c : line) {
        if (std::isprint(static_cast<unsigned char>(c))) {
            cleanedLine += (std::isspace(static_cast<unsigned char>(c)) ? ' ' : c);
        }
    }
    std::istringstream iss(cleanedLine);
    std::ostringstream oss;
    std::string word;
    while (iss >> word) {
        if (!oss.str().empty()) oss << " ";
        oss << word;
    }
    return oss.str();
}

// Parse input string
std::vector<std::string> RecommendCommand::parseInput(const std::string& input) {
    std::vector<std::string> result;
    std::istringstream iss(input);
    std::string num;
    while (iss >> num) {
        result.push_back(num);
    }
    return result;
}

// Get the list of movies watched by a specific user
std::vector<std::string> RecommendCommand::getUserMovies(const std::string& userId) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open the file: " + fileName);
    }

    std::string line;
    while (getline(file, line)) {
        line = cleanLine(line);
        std::istringstream iss(line);
        std::string currentUserId;
        if (iss >> currentUserId && currentUserId == userId) {
            return {std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>()};
        }
    }
    return {};
}

// Calculate common movies and build relevant users
void RecommendCommand::calculateCommonMovies(
    const std::string& id,
    const std::unordered_set<std::string>& ourUserMovieSet,
    std::unordered_map<std::string, int>& commonMoviesMap,
    std::unordered_set<std::string>& relevantUsers
) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open the file: " + fileName);
    }

    std::string line;
    while (getline(file, line)) {
        line = cleanLine(line);
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string currentUserId;
        if (!(iss >> currentUserId) || currentUserId == id) continue;

        std::unordered_set<std::string> currentUserMovies{
            std::istream_iterator<std::string>(iss),
            std::istream_iterator<std::string>()
        };

        int commonMoviesCount = std::count_if(currentUserMovies.begin(), currentUserMovies.end(),
                                              [&ourUserMovieSet](const std::string& movie) {
                                                  return ourUserMovieSet.count(movie) > 0;
                                              });

        if (commonMoviesCount > 0) {
            commonMoviesMap[currentUserId] = commonMoviesCount;
            relevantUsers.insert(currentUserId);
        }
    }
}

// Calculate movie scores
std::unordered_map<std::string, int> RecommendCommand::calculateMovieScores(
    const std::unordered_set<std::string>& relevantUsers,
    const std::unordered_set<std::string>& ourUserMovieSet,
    const std::unordered_map<std::string, int>& commonMoviesMap,
    const std::string& movieId
) {
    std::unordered_map<std::string, int> movieScores;

    std::ifstream file(fileName);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open the file: " + fileName);
    }

    std::string line;
    while (getline(file, line)) {
        line = cleanLine(line);
        std::istringstream iss(line);
        std::string currentUserId;
        if (!(iss >> currentUserId) || !relevantUsers.count(currentUserId)) continue;

        std::unordered_set<std::string> currentUserMovies{
            std::istream_iterator<std::string>(iss),
            std::istream_iterator<std::string>()
        };

        if (currentUserMovies.count(movieId) == 0) continue;

        for (const std::string& movie : currentUserMovies) {
            if (ourUserMovieSet.count(movie) == 0 && movie != movieId) {
                movieScores[movie] += commonMoviesMap.at(currentUserId);
            }
        }
    }

    return movieScores;
}

// Print top movies
std::string RecommendCommand::printTopMovies(const std::unordered_map<std::string, int>& movieScores) {
    std::vector<std::pair<std::string, int>> sortedMovies(movieScores.begin(), movieScores.end());
    std::sort(sortedMovies.begin(), sortedMovies.end(), [](const auto& a, const auto& b) {
        return a.second != b.second ? a.second > b.second : a.first < b.first;
    });

    std::ostringstream output;
    for (size_t i = 0; i < std::min(sortedMovies.size(), size_t(10)); i++) {
        if (i > 0) output << " ";
        output << sortedMovies[i].first;
    }

    return output.str();
}

// Execute the recommendation command
std::string RecommendCommand::execute(const std::string& input) {
    try {
        std::vector<std::string> parsedInput = parseInput(input);
        if (parsedInput.size() != 2) {
            return "400 Bad Request";
        }

        std::string id = parsedInput[0];
        std::string movieId = parsedInput[1];

        std::vector<std::string> ourUserMovies = getUserMovies(id);
        if (ourUserMovies.empty()) {
            return "404 Not Found";
        }

        std::unordered_set<std::string> ourUserMovieSet(ourUserMovies.begin(), ourUserMovies.end());
        std::unordered_map<std::string, int> commonMoviesMap;
        std::unordered_set<std::string> relevantUsers;

        calculateCommonMovies(id, ourUserMovieSet, commonMoviesMap, relevantUsers);
        auto movieScores = calculateMovieScores(relevantUsers, ourUserMovieSet, commonMoviesMap, movieId);
        std::string recommendations = printTopMovies(movieScores);

        return "200 Ok\n\n" + recommendations;
    } catch (const std::exception& e) {
        return "500 Internal Server Error: " + std::string(e.what());
    }
}
