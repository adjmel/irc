#include "../../../include/core/ServerCommand.hpp"
#include "../../../include/core/Server.hpp"
#include "../../../include/core/Log.hpp"
#include "../../../include/core/Client.hpp"
#include "../../../include/core/Channel.hpp"
#include "../../../include/core/Utils.hpp"
#include <string>
#include <iostream>
#include <sstream>

bool ServerCommand::commandIsValidInfoMods(std::string &command)
{
    std::vector<std::string> args = ServerCommand::commandArgsGet(command);
    
    if (args.empty()
        || args.size() < 2
        || !this->commandIsValidName(args[0], "INFO_MODS"))
        return false;
    return true;
}

void ServerCommand::commandRunInfoMods(Client &cli)
{
    std::string message = cli.messageGet();
    std::vector<std::string> args = ServerCommand::commandArgsGet(message);
    std::cout << "-- INFO_MODS " << std::endl;

    if (this->commandIsValidInfoMods(message))
    {
        std::string channel_name = Utils::trim(args[1]);
        Channel *ch = this->_server->channelsFindByName(channel_name);

        if (ch == NULL || !cli.channelsFindByName(channel_name))
        {
            return ;
        }
        std::vector<Moderator*> mods = ch->moderatorsGet();

        std::vector<Moderator*>::iterator it;
        for (it = mods.begin(); it != mods.end(); ++it)
        {   
            Client *c = this->_server->clientsFindById((*it)->id);
    
            if (c)
            {
                std::ostringstream oss;
                oss << c->nickGet() << " [" << (*it)->mode << "]" << std::endl;
                message = oss.str();
                this->_server->logGet()->append(message, 0);
                this->_server->sendx(cli.fdGet(), message.c_str()); 
            }
        }
    }
}