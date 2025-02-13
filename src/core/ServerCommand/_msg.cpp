#include "../../../include/core/ServerCommand.hpp"
#include "../../../include/core/Server.hpp"
#include "../../../include/core/Client.hpp"
#include "../../../include/core/Channel.hpp"
#include "../../../include/core/Log.hpp"
#include "../../../include/core/Bot.hpp"
#include "../../../include/core/Utils.hpp"
#include "../../../include/core/numeric_response.hpp"
#include <string>
#include <cstdlib>

bool ServerCommand::commandIsValidMsg(std::string &command)
{
    std::vector<std::string> args = ServerCommand::commandArgsGetExecptLast(command, 2);
    
    // if (!args.empty() 
    //     && args.size() == 8
    //     && this->commandIsValidName(args[2], ":DCC")
    //     && this->commandIsValidName(args[3], "SEND"))
    //     return false;

    if (args.empty() 
        || args.size() < 3
        || !this->commandIsValidName(args[0], "MSG"))
        return false;
    return true;
}


int ServerCommand::commandRunMsgSend(Client &from, Client &to, std::string message)
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

    return 0;
}

void ServerCommand::commandRunMsg(Client &cli)
{
    std::string message = cli.messageGet();
    std::vector<std::string> args = ServerCommand::commandArgsGetExecptLast(message, 2);
    std::cout << "-- MSG " << std::endl;

    if (this->commandIsValidMsg(message) && cli.statusGet() == 3)
    {
        std::string name = Utils::trim(args[1]);
        std::string msg = Utils::trim(args[2]);

        Client *c = this->_server->clientsFindByNick(name);

        if (c)
        {
            this->commandRunMsgSend(cli, *c, msg);
        }
        
    }
}