#include "../../../include/core/ServerCommand.hpp"
#include "../../../include/core/Server.hpp"
#include "../../../include/core/Client.hpp"
#include "../../../include/core/Channel.hpp"
#include "../../../include/core/Log.hpp"
#include "../../../include/core/Bot.hpp"
#include "../../../include/core/Utils.hpp"
#include "../../../include/core/numeric_response.hpp"
#include <string>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <algorithm>

bool ServerCommand::commandIsValidPrivMsg(std::string &command)
{
    std::vector<std::string> args = ServerCommand::commandArgsGetExecptLast(command, 2);
    
    // if (!args.empty() 
    //     && args.size() == 8
    //     && this->commandIsValidName(args[2], ":DCC")
    //     && this->commandIsValidName(args[3], "SEND"))
    //     return false;

    if (args.empty() 
        || args.size() < 3
        || !this->commandIsValidName(args[0], "PRIVMSG"))
        return false;
    return true;
}

void ServerCommand::commandRunPrivMsgSend(Client &from, Client &to, std::string message)
{
    this->_server->logGet()->append("PRIVMSG", 0);

    //std::cout << "++ PRIVMSG " << std::endl;
    std::string out = message;
    if (from.irssiIs())
    {
        if (!out.empty() && out[0] == ':')
            out.erase(0, 1);
    }
        
    int type = 2;
    if (this->_server->botGet()->dataHas("bad-word", out))
    {
        out = this->_server->botGet()->answer(out);
        std::string tmp = "[" + Utils::intToString(from.idGet())  + " " + from.nickGet() +  "] : " + out;
        this->_server->botGet()->logGet()->append(tmp, type);
    }

    //std::cout << "++ PRIVMSG " << std::endl;
    std::string tmp = "[" + Utils::intToString(from.idGet()) + " " + from.nickGet() + ", " 
        +  Utils::intToString(to.idGet()) + " " + to.nickGet() + "] : " + out;
    if (this->_server)
    {
        out = RPL_PRIVMSG(from.nickGet(), to.nickGet(), out);
        this->_server->logGet()->append(out, 0);
        this->_server->sendx(to.fdGet(), out.c_str()); 
    }
    this->_server->logGet()->append(tmp, type);

}

void ServerCommand::commandRunPrivMsg(Client &cli)
{
    std::string message = cli.messageGet();
    std::vector<std::string> args = ServerCommand::commandArgsGetExecptLast(message, 2);
    std::cout << "-- PRIVMSG " << std::endl;

    if (this->commandIsValidPrivMsg(message) && cli.statusGet() == 3)
    {
        std::string name = Utils::trim(args[1]);
        std::string msg = Utils::trim(args[2]);

        Channel *ch = this->_server->channelsFindByName(name);
        Client *c = this->_server->clientsFindByNick(name);

        if (!ch && !c)
        {
            message = ERR_NOSUCHNICK(this->_server->nameGet(), name);
            this->_server->logGet()->append(message, 0);
            //this->_server->sendx(cli.fdGet(), message.c_str()); 
        }

        if (ch && !c
            && ch->clientsHas(cli))
        {
            //std::cout << "-- ALL IN CHANNEL " << std::endl;
            ch->privmsg(cli, msg);
        }
        if (c)
        {
            this->commandRunPrivMsgSend(cli, *c, msg);
            // //std::cout << "-- PRIVATE TO ONE " << std::endl;
            // if (cli.channelsGet().size())
            // {
            //     ch = cli.channelsGet()[0];
            //     ch->privmsg(cli, *c, msg);
            // }
        }
        
    }
}