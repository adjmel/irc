#include "../../../include/core/ServerCommand.hpp"
#include "../../../include/core/Server.hpp"
#include "../../../include/core/Client.hpp"
#include "../../../include/core/Log.hpp"
#include "../../../include/core/Utils.hpp"
#include "../../../include/core/numeric_response.hpp"
#include <string>

bool ServerCommand::commandIsValidUser(std::string &command)
{
    std::vector<std::string> args = ServerCommand::commandArgsGetExecptLast(command, 4);
    
    if (args.empty() 
        || args.size() != 5
        || !this->commandIsValidName(args[0], "USER"))
        return false;
    return true;
}

void ServerCommand::commandRunUser(Client &cli)
{
    std::string message = cli.messageGet();
    std::vector<std::string> args = ServerCommand::commandArgsGetExecptLast(message, 4);
    std::cout << "-- USER " << std::endl;

    //USER MonPseudo localhost serveur-irc-localhost :John Doe    
    if (this->commandIsValidUser(message) && cli.statusGet() == 2)
    {
        cli.usernameSet(Utils::trim(args[1]));
        cli.hostnameSet(Utils::trim(args[2]));
        cli.servernameSet(Utils::trim(args[3]));
        cli.nameRealSet(Utils::trim(args[4]));
        cli.statusSet(3);

        message = RPL_WELCOME(cli.nickGet(), cli.nickGet());
        this->_server->logGet()->append(message, 0);
        this->_server->sendx(cli.fdGet(), message.c_str());
    }
}