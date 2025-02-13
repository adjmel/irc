#pragma once

#include <string>

class ServerClient
{
    private:
        
        int _fd;

    public:
        
        ServerClient();
        ~ServerClient();

        void connectx(const char *server_ip, int server_port);
        void sendx(const char *message);
        std::string receivex();
        void disconnectx();
};
