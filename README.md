# Netflix Project 
# Netflix Recommendation System

## Overview
The **Netflix Recommendation System** is a distributed system consisting of three main components:
1. A Node.js REST API server built with MVC architecture for handling user interactions and movie management
2. A C++ recommendation engine that analyzes user watching patterns and provides personalized movie recommendations
3. A MongoDB database for storing user data, movies, and categories

The system is containerized using Docker and follows SOLID principles and Object-Oriented Programming (OOP) practices.

---

## Project Structure

This project is divided into two main parts:

1. MVC Architecture (Node.js Server): The backend server that handles API requests, business logic, and data management.
2. Recommendation System: The service that the backend communicates with to provide movie recommendations.

### Below is the detailed folder structure:

```
├── .vscode/               # VS Code workspace settings
├── data/                 # Data files (e.g., watched movies list)
├── MVC/                  # Node.js API Server
├── src/                  # C++ Recommendation System source files
├── tests/                # Test suites
├── .gitignore           # Git ignore rules
├── CMakeLists.txt       # CMake configuration file
├── docker-compose.yml   # Docker services configuration
├── Dockerfile.cpp       # Dockerfile for C++ server
├── README.md           # Project documentation
└── run_tests.sh        # Test execution script
```


### Detailed Sub-Structures

#### MVC (Node.js Server)

```
MVC/
├── config/          # Environment configurations
├── controllers/     # Route controllers
├── models/         # Database models
├── node_modules/   # Node.js dependencies
├── routes/         # API route definitions
├── services/       # Business logic services
├── app.js          # Main entry point
├── Dockerfile.js   # Dockerfile for Node.js server
├── package.json    # Node.js dependencies and scripts
└── package-lock.json # Locked versions of dependencies
```

#### Recommendation System (src)

```
src/
├── include/                    # Header files directory
├── AddWatchedMovies.cpp       # Add movies to user's watched list
├── App.cpp                    # Main application logic
├── ConsoleMenu.cpp            # Console UI management
├── DeleteCommand.cpp          # Delete movie command implementation
├── HelpCommand.cpp            # Help command implementation
├── main.cpp                   # Program entry point
├── PatchMoviesCommand.cpp     # Update movie command implementation
├── RecommendCommand.cpp       # Movie recommendation logic
├── TCPclient.py              # Python TCP client for testing
├── TCPserver.cpp             # TCP server implementation
└── UserManager.cpp            # User management functionality
```

---

## Requirements
- Node.js (v16.x or higher)
- g++ (v9 or higher with C++17 support)
- Docker (v20.x or higher)
- Docker Compose (v2.x)
- MongoDB (v4.x or higher)

---

## How to Build and Run

Since the `config` folder is excluded from version control (as specified in `.gitignore`), you must manually create it before running the application. Follow the steps below:

1. Navigate to the `MVC` directory:
    ```bash
    cd MVC
    ```

2. Create a new folder named `config`:
    ```bash
    mkdir config
    ```

3. Inside the `config` folder, create the following two files:

   - `.env.local`:
     ```env
     CONNECTION_STRING=mongodb://127.0.0.1:27017/hello
     PORT=8080
     NODE_ENV=local
     ```

   - `.env.docker`:
     ```env
     CONNECTION_STRING=mongodb://mongodb:27017/hello
     PORT=8080
     NODE_ENV=docker
     CPP_SERVICE_HOST=cpp-server
     ```

   Ensure the environment variables match your setup.

4. The `config` folder should now look like this:
   ```plaintext
   MVC/
   ├── config/
   │   ├── .env.local
   │   ├── .env.docker  
  
If you finished do `cd ..` for going back to the main directory.


### Option 1: Run Locally
##### Make the Scripts Executable
Before running the project, ensure the scripts have executable permissions:  
```bash
    chmod +x setup.sh  
    chmod +x run_project.sh  
    chmod +x populate_real_data.sh  
```
  
#### setup  
Download and install all the required dependencies and configure the environment by running:  
```bash
    ./setup.sh  
```  
This will:  
1. Install backend dependencies for the NodeJS server.  
2. Install frontend dependencies for the React application.  
3. Ensure the configuration files are in place.  
  
#### Run the system  
To start the entire system, including the backend, frontend, and recommendation service:  
```bash
    ./run_project.sh
