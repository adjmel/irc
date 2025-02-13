#include "../../../include/core/ServerCommand.hpp"
#include "../../../include/core/Server.hpp"
#include "../../../include/core/Client.hpp"
#include "../../../include/core/Channel.hpp"
#include "../../../include/core/Log.hpp"
#include "../../../include/core/Utils.hpp"
#include "../../../include/core/numeric_response.hpp"
#include <string>
#include <cstdlib>

bool ServerCommand::commandIsValidMode(std::string &command)
{
    std::vector<std::string> args = ServerCommand::commandArgsGet(command);
    
    if (args.empty()
        || args.size() < 3 
        || !this->commandIsValidName(args[0], "MODE"))
        return false;
    return true;
}

bool ServerCommand::commandIsValidModeOption(std::string mode, std::string option)
{
    return mode.find(option) != std::string::npos;
}

void ServerCommand::commandRunModeI(Client &cli)
{
    std::string message = cli.messageGet();
    std::vector<std::string> args = ServerCommand::commandArgsGet(message);
    std::cout << "-- MODE I" << std::endl;

    std::string channel_name = Utils::trim(args[1]);

    Channel *ch = this->_server->channelsFindByName(channel_name);
    if (ch && ch->moderatorsCan(cli, "o") && args.size() >= 3)
    {
        std::cout << "--mode" << args[2] << std::endl;
        ch->modeI(cli, args[2]);
    }
}

void ServerCommand::commandRunModeT(Client &cli)
{
    std::string message = cli.messageGet();
    std::vector<std::string> args = ServerCommand::commandArgsGet(message);
    std::cout << "-- MODE T" << std::endl;

    std::string channel_name = Utils::trim(args[1]);
    
    Channel *ch = this->_server->channelsFindByName(channel_name);
    if (ch && ch->moderatorsCan(cli, "o") && args.size() == 3)
    {
        ch->modeT(cli, args[2]);
    }
}

void ServerCommand::commandRunModeK(Client &cli)
{
    std::string message = cli.messageGet();
    std::vector<std::string> args = ServerCommand::commandArgsGet(message);
    std::cout << "-- MODE K" << std::endl;

    std::string channel_name = Utils::trim(args[1]);
    
    Channel *ch = this->_server->channelsFindByName(channel_name);
    if (ch && ch->moderatorsCan(cli, "o") && args.size() >= 3)
    {
        ch->modeAdd(args[2]);
        if (args.size() > 3)
        {
            std::string password = Utils::trim(args[3]);
            ch->modeK(cli, args[2], password);
        }
        else
        {
            ch->modeK(cli, args[2], ch->passwordGet());
        }
    }
}

void ServerCommand::commandRunModeO(Client &cli)
{
    std::string message = cli.messageGet();
    std::vector<std::string> args = ServerCommand::commandArgsGet(message);
    std::cout << "-- MODE O" << std::endl;   

    std::string channel_name = Utils::trim(args[1]);
    
    Channel *ch = this->_server->channelsFindByName(channel_name);
    if (ch && ch->moderatorsCan(cli, "o") && args.size() > 3)
    {
        std::string nick = Utils::trim(args[3]);
        Client* c = this->_server->clientsFindByNick(nick);
        if (c)
        {
            ch->modeO(cli, args[2], *c);
        }
    }
}

void ServerCommand::commandRunModeL(Client &cli)
{
    std::string message = cli.messageGet();
    std::vector<std::string> args = ServerCommand::commandArgsGet(message);
    std::cout << "-- MODE L" << std::endl;

    std::string channel_name = Utils::trim(args[1]);
    
    Channel *ch = this->_server->channelsFindByName(channel_name);
    if (ch && ch->moderatorsCan(cli, "o") && args.size() >= 3)
    {
        if (args.size() > 3)
        {
            ch->modeL(cli, args[2], std::atoi(args[3].c_str()));
        }
        else
        {
            ch->modeL(cli, args[2]);
        }
    }
}

void ServerCommand::commandRunMode(Client &cli)
{
    std::string message = cli.messageGet();
    std::vector<std::string> args = ServerCommand::commandArgsGet(message);
    std::cout << "-- MODE " << std::endl;

    if (this->commandIsValidMode(message) && cli.statusGet() == 3)
    {
        std::string channel_name = Utils::trim(args[1]);

        Channel *ch = this->_server->channelsFindByName(channel_name);

        if (ch && !cli.channelsFindByName(channel_name))
        {
            message = ERR_NOTONCHANNEL(this->_server->nameGet(), ch->nameGet());
            //this->_server->sendx(cli.fdGet(), message.c_str()); 
        }
        
        if (ch && cli.channelsFindByName(channel_name)
            && ch->moderatorsCan(cli, "o"))
        {
            if (ServerCommand::commandIsValidModeOption(Utils::trim(args[2]), "i"))
                this->commandRunModeI(cli);
            else if (ServerCommand::commandIsValidModeOption(Utils::trim(args[2]), "t"))
                this->commandRunModeT(cli);
            else if (ServerCommand::commandIsValidModeOption(Utils::trim(args[2]), "k"))
                this->commandRunModeK(cli);
            else if (ServerCommand::commandIsValidModeOption(Utils::trim(args[2]), "o"))
                this->commandRunModeO(cli);
            else if (ServerCommand::commandIsValidModeOption(Utils::trim(args[2]), "l"))
                this->commandRunModeL(cli);
        }
    }
}