#pragma once

#include <string>

class Server;

class ServerConfig
{
    private:

        void loadConfigServerInfo(Server *erver, std::ifstream& configFile);
        void loadConfigAdmin(Server *server, std::ifstream& configFile);
        void loadConfigChannels(Server *server, std::ifstream& configFile);

    public:

        ServerConfig(Server *server);
        ~ServerConfig();

        void loadConfig(Server *server, std::string filename);
};

