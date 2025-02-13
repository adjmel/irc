#include "../../../include/core/ServerCommand.hpp"
#include "../../../include/core/Server.hpp"
#include "../../../include/core/Client.hpp"
#include "../../../include/core/Channel.hpp"
#include "../../../include/core/Log.hpp"
#include "../../../include/core/Utils.hpp"
#include "../../../include/core/numeric_response.hpp"
#include <string>

bool ServerCommand::commandIsValidKick(std::string &command)
{
    std::vector<std::string> args = ServerCommand::commandArgsGetExecptLast(command, 3);
    
    if (args.empty() 
        || args.size() < 3
        || !this->commandIsValidName(args[0], "KICK"))
        return false;
    return true;
}

void ServerCommand::commandRunKick(Client &cli)
{
    std::string message = cli.messageGet();
    std::vector<std::string> args = ServerCommand::commandArgsGetExecptLast(message, 3);
    std::cout << "-- KICK " << std::endl;

    if (this->commandIsValidKick(message) && cli.statusGet() == 3)
    {
        std::string channel_name = Utils::trim(args[1]);
        std::string nick = Utils::trim(args[2]);
        std::string reason = args.size() == 4 ? Utils::trim(args[3]) : "";

        Channel *ch = this->_server->channelsFindByName(channel_name);
        Client *c = this->_server->clientsFindByNick(nick);

        if (ch && c)
        {
            //std::cout << "-- KICK ---" << std::endl;
            ch->kick(cli, *c, reason);
        }

        // if (ch && !cli.channelsFindByName(channel_name))
        // {
        //     message = ERR_NOTONCHANNEL(this->_server->nameGet(), ch->nameGet());
        //     this->_server->logGet()->append(message, 0);
        //     //this->_server->sendx(cli.fdGet(), message.c_str()); 
        // }
        // else if (c && ch && !ch->clientsFindByNick(nick))
        // {
        //     message = ERR_NOSUCHNICK(this->_server->nameGet(), c->nickGet());
        //     this->_server->logGet()->append(message, 0);
        //     //this->_server->sendx(cli.fdGet(), message.c_str()); 
        // }
        
        // if(c && ch
        //     && cli.channelsFindByName(channel_name)
        //     && c->channelsFindByName(channel_name))
        // {
        //     std::cout << "-- KICK ---" << std::endl;
        //     ch->kick(cli, *c, reason);
        // }
    }
}