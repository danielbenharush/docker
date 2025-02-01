#!/bin/bash

# Set the base URL
BASE_URL="http://localhost:8080/api"

# Directories containing images and videos
PICTURE_DIR="./pictures"
VIDEOS_DIR="./videos"

# Validate dependencies
if ! command -v jq &> /dev/null; then
  echo "Error: 'jq' is not installed. Please install it and try again."
  exit 1
fi

# Validate directories
if [ ! -d "$PICTURE_DIR" ]; then
  echo "Error: Picture directory '$PICTURE_DIR' does not exist."
  exit 1
fi

if [ ! -d "$VIDEOS_DIR" ]; then
  echo "Error: Video directory '$VIDEOS_DIR' does not exist."
  exit 1
fi

# Get a list of all images and videos in the directories
PICTURES=($(ls "$PICTURE_DIR" | grep -E '\.(jpg|jpeg|png)$'))
VIDEOS=($(ls "$VIDEOS_DIR" | grep -E '\.(mp4|mkv|avi)$'))

# Validate that files exist
if [ ${#PICTURES[@]} -eq 0 ]; then
  echo "Error: No pictures found in '$PICTURE_DIR'."
  exit 1
fi

if [ ${#VIDEOS[@]} -eq 0 ]; then
  echo "Error: No videos found in '$VIDEOS_DIR'."
  exit 1
fi

# Create the manager user
# Create the manager user using multipart/form-data
manager_picture="${PICTURES[$RANDOM % ${#PICTURES[@]}]}"
response=$(curl -s -X POST "$BASE_URL/users" \
  -H "Content-Type: multipart/form-data" \
  -F "name=AdminManager" \
  -F "mail=admin@example.com" \
  -F "password=SecurePass123" \
  -F "phone=1234567890" \
  -F "role=manager" \
  -F "profilePicture=@$PICTURE_DIR/$manager_picture")

userID=$(echo "$response" | jq -r '._id')
if [ "$userID" != "null" ]; then
  echo -e "\nManager user created with ID: $userID and profile picture '$manager_picture'."
else
  echo -e "\nFailed to create manager user. Response: $response"
fi


# Define categories
categories=("Action" "Comedy" "Drama" "Horror" "Romance" "Science" "Animated")

# Create categories
for category in "${categories[@]}"; do
  response=$(curl -s -X POST "$BASE_URL/categories" \
    -H "Content-Type: application/json" \
    -d "{
          \"name\": \"$category\",
          \"promoted\": true
        }")
  echo -e "\nCategory '$category' created."
done

# Define movies for each category
declare -A movies
movies["Action"]="Die Hard|Mad Max: Fury Road|The Dark Knight|John Wick|Gladiator|Avengers: Endgame|Inception|The Matrix|Logan|Terminator 2"
movies["Comedy"]="The Hangover|Superbad|Step Brothers|Bridesmaids|Groundhog Day|Dumb and Dumber|Anchorman|Tropic Thunder|Elf|Deadpool"
movies["Drama"]="The Shawshank Redemption|Forrest Gump|The Godfather|Fight Club|The Social Network|A Beautiful Mind|Parasite|The Pursuit of Happyness|12 Angry Men|Joker"
movies["Horror"]="The Conjuring|It|A Nightmare on Elm Street|The Exorcist|Get Out|Hereditary|Scream|The Shining|The Ring|Us"
movies["Romance"]="The Notebook|Pride and Prejudice|Titanic|La La Land|Crazy Rich Asians|A Star is Born|Pretty Woman|Notting Hill|500 Days of Summer|Sleepless in Seattle"
movies["Science"]="Star Wars: A New Hope|Interstellar|Blade Runner 2049|The Martian|Jurassic Park|Arrival|E.T.|Dune|Ex Machina|Gravity"
movies["Animated"]="Toy Story|Frozen|The Lion King|Finding Nemo|Shrek|Up|The Incredibles|Coco|Moana|Zootopia"

# Create movies
for category in "${categories[@]}"; do
  IFS='|' read -r -a movieArray <<< "${movies[$category]}"
  for movie in "${movieArray[@]}"; do
    picture="${PICTURES[$RANDOM % ${#PICTURES[@]}]}"
    video="${VIDEOS[$RANDOM % ${#VIDEOS[@]}]}"
    response=$(curl -s -X POST "$BASE_URL/movies" \
      -H "Content-Type: multipart/form-data" \
      -F "title=$movie" \
      -F "category=$category" \
      -F "cast[]=Actor A" \
      -F "cast[]=Actor B" \
      -F "description=$movie is a popular $category movie loved by many." \
      -F "time=120 minutes" \
      -F "year=2021" \
      -F "subtitlesAvailable=English, Hebrew" \
      -F "video=@$VIDEOS_DIR/$video" \
      -F "moviePicture=@$PICTURE_DIR/$picture")
    movieID=$(echo "$response" | jq -r '._id')
    if [ "$movieID" != "null" ]; then
      echo -e "\nMovie '$movie' in category '$category' created with ID: $movieID."
    else
      echo -e "\nFailed to create movie '$movie' in category '$category'. Response: $response"
    fi
  done
done

echo -e "\nData population completed."
