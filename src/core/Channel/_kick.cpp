#include "../../../include/core/Channel.hpp"
#include "../../../include/core/Log.hpp"
#include "../../../include/core/Server.hpp"
#include "../../../include/core/Client.hpp"
#include "../../../include/core/Bot.hpp"
#include "../../../include/core/Utils.hpp"
#include "../../../include/core/numeric_response.hpp"
#include <unistd.h>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <ctime>
#include <sstream>
#include <sys/time.h>
#include <cstdlib> 

int Channel::kick(Client &mod, Client &cli, std::string reason)
{
    this->_log->append("KICK", 0);

    std::string message;

    if (!this->moderatorsCan(mod, "o"))
    {
        if (this->_server)
        {
            message = ERR_CHANOPRIVSNEEDED(this->_server->nameGet(), this->_name);
            this->_server->logGet()->append(message, 0);
            this->_server->sendx(mod.fdGet(), message.c_str());
        }
        return 1;
    }
    
    if (!this->clientsHas(mod))
    {
        if (this->_server)
        {
            message = ERR_NOTONCHANNEL(this->_server->nameGet(), this->_name);
            this->_server->logGet()->append(message, 0);
            this->_server->sendx(mod.fdGet(), message.c_str());
        }
        return 1;
    }

    if (!this->clientsHas(cli))
    {
        if (this->_server)
        {
            message = ERR_NOTONCHANNEL(this->_server->nameGet(), this->_name);
            this->_server->logGet()->append(message, 0);
            this->_server->sendx(mod.fdGet(), message.c_str());
        }
        return 1;
    }
    
    this->clientsRemove(cli.idGet());
    if (this->_server)
    {
        message = RPL_KICK(mod.nickGet(), this->_name, cli.nickGet(), reason);
        this->_server->logGet()->append(message, 0);
        this->_server->sendx(cli.fdGet(), message.c_str());
    }
    this->_log->append(message, 0);

    return 0;
}