#include "../../../include/core/ServerCommand.hpp"
#include "../../../include/core/Server.hpp"
#include "../../../include/core/Client.hpp"
#include "../../../include/core/Log.hpp"
#include "../../../include/core/Utils.hpp"
#include "../../../include/core/numeric_response.hpp"
#include <string>

bool ServerCommand::commandIsValidPass(std::string &command)
{
    std::vector<std::string> args = ServerCommand::commandArgsGet(command);
    
    if (args.empty() 
        || args.size() < 2
        || !this->commandIsValidName(args[0], "PASS"))
        return false;
    return true;
}

void ServerCommand::commandRunPass(Client &cli)
{
    std::string message = cli.messageGet();
    std::vector<std::string> args = ServerCommand::commandArgsGet(message);
    std::cout << "-- PASS " << std::endl;

    if (this->commandIsValidPass(message) && cli.statusGet() == 0)
    {
        if (this->_server->passwordGet() == args[1])
        {
            cli.statusSet(1);

            if (cli.irssiIs())
            {
                cli.statusSet(3);
                std::string nick = " ";
                message = RPL_NICK(nick, cli.nickGet());
                this->_server->sendx(cli.fdGet(), message.c_str());
                message = cli.nickGet();
                message = RPL_WELCOME(message, cli.nickGet());
                this->_server->sendx(cli.fdGet(), message.c_str());
                return ;
            }
               
            //message = ": NICK :alpha\n:alpha!fsundare@0.0.0.0 001 alpha :Welcome to the Internet Relay Network alpha!fsundare@0.0.0.0\n:alpha!fsundare@0.0.0.0 002 alpha :Your host is 0.0.0.0, running version 1.69\n:alpha!fsundare@0.0.0.0 003 alpha :This server was created Thu Jan 25 19:31:21 2024\n:alpha!fsundare@0.0.0.0 004 alpha ShibRC 1.69 aiwro Oovimnptkl\n:alpha!fsundare@0.0.0.0 251 alpha :There are 1 users and 0 invisible on 1 servers\n:alpha!fsundare@0.0.0.0 252 alpha 0 :operator(s) online\n:alpha!fsundare@0.0.0.0 253 alpha 0 :unknown connection(s)\n:alpha!fsundare@0.0.0.0 254 alpha 0 :channels formed\n:alpha!fsundare@0.0.0.0 255 alpha :I have 1 clients and 0 servers\n:alpha!fsundare@0.0.0.0 422 alpha :MOTD File is missing";

            
//                         message = ": NICK :alpha\n";
// message =+ ":alpha!fsundare@0.0.0.0 001 alpha :Welcome to the Internet Relay Network alpha!fsundare@0.0.0.0\n";
// message =+ ":alpha!fsundare@0.0.0.0 002 alpha :Your host is 0.0.0.0, running version 1.69\n";
// message =+ ":alpha!fsundare@0.0.0.0 003 alpha :This server was created Thu Jan 25 19:31:21 2024\n";
// message =+ ":alpha!fsundare@0.0.0.0 004 alpha ShibRC 1.69 aiwro Oovimnptkl\n";
// message =+ ":alpha!fsundare@0.0.0.0 251 alpha :There are 1 users and 0 invisible on 1 servers\n";
// message =+ ":alpha!fsundare@0.0.0.0 252 alpha 0 :operator(s) online\n";
// message =+ ":alpha!fsundare@0.0.0.0 253 alpha 0 :unknown connection(s)\n";
// message =+ ":alpha!fsundare@0.0.0.0 254 alpha 0 :channels formed\n";
// message =+ ":alpha!fsundare@0.0.0.0 255 alpha :I have 1 clients and 0 servers\n";
// message =+ ":alpha!fsundare@0.0.0.0 422 alpha :MOTD File is missing";
            
            //this->_server->sendx(cli.fdGet(), message.c_str());
            // message = "hello\n";
            // this->_server->sendx(cli.fdGet(), message.c_str());
        }
        else
        {
            message = ERR_PASSWDMISMATCH(this->_server->nameGet());
            this->_server->logGet()->append(message, 0);
            //this->_server->sendx(cli.fdGet(), message.c_str());
        }
    }   
}