#include "../../../include/core/ServerCommand.hpp"
#include "../../../include/core/Server.hpp"
#include "../../../include/core/Client.hpp"
#include "../../../include/core/Channel.hpp"
#include "../../../include/core/Utils.hpp"
#include <string>

bool ServerCommand::commandIsValidBot(std::string &command)
{
    std::vector<std::string> args = ServerCommand::commandArgsGet(command);
    
    if (args.empty() 
        || args.size() < 2
        || !this->commandIsValidName(args[0], "BOT"))
        return false;
    return true;
}

void ServerCommand::commandRunBot(Client &cli)
{
    std::string message = cli.messageGet();
    std::vector<std::string> args = ServerCommand::commandArgsGetExecptLast(message, 1);
    std::cout << "-- BOT " << std::endl;

    if (this->commandIsValidBot(message))
    {
        // std::ostringstream oss;
        // oss << cli << std::endl;
        // message = oss.str();
        // //this->_server->logGet()->append(message, 0);
        // this->_server->sendx(cli.fdGet(), message.c_str());
        Bot *bot = this->_server->botGet();
        Channel *c = NULL;
        
        if (cli.channelsGet().size())
        {
            c = cli.channelsGet()[0];
        }        

        if (bot && c)
        {
            c->bot(cli, *bot, Utils::trim(args[1]));
        }
    }
}