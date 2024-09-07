#include <iostream>
#include <string>
#include <unistd.h> // For close() function
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // For inet_ntoa() function

int main() {
    // Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error creating socket." << std::endl;
        return 1;
    }

    // Bind the socket to an address and port
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Listen on any available interface
    serverAddr.sin_port = htons(8080); // Example port

    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Error binding socket." << std::endl;
        close(sockfd);
        return 1;
    }

    // Listen for incoming connections
    if (listen(sockfd, 5) < 0) { // Allow up to 5 pending connections
        std::cerr << "Error listening on socket." << std::endl;
        close(sockfd);
        return 1;
    }

    std::cout << "Server listening on port 8080..." << std::endl;

    // Accept incoming connections
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    int clientSocket = accept(sockfd, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (clientSocket < 0) {
        std::cerr << "Error accepting connection." << std::endl;
        close(sockfd);
        return 1;
    }

    std::cout << "Connection accepted from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;

    // Send some data to the client
    std::string message = "Hello from server!";
    send(clientSocket, message.c_str(), message.length(), 0);

    // Close the client socket
    close(clientSocket);

    // Close the server socket
    close(sockfd);

    return 0;
}
