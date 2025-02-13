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

int Channel::join(Client &cli, std::string &password)
{
    this->_log->append("JOIN", 0);

    std::string message;

    if (this->clientsHas(cli) && !cli.channelRecentIs(this))
    {
        cli.channelRecentSet(this);
        if (this->_clients.size() == 0)
        {
            this->moderatorsAdd(cli);
            this->moderatorsModeUpdate(cli, "+o+i+k+t+l");
        }
        if (this->_server)
        {
            message = RPL_JOIN(cli.nickGet(), this->_name);
            this->_server->logGet()->append(message, 0);
            this->_server->sendx(cli.fdGet(), message.c_str()); 
        }
        this->_log->append(message, 0);
        this->restore(cli, this);
        return 0;
    }

    if (this->modeCan("i"))
    {
        if(this->clientsInvitedConfirm(cli))
        {
            if (this->_clients.size() == 0)
            {
                this->moderatorsAdd(cli);
                this->moderatorsModeUpdate(cli, "+o+i+k+t+l");
            }
            if (this->_server)
            {
                message = RPL_JOIN(cli.nickGet(), this->_name);
                this->_server->logGet()->append(message, 0);
                this->_server->sendx(cli.fdGet(), message.c_str()); 
            }
            this->_log->append(message, 0);
            this->restore(cli, this);
            return 0;
        }

        if (this->_server)
        {
            message = ERR_INVITONLYCHAN(this->_server->nameGet(), this->_name);
            this->_server->logGet()->append(message, 0);
            this->_server->sendx(cli.fdGet(), message.c_str());
        }
        return 1;
    }

    if (this->modeCan("k")
        && this->_password != password)
    {
        if (this->_server)
        {
            message = ERR_BADCHANNELKEY(this->_server->nameGet(), this->_name);
            this->_server->logGet()->append(message, 0);
            this->_server->sendx(cli.fdGet(), message.c_str());
        }
        return 1;
    }
    
    // if (this->clientsBanIs(cli))
    // {
    //     // ban err
    //     return 1;
    // }

    if (this->clientsHas(cli))
    {
        if (this->_server)
        {
            message = ERR_USERONCHANNEL(this->_server->nameGet(), cli.nickGet(), this->_name);
            this->_server->logGet()->append(message, 0);
            this->_server->sendx(cli.fdGet(), message.c_str());
        }
        return 1; 
    }

    if (this->modeCan("l")
        && this->clientsMaxIsMax())
    {
        if (this->_server)
        {
            message = ERR_CHANNELISFULL(this->_server->nameGet(), this->_name);
            this->_server->logGet()->append(message, 0);
            this->_server->sendx(cli.fdGet(), message.c_str());
        }
        return 1; 
    }

    if (this->_clients.size() == 0)
    {
        this->moderatorsAdd(cli);
        this->moderatorsModeUpdate(cli, "+o+i+k+t+l");
    }
    _clients.push_back(&cli);
    cli.channelsAdd(this);
    cli.channelRecentSet(this);
    if (this->_server)
    {
        message = RPL_JOIN(cli.nickGet(), this->_name);
        this->_server->logGet()->append(message, 0);
        this->_server->sendx(cli.fdGet(), message.c_str()); 
    }
    this->_log->append(message, 0);
    this->restore(cli, this);
    return 0;
}