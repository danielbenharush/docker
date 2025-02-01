# Use a base image with C++ pre-installed
FROM ubuntu:20.04

# Set environment variables
ENV DEBIAN_FRONTEND=noninteractive

# Install required dependencies
RUN apt-get update && apt-get install -y \
    g++ \
    cmake \
    make \
    python3 \
    python3-pip \
    git \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy the project files
COPY . /app

# Clean build directory if exists and create a new build
RUN rm -rf /app/build && mkdir /app/build

# Build the server and main project
RUN cmake -S . -B /app/build
RUN cmake --build /app/build

# Expose correct server port
EXPOSE 9000

# Default command with correct port
CMD ["./build/TCPserver", "9000"]