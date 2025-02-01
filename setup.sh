#!/bin/bash

# Navigate to the frontend directory
cd web/frontend-web || exit

# Install dependencies for the frontend
npm install web-vitals
npm install react@18 react-dom@18
npm install react-router-dom axios jwt-decode
npm init -y
npm install react@^19.0.0 react-dom@^19.0.0 react-scripts@^5.0.1
npx browserslist --update-db
npm install @pmmmwh/react-refresh-webpack-plugin --save-dev
npm install webpack webpack-cli webpack-dev-server html-webpack-plugin babel-loader --save-dev

# Navigate back to the root directory
cd ../..

# Install backend dependencies
npm install express
npm install cors
npm install mongoose
npm install custom-env
npm install node
npm install multer
npm install jsonwebtoken

echo "All dependencies have been installed successfully!"
