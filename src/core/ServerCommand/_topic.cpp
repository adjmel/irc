#include "../../../include/core/ServerCommand.hpp"
#include "../../../include/core/Server.hpp"
#include "../../../include/core/Client.hpp"
#include "../../../include/core/Channel.hpp"
#include "../../../include/core/Log.hpp"
#include "../../../include/core/Utils.hpp"
#include "../../../include/core/numeric_response.hpp"
#include <string>

bool ServerCommand::commandIsValidTopic(std::string &command)
{
   std::vector<std::string> args = ServerCommand::commandArgsGet(command);
    
    if (args.empty() 
        || args.size() < 2
        || !this->commandIsValidName(args[0], "TOPIC"))
        return false;
    return true;
}

void ServerCommand::commandRunTopic(Client &cli)
{
    std::string message = cli.messageGet();
    std::vector<std::string> args = ServerCommand::commandArgsGet(message);
    std::cout << "-- TOPIC " << std::endl;

    if (this->commandIsValidTopic(message) && cli.statusGet() == 3)
    {
        std::string channel_name = Utils::trim(args[1]);
        std::string topic = args.size() == 3 ? Utils::trim(args[2]) : "";

        //std::cout << "-- TOPIC " << std::endl;

        Channel *ch = this->_server->channelsFindByName(channel_name);
        if (ch && !cli.channelsFindByName(channel_name))
        {
            message = ERR_NOTONCHANNEL(this->_server->nameGet(), ch->nameGet());
            //this->_server->sendx(cli.fdGet(), message.c_str()); 
        }
        
        if (ch 
            && cli.channelsFindByName(channel_name)
            && ch->moderatorsCan(cli, "o"))
        {
            if (args.size() == 2)
            {
                ch->topic(cli);
            }
            else
            {
                ch->topic(cli, topic);
            }
        }
    }
}