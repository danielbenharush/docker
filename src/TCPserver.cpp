#include <map>
#include <string>
#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <condition_variable>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <mutex>
#include "App.hpp"
#include "UserManager.hpp"
#include "AddWatchedMoviesCommand.hpp"
#include "RecommendCommand.hpp"
#include "PatchMoviesCommand.hpp"
#include "DeleteCommand.hpp"
#include "HelpCommand.hpp"
#include "ConsoleMenu.hpp"

std::mutex appMutex;
std::mutex queueMutex;
std::condition_variable condition;
std::queue<int> clientQueue;
bool stopThreads = false;

class TCPserver {
public:
    TCPserver(App* appInstance, int port, size_t threadCount);
    ~TCPserver();
    void start();

private:
    int serverSocket;
    int port;
    struct sockaddr_in serverAddress;
    App* app;
    std::vector<std::thread> threads;

    void acceptClients();
    void workerThread();
};

TCPserver::TCPserver(App* appInstance, int port, size_t threadCount) : serverSocket(0), app(appInstance), port(port) {
    memset(&serverAddress, 0, sizeof(serverAddress));

    // Initialize thread pool
    for (size_t i = 0; i < threadCount; ++i) {
        threads.emplace_back(&TCPserver::workerThread, this);
    }
}

TCPserver::~TCPserver() {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        stopThreads = true;
    }
    condition.notify_all();

    for (std::thread& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    close(serverSocket);
}

void TCPserver::start() {
    // Create server socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Set server address
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    // Bind socket to port
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Bind failed");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(serverSocket, 100) < 0) {
        perror("Listen failed");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is running on port " << port << std::endl;

    acceptClients();
}

void TCPserver::acceptClients() {
    while (true) {
        struct sockaddr_in clientAddress;
        socklen_t clientLen = sizeof(clientAddress);

        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientLen);
        if (clientSocket < 0) {
            perror("Accept failed");
            continue;
        }

        {
            std::lock_guard<std::mutex> lock(queueMutex);
            clientQueue.push(clientSocket);
        }
        condition.notify_one();
    }
}

void TCPserver::workerThread() {
    while (true) {
        int clientSocket;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [] { return !clientQueue.empty() || stopThreads; });

            if (stopThreads && clientQueue.empty()) {
                return;
            }

            clientSocket = clientQueue.front();
            clientQueue.pop();
        }

        char buffer[1024];
        while (true) {
            memset(buffer, 0, sizeof(buffer));
            int bytesRead = read(clientSocket, buffer, sizeof(buffer));
            if (bytesRead <= 0) {
                close(clientSocket);
                break;
            }

            std::string clientMessage(buffer);
            std::string response;

            // Secure the critical section with a mutex
            {
                std::lock_guard<std::mutex> lock(appMutex);
                if (app) {
                    response = app->processCommand(clientMessage);
                } else {
                    response = "Server error: No application available to process command.";
                }
            }

            send(clientSocket, response.c_str(), response.size(), 0);
        }
    }
}
