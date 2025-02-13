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

int Channel::topic(Client &mod)
{
    this->_log->append("TOPIC", 0);

    std::string message;

    if (!this->clientsHas(mod))
    {
        if (this->_server)
        {
            message = ERR_NOTONCHANNEL(this->_server->nameGet(), this->_name);
            this->_server->logGet()->append(message, 0);
            this->_server->sendx(mod.fdGet(), message.c_str());
            this->_server->sendx(mod.fdGet(), "\n"); 
        }
        return 1;
    }

    if (this->_server)
    {
        if (this->_topic.empty())
            message = RPL_NOTOPIC(mod.nickGet(), this->_name);
        else
            message = RPL_TOPIC(mod.nickGet(), this->_name, this->_topic);

        this->_server->logGet()->append(message, 0);
        this->_server->sendx(mod.fdGet(), message.c_str());
        this->_server->sendx(mod.fdGet(), "\n"); 
    }
    this->_log->append(message, 0);
    return 0;
}

int Channel::topic(Client &mod, std::string topic)
{
    this->_log->append("TOPIC", 0);

    std::string message;

    if (mod.irssiIs())
    {
        if (!topic.empty() && topic[0] == ':')
            topic.erase(0, 1);
        
        std::cout << "--- " << topic << std::endl; 
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

    if (!this->moderatorsCan(mod,"o"))
    {
        if (this->_server)
        {
            message = ERR_CHANOPRIVSNEEDED(this->_server->nameGet(), this->_name);
            this->_server->logGet()->append(message, 0);
            this->_server->sendx(mod.fdGet(), message.c_str());
        }
        return 1;
    }
    
    if (!this->modeCan("t"))
        return 1;

    this->_topic = topic;
    if (this->_server)
    {
        message = RPL_TOPIC(mod.nickGet(), this->_name, this->_topic);
        this->_server->logGet()->append(message, 0);
        this->_server->sendx(mod.fdGet(), message.c_str());
    }
    this->_log->append(message, 0);

    message = "Topic has changed :" + this->_topic;
     std::string out_one  = "";
    std::vector<Client*>::iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (this->_server && *it != &mod)
        {   
            std::string out_one = RPL_TOPIC(mod.nickGet(), this->_name, this->_topic);
            this->_server->logGet()->append(out_one, 1);
            this->_server->sendx((*it)->fdGet(), out_one.c_str()); 
        }
    }
    return 0;
}