import socket
import os
import sys

def main():
    # Get server address and port from command-line arguments if provided
    if len(sys.argv) == 3:
        server_address = sys.argv[1]
        try:
            server_port = int(sys.argv[2])
            if server_port <= 0 or server_port > 65535:
                raise ValueError
        except ValueError:
            print("Invalid port number. Port must be an integer in the range 1-65535.")
            sys.exit(1)
    else:
        # Fallback to environment variables or defaults
        server_address = os.getenv('SERVER_ADDRESS', '127.0.0.1')  # Default to localhost
        server_port = int(os.getenv('SERVER_PORT', '8080'))        # Default to 8080

    # Create a TCP socket
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        # Connect to the server
        client_socket.connect((server_address, server_port))
        print(f"Connected to server at {server_address}:{server_port}")

        while True:
            # Get user input
            message = input("")

            # Send the message to the server
            client_socket.sendall(message.encode())

            # Receive the server's response
            response = client_socket.recv(1024).decode()
            print(response)

    except ConnectionError as e:
        print(f"Connection error: {e}")

    finally:
        # Close the socket
        client_socket.close()
        print("Connection closed.")

if __name__ == "__main__":
    main()
