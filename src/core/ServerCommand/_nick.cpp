#include "../../../include/core/ServerCommand.hpp"
#include "../../../include/core/Server.hpp"
#include "../../../include/core/Client.hpp"
#include "../../../include/core/Utils.hpp"
#include "../../../include/core/Log.hpp"
#include "../../../include/core/numeric_response.hpp"
#include <string>

bool ServerCommand::commandIsValidNick(std::string &command)
{
    std::vector<std::string> args = ServerCommand::commandArgsGet(command);
    
    if (args.empty() 
        || args.size() != 2
        || !this->commandIsValidName(args[0], "NICK"))
        return false;
    return true;
}

void ServerCommand::commandRunNick(Client &cli)
{
    std::string message = cli.messageGet();
    std::vector<std::string> args = ServerCommand::commandArgsGet(message);
    std::cout << "-- NICK " << std::endl;
   
    std::string nick = Utils::trim(args[1]);
    if (this->commandIsValidNick(message) && cli.statusGet() >= 1)
    {
        if (Client::nickIsValid(nick))
        {
            Client *c = this->_server->clientsFindByNick(nick);
            if (!c)
            {
                if (cli.statusGet() == 1)
                    cli.statusSet(2);
                std::string nick_previous = cli.nickGet();
                cli.nickSet(nick);
                if (nick_previous.empty())
                {
                    //std::cout << "--- NICK " << nick_previous << std::endl; 
                    message = RPL_NICK(this->_server->nameGet(), nick);
                }
                else
                {
                    //std::cout << "+++ NICK " << nick_previous << std::endl; 
                    message = RPL_NICK(nick_previous, nick);
                }
                this->_server->sendx(cli.fdGet(), message.c_str());
            }
            else if (c->idGet() != cli.idGet())
            {
                message = ERR_NICKNAMEINUSE(this->_server->nameGet(), nick);
                this->_server->logGet()->append(message, 0);

                //cli.statusSet(0);
                //this->_server->sendx(cli.fdGet(), message.c_str());
                //this->_server->clientsRemove(cli.fdGet());

                //c->statusSet(0);
                //this->_server->sendx(c->fdGet(), message.c_str());
                //this->_server->clientsRemove(c->fdGet());
            }
        }
        // else
        // {
        //     message = ERR_NONICKNAMEGIVEN(this->_server->nameGet());
        //     this->_server->logGet()->append(message, 0);
        //     this->_server->sendx(cli.fdGet(), message.c_str());
        // }
    }
}