```  
This script will:  
1. Start the NodeJS server locally.  
2. Compile and run the C++ recommendation system.  
3. Launch the React frontend application.
   
#### Populate the Database with Sample Data  
After the system is running, you can populate it with sample users, categories, and movies:  
```bash
    ./populate_real_data.sh 
```    
This script will:  
1. Create a manager user with a profile picture. (username : AdminManager , password : SecurePass123)
2. Add predefined categories.
3. Populate movies in each category using random images and videos from the specified directories.
  
### Option 2: Run with Docker 
```bash
docker-compose up --build
```
To view logs in real-time: If you want to monitor the logs of specific services while they are running, use the following commands:

For the Node.js server:
```bash
docker-compose logs -f nodejs-server
```

For the C++ server:
```bash
docker-compose logs -f cpp-server
```

For MongoDB:
```bash
docker-compose logs -f mongodb
```
---

## API Documentation - MVC Server

1. Base URL  
2. All API endpoints are prefixed with /api  
3. For example: http://localhost:8080/api/movies  

### Users API (/api/users)  

#### POST /api/users  
Description: Create a new user.  
Body:  
```bash
{
  "name": "string",
  "mail": "string",
  "password": "string",
  "phone": "string",
  "profilePicture": "string"
}
```
Example:
```bash
curl -X POST http://localhost:8080/api/users \
-H "Content-Type: application/json" \
-d '{
  "name": "testuser",
  "mail": "test@test.com",
  "password": "testpass",
  "phone": "1234567890"
}'
```

#### GET /api/users/:id  
Description: Get details of a specific user by their ID.  
Parameters: id (string): The ID of the user.  
Response: Returns the user details.  

Example:
```bash
curl -X GET http://localhost:8080/api/users/123456789
```

### Authentication API (/api/tokens)

#### POST /api/tokens  
Description: Authenticate a user and generate a token.  
Body:  
```bash
{
  "name": "string",
  "password": "string"
}
```
Example:
```bash
curl -X POST http://localhost:8080/api/tokens \
-H "Content-Type: application/json" \
-d '{
  "name": "testuser",
  "password": "testpass"
}'
```

### Categories API (/api/categories)  

#### GET /api/categories  
Description: Get a list of all categories.  
Response: Returns a list of categories.  

Example:
```bash
curl -X GET http://localhost:8080/api/categories
```

#### POST /api/categories  
Description: Create a new category.  
Body:  
```bash
{
  "name": "string",
  "promoted": "boolean"
}
```
Example:
```bash
curl -X POST http://localhost:8080/api/categories \
-H "Content-Type: application/json" \
-d '{
  "name": "Action",
  "promoted": true
}'
```

#### GET /api/categories/:id  
Description: Get details of a specific category by its ID.  
Parameters: id (string): The ID of the category.  
Response: Returns the category details.  

Example:
```bash
curl -X GET http://localhost:8080/api/categories/123456789
```

#### PATCH /api/categories/:id  
Description: Update a category by its ID.  
Parameters: id (string): The ID of the category.  
Body:
```bash
{
  "name": "string",
  "promoted": "boolean"
}
```
Example:
```bash
curl -X PATCH http://localhost:8080/api/categories/123456789 \
-H "Content-Type: application/json" \
-d '{
  "name": "Action Movies",
  "promoted": true
}'
```

#### DELETE /api/categories/:id  
Description: Delete a category by its ID.  
Parameters: id (string): The ID of the category.  
Response: Returns a status indicating the deletion.  

Example:
```bash
curl -X DELETE http://localhost:8080/api/categories/123456789
```

### Movies API (/api/movies)  

#### GET /api/movies/search/:query  
Description: Search for movies by query.  
Parameters: query (string): The search term.  
Response: Returns a list of movies matching the query.  

Example:
```bash
curl -X GET http://localhost:8080/api/movies/search/Matrix \
-H "id_name: testuser"
```

#### GET /api/movies/:id  
Description: Get details of a specific movie by its ID.  
Parameters: id (string): The ID of the movie.  
Response: Returns the movie details.  

Example:
```bash
curl -X GET http://localhost:8080/api/movies/123456789
```

#### GET /api/movies  
Description: Get a list of all movies or movies filtered by categories.  
Parameters: Optional filters for categories.  
Response: Returns a list of movies.  

Example:
```bash
curl -X GET http://localhost:8080/api/movies \
-H "id_name: testuser"
```

#### POST /api/movies  
Description: Create a new movie.  
Body:  
```bash
{
  "title": "string",
  "category": "string",
  "cast": ["string"],
  "description": "string",
  "time": "string",
  "year": "number",
  "subtitlesAvailable": "string"
}
```
Example:
```bash
curl -X POST http://localhost:8080/api/movies \
-H "Content-Type: application/json" \
-H "id_name: testuser" \
-d '{
  "title": "The Matrix",
  "category": "Action",
  "cast": ["Keanu Reeves", "Laurence Fishburne"],
  "description": "A computer programmer discovers a mysterious world",
  "time": "2h 16m",
  "year": 1999,
  "subtitlesAvailable": "Hebrew"
}'
```

#### PUT /api/movies/:id  
Description: Replace the details of a specific movie by its ID.  
Parameters: id (string): The ID of the movie.  
Body:  
```bash
{
  "title": "string",
  "category": "string",
  "cast": ["string"],
  "description": "string",
  "time": "string",
  "year": "number",
  "subtitlesAvailable": "string"
}
```
Example:
```bash
curl -X PUT http://localhost:8080/api/movies/123456789 \
-H "Content-Type: application/json" \
-H "id_name: testuser" \
-d '{
  "title": "The Matrix Reloaded",
  "category": "Action",
  "cast": ["Keanu Reeves", "Laurence Fishburne"],
  "description": "The Matrix sequel",
  "time": "2h 30m",
  "year": 2003,
  "subtitlesAvailable": "Hebrew"
}'
```

#### DELETE /api/movies/:id  
Description: Delete a movie by its ID.  
Parameters: id (string): The ID of the movie.  
Response: Returns a status indicating the deletion.  

Example:
```bash
curl -X DELETE http://localhost:8080/api/movies/123456789 \
-H "id_name: testuser"
```

#### GET /api/movies/:id/recommend  
Description: Get recommended movies for a specific movie for specific user.  
Parameters: id (string): The ID of the movie.  
Headers: id_name (string): The ID of the user.  
Response: Returns a list of recommended movies.  

Example:
```bash
curl -X GET http://localhost:8080/api/movies/123456789/recommend \
-H "id_name: testuser"
```

#### POST /api/movies/:id/recommend  
Description: Add a movie to the user's recommendation list.  
Parameters: id (string): The ID of the movie.  
Headers: id_name (string): The ID of the user.  
Response: Returns a status indicating the addition.  

Example:
```bash
curl -X POST http://localhost:8080/api/movies/123456789/recommend \
-H "id_name: testuser"
```

### Important Notes:
1. Replace `123456789` in the examples with actual IDs from your database.
2. The `id_name` header is required for most movie-related operations.
3. All POST and PUT requests require the `Content-Type: application/json` header.
  
### Status Codes
Each endpoint returns appropriate HTTP status codes:  
  
200 OK: The request was successful.  
201 Created: A resource was successfully created.  
204 No Content: A resource was successfully deleted.  
400 Bad Request: The request was invalid.  
404 Not Found: The resource was not found.  
  
---

## Version Info 

### Languages and Core Technologies
* **API Server**: Node.js (v16.x) with Express
* **Recommendation Engine**: C++ (C++17 standard)
* **Database**: MongoDB (v4.x)
* **Container Platform**: Docker & Docker Compose

### Node.js Dependencies
* Express (v4.21.2)
* Mongoose (v6.10.0)
* CORS (v2.8.5)
* Custom-env (v2.0.6)

### C++ Dependencies
* Standard Template Library (STL)
* Networking libraries for TCP/IP communication
* CMake (build system)

### Development Tools
* **Version Control**: Git
* **Test Framework**: Google Test (GTest)
* **Container Platform**: Docker & Docker Compose
* **Database Tools**: MongoDB Compass (GUI)

### Ports Used
* Node.js Server: 8080
* C++ Recommendation Server: 9000
* MongoDB: 27017

### Supported Platforms
* Linux
* Windows (via WSL2)
* macOS
