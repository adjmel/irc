#include "../../../include/core/ServerCommand.hpp"
#include "../../../include/core/Server.hpp"
#include "../../../include/core/Client.hpp"
#include "../../../include/core/Channel.hpp"
#include "../../../include/core/Utils.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstdlib>

bool ServerCommand::commandIsValidFileTransfert(std::string &command)
{
    std::vector<std::string> args = ServerCommand::commandArgsGet(command);
    if (args.size() == 8)
        std::cout << Utils::trim(args[0]) 
            << " " << Utils::trim(args[2])
            << " " << Utils::trim(args[3])
            << std::endl; 
    if (args.empty() 
        || args.size() != 8
        || !this->commandIsValidName(Utils::trim(args[0]), "PRIVMSG"))
        return false;
    return true;
}

void ServerCommand::commandRunFileTransfertReceive(const char *filename, in_addr_t clientIp, int clientPort, std::streamsize fileSize) 
{
    int serverSocket, clientSocket;
    sockaddr_in serverAddr, clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
    (void) clientIp;

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) 
    {
        std::cerr << "Error: Could not create socket." << std::endl;
        return;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(clientPort);  // Utiliser le port spécifié par le client

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error: Could not bind to port." << std::endl;
        close(serverSocket);
        return;
    }

    if (listen(serverSocket, 1) == -1) {
        std::cerr << "Error: Could not listen on port." << std::endl;
        close(serverSocket);
        return;
    }

    std::cout << "Server listening on port " << clientPort << "..." << std::endl;

    // Attente de la connexion du client
    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientLen);
    if (clientSocket == -1) {
        std::cerr << "Error: Could not accept connection." << std::endl;
        close(serverSocket);
        return;
    }

    std::cout << "Client connected." << std::endl;

    // Réception du fichier
    std::ofstream file(filename, std::ios::binary);
    char buffer[1024];
    std::streamsize bytesRead = 0;

    while (bytesRead < fileSize) {
        ssize_t receivedBytes = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (receivedBytes <= 0) {
            std::cerr << "Error: Failed to receive file data." << std::endl;
            break;
        }
        file.write(buffer, receivedBytes);
        bytesRead += receivedBytes;
    }

    file.close();
    close(clientSocket);
    close(serverSocket);

    std::cout << "File received successfully." << std::endl;
}


void ServerCommand::commandRunFileTransfertSend(const char *filename, const char *clientIp, int clientPort) 
{
    int serverSocket, clientSocket;
    sockaddr_in serverAddr, clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
    (void) clientIp;

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error: Could not create socket." << std::endl;
        return;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(clientPort);

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error: Could not bind to port." << std::endl;
        close(serverSocket);
        return;
    }

    if (listen(serverSocket, 1) == -1) {
        std::cerr << "Error: Could not listen on port." << std::endl;
        close(serverSocket);
        return;
    }

    std::cout << "Server listening on port " << clientPort << "..." << std::endl;

    // Attente de la connexion du client
    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientLen);
    if (clientSocket == -1) {
        std::cerr << "Error: Could not accept connection." << std::endl;
        close(serverSocket);
        return;
    }

    std::cout << "Client connected." << std::endl;

    // Lecture du fichier
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file for sending." << std::endl;
        close(clientSocket);
        close(serverSocket);
        return;
    }

    char buffer[1024];
    std::streamsize bytesRead;

    while (!file.eof()) {
        file.read(buffer, sizeof(buffer));
        bytesRead = file.gcount();

        if (bytesRead <= 0) {
            std::cerr << "Error: Failed to read file data." << std::endl;
            break;
        }

        ssize_t sentBytes = send(clientSocket, buffer, bytesRead, 0);
        if (sentBytes <= 0) {
            std::cerr << "Error: Failed to send file data." << std::endl;
            break;
        }
    }

    file.close();
    close(clientSocket);
    close(serverSocket);

    std::cout << "File sent successfully." << std::endl;
}

void ServerCommand::commandRunFileTransfert(Client &cli)
{
    std::string message = cli.messageGet();
    std::vector<std::string> args = ServerCommand::commandArgsGet(message);
    std::cout << "-- FILE_TRANSFERT " << std::endl;

    return ;

    if (this->commandIsValidFileTransfert(message))
    {
        // std::string nick = Utils::trim(args[1]);
        // std::string filename = Utils::trim(args[4]);
        // std::string ipStr = Utils::trim(args[5]);
        // std::string portStr = Utils::trim(args[6]);
        // std::string sizeStr = Utils::trim(args[7]);

        // // Convertir les chaînes en valeurs numériques
        // unsigned long ip = strtoul(ipStr.c_str(), NULL, 10);
        // int port = atoi(portStr.c_str());
        // std::streamsize size = static_cast<std::streamsize>(atol(sizeStr.c_str())) * 1024; // Convertir en octets

        // // Chemin complet du fichier de destination
        // std::string destinationPath = "./storage/" + nick + "-" + filename;

        // this->commandRunFileTransfertReceive(destinationPath.c_str(), ip, port, size);
         
         // this->_server->sendx(cli.fdGet(), "ACCEPT\n");

        // Supposons que vous avez obtenu ces informations au préalable
        const char* serverIpAddress = "0.0.0.0";
        std::string filename = "http://" + std::string(serverIpAddress) + ":8081/mnt/nfs/homes/fsundare/sgoinfre/doc.txt";
        const char* clientIp = "0.0.0.0"; /* Adresse IP du client */
        int clientPort = 1234;             /* Port d'écoute du client */

        std::cout << " --- " << filename << std::endl;
        this->commandRunFileTransfertSend(filename.c_str(), clientIp, clientPort);


        

        // // Envoyer un message avec le lien du fichier local sur un canal
        // const char* channel = "#exemple";
        // message = "PRIVMSG #exemple :Le fichier est disponible ici : http://" + std::string(serverIpAddress) + ":8081/mnt/nfs/homes/fsundare/sgoinfre/doc.txt\r\n";

        // //this->_server->sendx(cli.fdGet(), message.c_str());
       
    }
}