#include "../../../include/core/ServerCommand.hpp"
#include "../../../include/core/Server.hpp"
#include "../../../include/core/Client.hpp"
#include "../../../include/core/Channel.hpp"
#include "../../../include/core/Utils.hpp"
#include <string>
#include <iostream>
#include <sstream>

bool ServerCommand::commandIsValidInfoMe(std::string &command)
{
    std::vector<std::string> args = ServerCommand::commandArgsGet(command);
    
    if (args.empty()
        || args.size() < 1
        || !this->commandIsValidName(args[0], "INFO_ME"))
        return false;
    return true;
}

void ServerCommand::commandRunInfoMe(Client &cli)
{
    std::string message = cli.messageGet();
    std::vector<std::string> args = ServerCommand::commandArgsGet(message);
    //std::cout << "-- INFO_ME " << std::endl;

    if (this->commandIsValidInfoMe(message))
    {
        std::ostringstream oss;
        oss << cli << std::endl;
        message = oss.str();
        //this->_server->logGet()->append(message, 0);
        this->_server->sendx(cli.fdGet(), message.c_str()); 
    }
}