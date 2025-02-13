#include "../../../include/core/ServerCommand.hpp"
#include "../../../include/core/Server.hpp"
#include "../../../include/core/Client.hpp"
#include "../../../include/core/Utils.hpp"
#include "../../../include/core/Log.hpp"
#include "../../../include/core/numeric_response.hpp"
#include <string>

bool ServerCommand::commandIsValidPing(std::string &command)
{
    std::vector<std::string> args = ServerCommand::commandArgsGet(command);
    
    if (args.empty() 
        || args.size() < 2
        || !this->commandIsValidName(args[0], "PING"))
        return false;
    return true;
}

void ServerCommand::commandRunPing(Client &cli)
{
    std::string message = cli.messageGet();
    std::vector<std::string> args = ServerCommand::commandArgsGet(message);
    std::cout << "-- PING " << std::endl;
    std::string nick = Utils::trim(args[1]);

    if (this->commandIsValidPing(message))
    {
        message = ":" + nick + " PONG :" + nick + "\n";
        this->_server->logGet()->append(message, 0);
        //this->_server->sendx(cli.fdGet(), message.c_str());
    }
}