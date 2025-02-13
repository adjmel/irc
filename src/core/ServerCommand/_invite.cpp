#include "../../../include/core/ServerCommand.hpp"
#include "../../../include/core/Server.hpp"
#include "../../../include/core/Client.hpp"
#include "../../../include/core/Channel.hpp"
#include "../../../include/core/Log.hpp"
#include "../../../include/core/Utils.hpp"
#include "../../../include/core/numeric_response.hpp"
#include <string>

bool ServerCommand::commandIsValidInvite(std::string &command)
{
    std::vector<std::string> args = ServerCommand::commandArgsGet(command);
    
    if (args.empty()
        || args.size() < 3
        || !this->commandIsValidName(args[0], "INVITE"))
        return false;
    return true;
}

void ServerCommand::commandRunInvite(Client &cli)
{
    std::string message = cli.messageGet();
    std::vector<std::string> args = ServerCommand::commandArgsGet(message);
    std::cout << "-- INVITE " << std::endl;
    if (this->commandIsValidInvite(message))
    {
        // std::cout << "-- INVITE " << std::endl;
        std::string nick = Utils::trim(args[1]);
        std::string channel_name = Utils::trim(args[2]);
        Client* c = this->_server->clientsFindByNick(nick);
        Channel* ch = this->_server->channelsFindByName(channel_name);
        if (!c)
        {
            // std::cout << "-- INVITE 55" << std::endl;
            message = ERR_NOSUCHNICK(this->_server->nameGet(), nick);
            this->_server->logGet()->append(message, 0);
            //this->_server->sendx(cli.fdGet(), message.c_str()); 
        }
        else if (!ch)
        {
            // std::cout << "-- INVITE 4" << std::endl;
            message = ERR_NOTONCHANNEL(this->_server->nameGet(), channel_name);
            this->_server->logGet()->append(message, 0);
            //this->_server->sendx(cli.fdGet(), message.c_str()); 
        }
        
        if (ch && c 
            && ch->clientsHas(cli)
            && !ch->clientsHas(*c))
        {
            // std::cout << "-- INVITE ++" << std::endl;
            ch->invite(cli, *c);
        }
    }
}