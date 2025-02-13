#include "../../include/core/Server.hpp"
#include "../../include/core/ServerConfig.hpp"
#include "../../include/core/ServerCommand.hpp"
#include "../../include/core/Client.hpp"
#include "../../include/core/Channel.hpp"
#include "../../include/core/Log.hpp"
#include "../../include/core/Bot.hpp"
#include "../../include/core/Utils.hpp"
#include "../../include/core/numeric_response.hpp"
#include <iostream>
#include <unistd.h>
#include <cstdio>  
#include <cstdlib>
#include <cstddef>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>
#include <csignal>
#include <vector>
#include <fcntl.h>
#include <algorithm>
#include <ctime>
#include <string>
#include <sstream>

Server* Server::instance = NULL;

Server::Server() 
{

}

Server::Server(std::string address, int port,
    std::string password) :
    _status(0), _address(address), _port(port), _password(password)
{
    Server::instance = this;
    ServerConfig serverConfig(this);
    this->_server_command = new ServerCommand(this);

    this->_log = new Log("server.", "logs/");
    this->_bot = new Bot("assets/bot.txt");

    this->setup();
    this->setupSignals();    
}

Server::~Server() 
{
    this->stop();

    for (std::vector<Admin*>::iterator it = this->_admins.begin(); it != this->_admins.end(); ++it) 
    {
        delete *it;
        *it = NULL;
    }

    for (std::vector<Client*>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it) 
    {
        if (close((*it)->fdGet()) == -1) 
        {
            perror("Erreur lors de la fermeture du socket");
        }
        delete *it;
        *it = NULL;
    }

    for (std::vector<Channel*>::iterator it = this->_channels.begin(); it != this->_channels.end(); ++it) 
    {
        delete *it;
        *it = NULL;
    }

    delete this->_server_command;
    delete this->_log;
    delete this->_bot;
    //std::cout << "SESTROYED ... " << std::endl;
}

void Server::setup()
{
    this->_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_fd == -1)
    {
        perror("Error: Socket Create.");
        this->_status = -1;
        return ;
    }

    if (fcntl(this->_fd, F_SETFL, O_NONBLOCK) == -1) 
    {
        std::cerr << "Erreur lors de la mise en mode non bloquant de la socket" << std::endl;
        close(this->_fd);
        this->_status = -1;
        return ;
    }

    memset(&this->_server_address, 0, sizeof(this->_server_address)); 
    this->_server_address.sin_family = AF_INET;
    this->_server_address.sin_addr.s_addr = inet_addr(this->_address.c_str());
    this->_server_address.sin_port = htons(this->_port);
}

void Server::bindx()
{
    if (this->_status == -1)
        return ;
    if (bind(this->_fd, (struct sockaddr*)&(this->_server_address), 
        sizeof(this->_server_address)) == -1) 
    {
        perror("Erreur lors de la liaison du socket");
        close(this->_fd);
        this->_status = -1;
        return ;
    }
}

void Server::listenx()
{
    if (this->_status == -1)
        return ;
    if (listen(this->_fd, SOMAXCONN) == -1) 
    {
        perror("Erreur lors de l'écoute du socket");
        close(this->_fd);
        this->_status = -1;
        return ;
    }
}

void Server::pollx()
{
    std::cout << "Serveur en attente de connexions..." << std::endl;
    
    if (this->_status == -1)
        return ;
    this->_status = 1;
    while (this->_status) 
    {
        std::vector<struct pollfd> pollDescriptors;

        // WATCH SERVER
        struct pollfd mainPollDescriptor;
        mainPollDescriptor.fd = _fd;
        mainPollDescriptor.events = POLLIN;
        pollDescriptors.push_back(mainPollDescriptor);

        // WATCH CLIENTS
        for (std::vector<Client*>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it) 
        {
            int clientSocket = (*it)->fdGet();
            struct pollfd clientPollDescriptor;
            clientPollDescriptor.fd = clientSocket;
            clientPollDescriptor.events = POLLIN | POLLHUP | POLLERR; 
            pollDescriptors.push_back(clientPollDescriptor);
        }

        // WAIT EVENT
        int ready = poll(pollDescriptors.data(), pollDescriptors.size(), -1);
        if (ready == -1) 
        {
            perror("Erreur lors de l'appel à poll");
            this->_status = -1;
            break;
        }

        if (ready > 0) 
        {   
            this->pollxServer(pollDescriptors);
            this->pollxClients(pollDescriptors);
        }

        if (this->_status == 0)
            break;
    }
}

