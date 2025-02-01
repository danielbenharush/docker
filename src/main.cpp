#include <map>
#include <string>
#include <iostream>
#include "App.hpp"
#include "UserManager.hpp"
#include "AddWatchedMoviesCommand.hpp"
#include "RecommendCommand.hpp"
#include "PatchMoviesCommand.hpp"
#include "DeleteCommand.hpp"
#include "HelpCommand.hpp"
#include "ConsoleMenu.hpp"
#include "TCPserver.hpp"

int main(int argc, char* argv[]) {
    try {

         if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    int port = std::stoi(argv[1]);
    if (port <= 0 || port > 65535) {
        std::cerr << "Invalid port number. Port must be in range 1-65535." << std::endl;
        return 1;
    }

        // Initialize UserManager with the data file path
        UserManager userManager("data/watched_movies.txt");

        // Create the menu
        IMenu* menu = new ConsoleMenu();

        // Create the commands map
        std::map<std::string, ICommand*> commands;

        // Add commands
        commands["post"] = new AddWatchedMoviesCommand(&userManager);
        commands["patch"] = new PatchMoviesCommand(&userManager);
        commands["delete"] = new DeleteCommand(&userManager);
        commands["get"] = new RecommendCommand("data/watched_movies.txt");
        commands["delete"] = new DeleteCommand(&userManager);

        // Add the help command, passing the menu
        commands["help"] = new HelpCommand(menu);

        // Create the application and pass the commands and UserManager
        App app(menu, commands, &userManager);

        // Start the server
        TCPserver server(&app, port, 1000);
        server.start();

        // Cleanup commands
        for (auto& cmd : commands) {
            delete cmd.second;
        }

        // Cleanup menu
        delete menu;

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error in main: " << e.what() << std::endl;
        return 1;
    }
}
