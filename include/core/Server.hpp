#pragma once

#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <vector>

class ServerCommand;
class Client;
class Channel;
class Log;
class Bot;

struct Admin
{
    std::string name;
    std::string password;
};

class Server
{
    private:

        static Server* instance;

        int _fd;
        int _status;
        std::string _address;
        int _port;
        std::string _password;
        sockaddr_in _server_address;
        std::string _name;
        std::string _network;
        
        Log* _log;
        ServerCommand* _server_command;

        std::vector<Admin*> _admins;
        std::vector<Client*> _clients;
        std::vector<Channel*> _channels;

        Bot *_bot;
        
        void setup();
        void setupSignals();
        void bindx();
        void listenx();
        void pollx();
        void pollxServer(std::vector<struct pollfd>& pollDescriptors);
        void pollxClients(std::vector<struct pollfd>& pollDescriptors);
        void handleSignals(int signal);
        static void handleSignalsWrapper(int signal);
        static void* handleClient(void* arg);

    public:
        
        Server();
        Server(std::string address, int port, std::string password);
        ~Server();
        
        int fdGet() const;
        int statusGet() const;
        void statusSet(int status);

        void passwordSet(std::string username);
        std::string passwordGet();

        void nameSet(std::string name);
        std::string nameGet();

        void networkSet(std::string network);
        std::string networkGet();

        void adminsAdd(Admin *admin);
        bool adminsIs(const std::string &name, const std::string &password);
        bool adminsIs(const std::string &name);

        Log *logGet();
        Bot *botGet();

        void sendx(int fd_client, const char *message);
        std::string recvx(int fd_client);
        void start();
        void stop();

        std::vector<Client*> &clientsGet();
        void clientsConnect(std::string message, Client &cli);
        bool clientsIsAdmin(std::string &username);
        Client *clientsFindAdmin(std::string &username);
        Client* clientsFind(int fd);
        Client* clientsFindById(int id);
        Client* clientsFindByName(std::string& name);
        Client* clientsFindByNick(std::string& nick);
        void clientsRemoveByNick(std::string& nick);
        void clientsRemove(int id);
        bool clientsIsExist(int fd);
        void clientsSendServerQuitMessage();

        void channelsAdd(Channel* channel);
        Channel* channelsFindByName(std::string& name);
        
        bool  irssiIs(const std::string& message);
        std::string irssiNickGet(const std::string& message);

        friend std::ostream &operator<<(std::ostream &out, const Server &server);
};
