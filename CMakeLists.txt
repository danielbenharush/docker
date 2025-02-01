cmake_minimum_required(VERSION 3.10)

# Project name
project(MovieRecommendationSystem)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src/include)

# Add the source files
set(LIBRARY_SOURCES
    src/App.cpp
    src/ConsoleMenu.cpp
    src/AddWatchedMoviesCommand.cpp
    src/DeleteCommand.cpp
    src/PatchMoviesCommand.cpp
    src/RecommendCommand.cpp
    src/HelpCommand.cpp
    src/UserManager.cpp
    src/TCPserver.cpp
)

# Create a shared library for common logic
add_library(MovieRecommendationLibrary ${LIBRARY_SOURCES})

# the OLD create from EX_2
# Create the main executable
#add_executable(MovieRecommendationSystem src/main.cpp ${LIBRARY_SOURCES})
#target_link_libraries(MovieRecommendationSystem PRIVATE MovieRecommendationLibrary pthread)


# Create the main executable the NEW way to create the main executable
add_executable(TCPserver src/main.cpp ${LIBRARY_SOURCES})
target_link_libraries(TCPserver PRIVATE MovieRecommendationLibrary pthread)

# Define test sources
set(TEST_SOURCES
    tests/AddWatchedMoviesCommandTest.cpp
    tests/RecommendCommandTest.cpp
    tests/PatchMoviesCommandTest.cpp
    tests/ConsoleMenuTest.cpp
    tests/HelpCommandTest.cpp
    tests/DeleteCommandTest.cpp
)

# Create the test executable 
## add_executable(MovieRecommendationSystemTests ${TEST_SOURCES})
## target_link_libraries(MovieRecommendationSystemTests PRIVATE MovieRecommendationLibrary gtest gtest_main pthread)
