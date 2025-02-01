#ifndef TCPSERVER_HPP
#define TCPSERVER_HPP

#include "App.hpp"
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <netinet/in.h>

class TCPserver {
private:
    int serverSocket;
    struct sockaddr_in serverAddress;
    std::vector<std::thread> threads;
    std::queue<int> clientQueue;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stopThreads;
    App* app;

    void handleClient(int clientSocket);
    void workerThread();

public:
    TCPserver(App* appInstance, int port, size_t threadCount);
    ~TCPserver();

    void start();

private:
    void acceptClients();
};

#endif // TCPSERVER_HPP