void Server::pollxServer(std::vector<struct pollfd>& pollDescriptors)
{
    std::cout << "POLL SERVER" << std::endl; 

    if (pollDescriptors[0].revents & POLLIN) 
    {
        sockaddr_in clientAddress;
        socklen_t clientAddressLength = sizeof(clientAddress);
        int clientSocket = accept(this->_fd, (struct sockaddr*)&clientAddress, &clientAddressLength);
        if (clientSocket == -1) 
        {
            perror("Erreur lors de l'acceptation de la connexion");
            close(this->_fd);
            this->_status = 0;
            return ;
        }

        if (!this->clientsIsExist(clientSocket)) 
        {
            Client *c = new Client(clientSocket);
            this->_clients.push_back(c);

            char clientIP[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(clientAddress.sin_addr), clientIP, INET_ADDRSTRLEN);
            int clientPort = ntohs(clientAddress.sin_port);

            std::string message = "New client #" + Utils::intToString(c->idGet()) + " from " + clientIP + ":" + Utils::intToString(clientPort);
            std::cout << message << std::endl;
            this->_log->append(message, 0);     

            //std::string welcomeMessage = "001 " + Utils::intToString(c->idGet()) + " :Welcome to the Internet Relay Chat Network " + c->nickGet();
            //this->sendx(clientSocket, welcomeMessage.c_str());
            // std::string capAckMessage = "CAP * ACK :";
            // this->sendx(clientSocket, capAckMessage.c_str());       
        }
    }
}

void Server::pollxClients(std::vector<struct pollfd>& pollDescriptors)
{
    std::cout << "POLL CLIENTS" << std::endl; 

    for (size_t i = 1; i < pollDescriptors.size(); ++i) 
    {
        Client *c = this->clientsFind(pollDescriptors[i].fd);

        if (pollDescriptors[i].revents & POLLIN) 
        {
            std::string message = this->recvx(pollDescriptors[i].fd);

            if (message.empty())
            {
                std::cout << "Client disconnected: " << c->nickGet() << std::endl;
                this->clientsRemove(c->idGet());
                continue;
            }

            if (message.find_first_not_of("\n") == std::string::npos)
                continue;
          
            if(this->_server_command->commandIsPartial(message))
            {
                message = Utils::strClean(message);
                c->messageSet(message);
                std::cout << "Command partial." << std::endl;
                continue ;
            }

            message = c->messageGet().append(Utils::strClean(message));
            if (!message.empty())
                message = message.erase(message.size() - 1);
            message = Utils::strClean(message);

            if(this->irssiIs(message) && !c->irssiIs())
            {
                std::cout << "IRSSI Client" << std::endl;
                std::string nick = this->irssiNickGet(message);
                bool is_not_unick = this->clientsFindByNick(nick) != NULL;
                c->irssi(message);
                if (is_not_unick)
                {
                    nick = Client::nickGenerateUnique();
                    c->nickSet(nick);
                }
                std::cout << *c << std::endl;
                //this->sendx(c->fdGet(), "CAP END");
                continue;
            }

            //std::cout << "RUN CMD: " << message << "*" << std::endl;
            
            if (!message.empty() && message.size() >= 1)
                this->_server_command->commandRun(*c, message);
        }

        if (pollDescriptors[i].revents & (POLLHUP | POLLERR))
        {
            std::cout << "Client disconnected UP ERR: " << c->nickGet() << std::endl;
           
            this->clientsRemove(c->idGet());
        }
    }
}

