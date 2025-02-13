#include "../../../include/core/ServerCommand.hpp"
#include "../../../include/core/Server.hpp"
#include "../../../include/core/Client.hpp"
#include "../../../include/core/Channel.hpp"
#include "../../../include/core/Log.hpp"
#include "../../../include/core/Utils.hpp"
#include "../../../include/core/numeric_response.hpp"
#include <string>

bool ServerCommand::commandIsValidQuit(std::string &command)
{
    std::vector<std::string> args = ServerCommand::commandArgsGetExecptLast(command, 2);
    
    if (args.empty() 
        || args.size() < 1
        || !this->commandIsValidName(command, "QUIT"))
        return false;
    return true;
}

void ServerCommand::commandRunQuit(Client &cli)
{
    std::string message = cli.messageGet();
    std::vector<std::string> args = ServerCommand::commandArgsGetExecptLast(message, 2);
    std::cout << "-- QUIT " << std::endl;

    if (this->commandIsValidQuit(message))
    {
        std::string msg = args.size() == 2 ? Utils::trim(args[1]) : "";

        this->_server->clientsRemove(cli.idGet());

        // peut etre envoyer aux autres
        message = RPL_QUIT(this->_server->nameGet(), msg);
        this->_server->logGet()->append(message, 0);
    }
}