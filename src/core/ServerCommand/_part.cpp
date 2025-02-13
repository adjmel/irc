#include "../../../include/core/ServerCommand.hpp"
#include "../../../include/core/Server.hpp"
#include "../../../include/core/Channel.hpp"
#include "../../../include/core/Client.hpp"
#include "../../../include/core/Utils.hpp"
#include "../../../include/core/Log.hpp"
#include "../../../include/core/numeric_response.hpp"
#include <string>

bool ServerCommand::commandIsValidPart(std::string &command)
{
    std::vector<std::string> args = ServerCommand::commandArgsGetExecptLast(command, 2);
    
    if (args.empty() 
        || args.size() < 2
        || !this->commandIsValidName(args[0], "PART"))
        return false;
    return true;
}

void ServerCommand::commandRunPart(Client &cli)
{
    std::string message = cli.messageGet();
    std::vector<std::string> args = ServerCommand::commandArgsGetExecptLast(message, 2);
    std::cout << "-- PART " << std::endl;
   
    std::string channel_name = Utils::trim(args[1]);
    std::string reason = args.size() == 3 ? Utils::trim(args[2]) : "";
    if (this->commandIsValidPart(message) && cli.statusGet() >= 1)
    {
        Channel *ch = cli.channelsFindByName(channel_name);
        if (ch)
        {
            ch->part(cli, reason);
        }
        else
        {
            message = RPL_PART(cli.nickGet(), channel_name, reason);
            this->_server->logGet()->append(message, 0);
            this->_server->sendx(cli.fdGet(), message.c_str());
            
            this->_server->logGet()->append(message, 0);
        }
    }
}