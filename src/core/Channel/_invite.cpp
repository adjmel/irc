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

int Channel::invite(Client &mod, Client &cli)
{
    this->_log->append("INVITE", 0);

    std::string message;

    if (!this->clientsHas(mod))
    {
        if (this->_server)
        {
            message = ERR_USERONCHANNEL(this->_server->nameGet(), cli.nickGet(), this->_name);
            this->_server->logGet()->append(message, 0);
            //this->_server->sendx(cli.fdGet(), message.c_str());
        }
        return 1;
    }

    if (this->modeCan("i") && !this->moderatorsCan(mod, "o"))
    {
        if (this->_server)
        {
            message = ERR_CHANNELISFULL(this->_server->nameGet(), this->_name);
            this->_server->logGet()->append(message, 0);
            //this->_server->sendx(mod.fdGet(), message.c_str());
        }
        return 1;
    }
    
    // this->_clients.push_back(&cli);
    // cli.channelsAdd(this);
    this->clientsInvitedAdd(cli);
    // if (this->clientsBanIs(cli))
    //     this->clientsBanRemove(cli);

    if (this->_server)
    {
        message = RPL_INVITE(mod.nickGet(), cli.nickGet(), this->_name);
        this->_server->logGet()->append(message, 0);
        this->_server->sendx(cli.fdGet(), message.c_str());
    }
    this->_log->append(message, 0);
    return 0;
}