void Server::start()
{
    this->bindx();
    this->listenx();
    this->pollx();
}

void Server::stop()
{
    if (this->_fd)
    {
        this->clientsSendServerQuitMessage();
        close(this->_fd);
        this->_fd = 0;
        Server::instance = NULL;
    }

    this->_status = 0;
    //std::cout << "STOP ... " << std::endl;
}

void Server::setupSignals()
{
    if (signal(SIGINT, &Server::handleSignalsWrapper) == SIG_ERR) 
    {
        perror("Erreur lors de la configuration du gestionnaire de signal");
        this->stop();
    }
}

void Server::handleSignalsWrapper(int signal) 
{
    Server::instance->handleSignals(signal);
}

void Server::handleSignals(int signal)
{
    (void) signal;
    this->stop();
}

void Server::sendx(int fd_client, const char* message)
{
    // Envoi des données au serveur
    //std::string mes = "\033[32m" + std::string(message) + "\033[0m";
    ssize_t bytes_sent = send(fd_client, message, strlen(message), 0);
    if (bytes_sent != -1)
    {
        std::string mes = "\033[32m" + Utils::now() 
            + ": Données envoyées par le SERVER:\033[0m " + message;
        std::cout << mes << std::endl;
    }
}

std::string Server::recvx(int fd_client)
{
    char buffer[1024];
    ssize_t bytesRead = recv(fd_client, buffer, sizeof(buffer), 0);
    if (bytesRead != -1)
    {
        buffer[bytesRead] = '\0'; // Assure la null-termination de la chaîne
        std::string message = "\033[31m" + Utils::now() 
            + ": Données envoyées par le CLIENT:\033[0m " + buffer;
        std::cout << message << std::endl;
    }
    return std::string(buffer);
}


int Server::fdGet() const
{
    return this->_fd;
}

int Server::statusGet() const
{
    return this->_status;
}

void Server::statusSet(int status)
{
    this->_status = status;
}

void Server::passwordSet(std::string password)
{
    this->_password = password;
}

std::string Server::passwordGet()
{
    return this->_password;
}

void Server::nameSet(std::string name) 
{
    _name = name;
}

std::string Server::nameGet() 
{
    return _name;
}

void Server::networkSet(std::string network) 
{
    _network = network;
}

std::string Server::networkGet() 
{
    return _network;
}

void Server::adminsAdd(Admin *admin) 
{
    _admins.push_back(admin);
}

bool Server::adminsIs(const std::string &name, const std::string &password) 
{
    for (std::vector<Admin*>::iterator it = _admins.begin(); it != _admins.end(); ++it) {
        if ((*it)->name == name && (*it)->password == password) {
            return true;
        }
    }
    return false;
}

bool Server::adminsIs(const std::string &name) 
{
    for (std::vector<Admin*>::iterator it = _admins.begin(); it != _admins.end(); ++it) {
        if ((*it)->name == name) {
            return true;
        }
    }
    return false;
}


std::vector<Client*> &Server::clientsGet()
{
    return this->_clients;
}

// irssi
void Server::clientsConnect(std::string message, Client &cli)
{
    std::istringstream iss(message);
    std::string token;

    // Ignorer la première ligne "CAP LS"
    std::getline(iss, token);

    std::string nick;
    std::string username;
    std::string hostname;
    std::string servername;
    std::string realname;
    // Analyser la ligne "NICK alpha"
    if (iss >> token >> nick) {
        // Analyser la ligne "USER jdoe jdoe 0.0.0.0 :John DOE"
        iss >> username >> hostname >> servername >> token;
        std::getline(iss, realname);
    }
    cli.nickSet(nick);
    cli.usernameSet(username);
    cli.nameRealSet(realname);
}

