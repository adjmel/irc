#include "../../../include/core/ServerCommand.hpp"
#include "../../../include/core/Server.hpp"
#include "../../../include/core/Client.hpp"
#include "../../../include/core/Utils.hpp"
#include "../../../include/core/Log.hpp"
#include "../../../include/core/numeric_response.hpp"
#include <string>

bool ServerCommand::commandIsValidPong(std::string &command)
{
    std::vector<std::string> args = ServerCommand::commandArgsGet(command);
    
    if (args.empty() 
        || args.size() < 2
        || !this->commandIsValidName(args[0], "PONG"))
        return false;
    return true;
}

void ServerCommand::commandRunPong(Client &cli)
{
    std::string message = cli.messageGet();
    std::vector<std::string> args = ServerCommand::commandArgsGet(message);
    std::cout << "-- PONG " << std::endl;
    std::string nick = Utils::trim(args[1]);

    if (this->commandIsValidPong(message))
    {
        
    }
}