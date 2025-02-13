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

int Channel::modeI(Client &mod, std::string mode)
{
    this->_log->append("MODE i", 0);

    std::string message;

    if (!this->clientsHas(mod))
    {
        if (this->_server)
        {
            message = ERR_NOTONCHANNEL(this->_server->nameGet(), this->_name);
            this->_server->logGet()->append(message, 0);
            //this->_server->sendx(mod.fdGet(), message.c_str());
        }
        return 1;
    }

    if (!this->moderatorsCan(mod, "o"))
    {
        if (this->_server)
        {
            message = ERR_CHANOPRIVSNEEDED(this->_server->nameGet(), this->_name);
            this->_server->logGet()->append(message, 0);
            //this->_server->sendx(mod.fdGet(), message.c_str());
        }
        return 1;
    }

    this->modeAdd(mode);
    
    if (this->_server)
    {
        message = RPL_MODE(mod.nickGet(), this->_name, this->_mode, "");
        this->_server->logGet()->append(message, 0);
        this->_server->sendx(mod.fdGet(), message.c_str());
    }
    return 0;
}

int Channel::modeT(Client &mod, std::string mode)
{
    this->_log->append("MODE t", 0);

    std::string message;

    if (!this->clientsHas(mod))
    {
        if (this->_server)
        {
            message = ERR_NOTONCHANNEL(this->_server->nameGet(), this->_name);
            this->_server->logGet()->append(message, 0);
            //this->_server->sendx(mod.fdGet(), message.c_str()); 
        }
        return 1;
    }

    if (!this->moderatorsCan(mod, "o"))
    {
        if (this->_server)
        {
            message = ERR_CHANOPRIVSNEEDED(this->_server->nameGet(), this->_name);
            this->_server->logGet()->append(message, 0);
            //this->_server->sendx(mod.fdGet(), message.c_str());
        }
        return 1;
    }

    this->modeAdd(mode);
    
    if (this->_server)
    {
        message = RPL_MODE(mod.nickGet(), this->_name, this->_mode, "");
        this->_server->logGet()->append(message, 0);
        this->_server->sendx(mod.fdGet(), message.c_str());
    }
    return 0;
}

int Channel::modeK(Client &mod, std::string mode, std::string password)
{
    this->_log->append("MODE k", 0);

    std::string message;

    if (!this->clientsHas(mod))
    {
        if (this->_server)
        {
            message = ERR_NOTONCHANNEL(this->_server->nameGet(), this->_name);
            this->_server->logGet()->append(message, 0);
            //this->_server->sendx(mod.fdGet(), message.c_str());
        }
        return 1;
    }

    if (!this->moderatorsCan(mod, "o"))
    {
        if (this->_server)
        {
            message = ERR_CHANOPRIVSNEEDED(this->_server->nameGet(), this->_name);
            this->_server->logGet()->append(message, 0);
            //this->_server->sendx(mod.fdGet(), message.c_str());
        }
        return 1;
    }

    this->modeAdd(mode);
    this->_password = password;
    
    if (this->_server)
    {
        message = RPL_MODE(mod.nickGet(), this->_name, this->_mode, password);
        this->_server->logGet()->append(message, 0);
        this->_server->sendx(mod.fdGet(), message.c_str());
    }
    return 0;
}

int Channel::modeO(Client &mod, std::string mode, Client &cli)
{
    this->_log->append("MODE o", 0);

    std::string message;

    if (!this->clientsHas(mod))
    {
        if (this->_server)
        {
            message = ERR_NOTONCHANNEL(this->_server->nameGet(), this->_name);
            this->_server->logGet()->append(message, 0);
            //this->_server->sendx(mod.fdGet(), message.c_str()); 
        }
        return 1;
    }

    if (!this->clientsHas(cli))
    {
        if (this->_server)
        {
            message = ERR_NOTONCHANNEL(this->_server->nameGet(), this->_name);
            this->_server->logGet()->append(message, 0);
            //this->_server->sendx(mod.fdGet(), message.c_str());
        }
        return 1;
    }

    if (!this->moderatorsCan(mod, "o"))
    {
        if (this->_server)
        {
            message = ERR_CHANOPRIVSNEEDED(this->_server->nameGet(), this->_name);
            this->_server->logGet()->append(message, 0);
            //this->_server->sendx(mod.fdGet(), message.c_str());
        }
        return 1;
    }

    this->moderatorsAdd(cli);
    this->moderatorsModeUpdate(cli, mode);
    //cli.modeAdd(mode);
    
    if (this->_server)
    {
        message = RPL_MODE(mod.nickGet(), this->_name, this->_mode, cli.nickGet());
        this->_server->logGet()->append(message, 0);
        this->_server->sendx(mod.fdGet(), message.c_str());
    }
    return 0;
}

int Channel::modeL(Client &mod, std::string mode, int clients_max)
{
    this->_log->append("MODE l", 0);

    std::string message;

    if (!this->moderatorsCan(mod, "o"))
    {
        if (this->_server)
        {
            message = ERR_CHANOPRIVSNEEDED(this->_server->nameGet(), this->_name);
            this->_server->logGet()->append(message, 0);
            //this->_server->sendx(mod.fdGet(), message.c_str());
        }
        return 1;
    }

    this->modeAdd(mode);
    this->_clients_max = clients_max;

    if (this->_server)
    {
        message = RPL_MODE(mod.nickGet(), this->_name, this->_mode, Utils::intToString(clients_max));
        this->_server->logGet()->append(message, 0);
        this->_server->sendx(mod.fdGet(), message.c_str());
    }
    return 0;
}

int Channel::modeL(Client &mod, std::string mode)
{
    this->_log->append("MODE l", 0);

    std::string message;

    if (!this->moderatorsCan(mod, "o"))
    {
        if (this->_server)
        {
            message = ERR_CHANOPRIVSNEEDED(this->_server->nameGet(), this->_name);
            this->_server->logGet()->append(message, 0);
            //this->_server->sendx(mod.fdGet(), message.c_str());
        }
        return 1;
    }

    this->modeAdd(mode);

    if (this->_server)
    {
        message = RPL_MODE(mod.nickGet(), this->_name, this->_mode, "");
        this->_server->logGet()->append(message, 0);
        this->_server->sendx(mod.fdGet(), message.c_str());
    }
    return 0;
}