void Server::clientsRemove(int id) 
{
    for (std::vector<Client*>::iterator it = this->_clients.begin(); it != this->_clients.end();) 
    {
        if ((*it)->idGet() == id) 
        {
            (*it)->channelsDeleteAll();
            int fd = (*it)->fdGet();
            close(fd);
            delete *it;
            *it = NULL;
            it = this->_clients.erase(it);  // Update iterator to the next valid position
            std::cout << "DELETE CLIENT FROM SERVER" << std::endl; 
            break;  // To prevent iterate after erase
        } 
        else 
        {
            ++it;  // Increment the iterator only if no erasure occurs
        }
    }
}

bool Server::clientsIsExist(int fd) 
{
    for (size_t i = 0; i < this->_clients.size(); i++)
    {
        if (this->_clients[i]->fdGet() == fd)
            return true;
    }
    return false;
}

bool Server::clientsIsAdmin(std::string &username) 
{
    for (size_t i = 0; i < this->_clients.size(); i++)
    {
        if (this->_clients[i]->usernameGet() == username)
            return true;
    }
    return false;
}

Client *Server::clientsFindAdmin(std::string &username) 
{
    for (size_t i = 0; i < this->_clients.size(); i++)
    {
        if (this->_clients[i]->usernameGet() == username)
            return this->_clients[i];
    }
    return NULL;
}

Client *Server::clientsFind(int fd) 
{
    for (size_t i = 0; i < this->_clients.size(); ++i)
    {
        if (this->_clients[i]->fdGet() == fd)
            return this->_clients[i];
    }
    return NULL;
}


Client *Server::clientsFindById(int id)
{
    for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) 
    {
        if ((*it)->idGet() == id)
            return (*it);
    }
    return NULL;
}

Client *Server::clientsFindByName(std::string& name)
{
    for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) 
    {
        if ((*it)->usernameGet().find(name) == 0)
            return (*it);
    }
    return NULL;
}
Client* Server::clientsFindByNick(std::string& nick) 
{
    for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if ((*it)->nickGet() == nick) {
            return *it;
        }
    }
    return NULL; // Retourne NULL si le pseudonyme n'est pas trouvé
}

void Server::clientsRemoveByNick(std::string& nick) 
{
    for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end();) {
        if ((*it)->nickGet() == nick) {
            it = _clients.erase(it);
        } else {
            ++it;
        }
    }
}

void Server::clientsSendServerQuitMessage()
{
    for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) 
    {
        this->sendx((*it)->fdGet(), "Server has stopped.\n");
    }
}

void Server::channelsAdd(Channel* channel)
{
    this->_channels.push_back(channel);
}

Channel* Server::channelsFindByName(std::string& name)
{
    for (std::vector<Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it) 
    {
        if ((*it)->nameGet() == name)
            return (*it);
    }
    return NULL;
}

Log *Server::logGet()
{
    return this->_log;
}

Bot *Server::botGet()
{
    return this->_bot;
}

bool  Server::irssiIs(const std::string& message) 
{
    const std::string prefixToCheck = "CAP LS";

    return message.compare(0, prefixToCheck.length(), prefixToCheck) == 0;
}

std::string  Server::irssiNickGet(const std::string& message) 
{
    std::istringstream iss(message);
    std::string command;
    std::string nick;
    while (iss >> command) 
    {
        if (command == "NICK") 
        {
            iss >> nick;
            break ;
        }
        else if (command == "CAP" && (iss >> command) && command == "LS") 
        {

        }
    }

    return nick; 
}

std::ostream &operator<<(std::ostream &out, const Server &server)
{
    out << "Channel [";
    out << "Name: " << server._name << ", ";
    out << "Network: " << server._network << ", ";
    out << "Password: " << server._password << ", ";
    out << "]";
    return out;
}