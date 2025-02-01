#!/bin/bash

# Navigate to the MVC directory and start the backend
cd MVC || exit
echo "Starting backend with Node.js..."
node app.js & # Run the backend in the background

# Navigate back to the root directory
cd ..

# Compile the C++ code
echo "Compiling C++ project..."
g++ -std=c++17 -Isrc/include -pthread -o main \
    src/AddWatchedMoviesCommand.cpp \
    src/PatchMoviesCommand.cpp \
    src/DeleteCommand.cpp \
    src/UserManager.cpp \
    src/App.cpp \
    src/ConsoleMenu.cpp \
    src/RecommendCommand.cpp \
    src/HelpCommand.cpp \
    src/TCPserver.cpp \
    src/main.cpp

if [ $? -eq 0 ]; then
    echo "C++ compilation successful. Running the server..."
    ./main 9000 & # Run the compiled program in the background
else
    echo "C++ compilation failed. Exiting..."
    exit 1
fi

# Navigate to the frontend directory and start the frontend
cd web/frontend-web || exit
echo "Starting the frontend..."
npm start
