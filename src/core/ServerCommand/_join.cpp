#include "../../../include/core/ServerCommand.hpp"
#include "../../../include/core/Server.hpp"
#include "../../../include/core/Client.hpp"
#include "../../../include/core/Channel.hpp"
#include "../../../include/core/Log.hpp"
#include "../../../include/core/Utils.hpp"
#include "../../../include/core/numeric_response.hpp"
#include <string>
#include <cctype>

bool ServerCommand::commandIsValidJoinChannelName(std::string name)
{
    if (name.empty() || name[0] != '#') {
        return false;
    }

    for (std::size_t i = 1; i < name.length(); ++i) 
    {
        char c = name[i];
        if (!std::isalnum(c) && c != '-' && c != '_' && c != '+') 
        {
            return false;
        }
    }

    if (name.length() > 32) 
    {
        return false;
    }

    return true;
}

bool ServerCommand::commandIsValidJoin(std::string &command)
{
    std::vector<std::string> args = ServerCommand::commandArgsGet(command);

    if (args.empty()
        || !this->commandIsValidName(args[0], "JOIN"))
        return false;
    return true;
}

void ServerCommand::commandRunJoin(Client &cli)
{
    std::string message = cli.messageGet();
    std::vector<std::string> args = ServerCommand::commandArgsGet(message);
    std::cout << "-- JOIN " << std::endl;
    
    if (this->commandIsValidJoin(message) && cli.statusGet() == 3)
    {
        std::string name = Utils::trim(args[1]);
        std::vector<std::string> names = Utils::split(name,",");

        std::string password = args.size() == 3 ? Utils::trim(args[2]) : "";
        
        Channel *c  = NULL;
        if (names.size() > 1)
        {
            for (size_t i = 0; i < names.size(); i++)
            {
                c = this->_server->channelsFindByName(names[i]);
                if (c)
                {
                   c->join(cli, password);
                }
                else 
                {          
                    //std::cout << "-- JOIN create " << name << std::endl;  
                    c = new Channel(this->_server, cli);
                    c->nameSet(name);
                    this->_server->channelsAdd(c); 

                    message = RPL_JOIN(cli.nickGet(), c->nameGet());
                    this->_server->logGet()->append(message, 0);
                    this->_server->sendx(cli.fdGet(), message.c_str());
                }
            }
        }
        else
        {
            c = this->_server->channelsFindByName(name);
            if (c)
            {
                c->join(cli, password);        
            }
            else 
            {
                if (name[0] != '#')
                    return ;
                c = new Channel(this->_server, cli);
                c->nameSet(name);
                this->_server->channelsAdd(c); 

                message = RPL_JOIN(cli.nickGet(), c->nameGet());
                this->_server->logGet()->append(message, 0);
                this->_server->sendx(cli.fdGet(), message.c_str());
            }
        }
    }
}