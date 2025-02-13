#include "../../include/core/ServerClient.hpp"
#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

ServerClient::ServerClient() : _fd(-1)
{
}

ServerClient::~ServerClient()
{
    if (_fd != -1)
    {
        close(_fd);
    }
}

void ServerClient::connectx(const char *server_ip, int server_port)
{
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_fd == -1)
    {
        perror("Error: Socket Create.");
        exit(EXIT_FAILURE);
    }

    sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(server_ip);
    serverAddress.sin_port = htons(server_port);

    if (connect(_fd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
    {
        perror("Error: Connection Failed.");
        close(_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Connected to the server." << std::endl;
}

void ServerClient::sendx(const char *message)
{
    if (_fd != -1)
    {
        ssize_t sent = send(_fd, message, strlen(message), 0);
        if (sent == -1)
        {
            perror("Error: Sending Message.");
            close(_fd);
            exit(EXIT_FAILURE);
        }
        //std::cout << "Message sent: " << message << std::endl;
    }
}

std::string ServerClient::receivex()
{
    const int bufferSize = 1024;
    char buffer[bufferSize];
    std::string receivedMessage;

    ssize_t bytesRead;
    do
    {
        bytesRead = recv(_fd, buffer, bufferSize - 1, 0);

        if (bytesRead == -1)
        {
            perror("Error: Receiving Message.");
            close(_fd);
            exit(EXIT_FAILURE);
        }
        else if (bytesRead > 0)
        {
            buffer[bytesRead] = '\0'; // Assure la null-termination de la chaîne
            receivedMessage += buffer;
        }
    } while (bytesRead == bufferSize - 1 && receivedMessage.find('\n') == std::string::npos);

    //std::cout << "Received message: " << receivedMessage << std::endl;
    return receivedMessage;
}

void ServerClient::disconnectx()
{
    if (_fd != -1)
    {
        close(_fd);
        std::cout << "Disconnected from the server." << std::endl;
    }
}