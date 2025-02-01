# Base image for Node.js
FROM node:16
# Copy all application files
COPY . .

# Set the working directory inside the container
WORKDIR /MVC

# Set Docker environment variable
ENV DOCKER=true

# Copy package files first for better caching
COPY package*.json ./

# Install dependencies
RUN npm install

# Copy the rest of the application files
COPY . .

# Expose the correct port
EXPOSE 8080

# Start the application
CMD ["npm", "start"]