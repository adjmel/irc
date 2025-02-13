#include "../../../include/core/ServerCommand.hpp"
#include "../../../include/core/Server.hpp"
#include "../../../include/core/Client.hpp"
#include "../../../include/core/Channel.hpp"
#include "../../../include/core/Log.hpp"
#include "../../../include/core/Utils.hpp"
#include "../../../include/core/numeric_response.hpp"
#include <string>

bool ServerCommand::commandIsValidServerQuit(std::string &command)
{
    std::vector<std::string> args = ServerCommand::commandArgsGetExecptLast(command, 3);

    if (args.empty() 
        || args.size() < 1
        || !this->commandIsValidName(args[0], "SQUIT"))
        return false;
    return true;
}

void ServerCommand::commandRunServerQuit(Client &cli)
{
    std::string message = cli.messageGet();
    std::vector<std::string> args = ServerCommand::commandArgsGetExecptLast(message, 3);
    std::cout << "-- SQUIT " << std::endl;

    if (this->commandIsValidServerQuit(message))
    {
        std::string msg = args.size() == 2 ? Utils::trim(args[1]) : "";
        
        this->_server->statusSet(0);
   
        message = RPL_QUIT(this->_server->nameGet(), msg);
        this->_server->logGet()->append(message, 0);
    }
}

