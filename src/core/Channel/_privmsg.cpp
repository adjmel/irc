#include "../../../include/core/Channel.hpp"
#include "../../../include/core/Log.hpp"
#include "../../../include/core/Server.hpp"
#include "../../../include/core/Client.hpp"
#include "../../../include/core/Bot.hpp"
#include "../../../include/core/Utils.hpp"
#include "../../../include/core/numeric_response.hpp"
#include <unistd.h>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <ctime>
#include <sstream>
#include <sys/time.h>
#include <cstdlib>

int Channel::privmsg(Client &from, std::string message)
{
    this->_log->append("PRIVMSG", 0);

    std::string out = message;

    if (from.irssiIs())
    {
        if (!out.empty() && out[0] == ':')
            out.erase(0, 1);
    }

    int type = 1;
    if (this->_bot->dataHas("bad-word", out))
    {
        out = this->_bot->answer(out);
        std::string tmp = "[" + Utils::intToString(from.idGet()) + "] : " + out;
        this->_bot->logGet()->append(tmp, type);
    }

    std::string tmp = "[" + Utils::intToString(from.idGet()) + " " + from.nickGet() + "] : " + out;
    
    std::vector<Client*>::iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (this->_server && *it != &from)
        {
            std::string out_one = RPL_PRIVMSG(from.nickGet(), this->_name, out);
            this->_server->logGet()->append(out_one, 1);
            this->_server->sendx((*it)->fdGet(), out_one.c_str()); 
        }
    }
    
    this->_log->append(tmp, type);
    
    return 0;
}

int Channel::privmsg(Client &from, Client &to, std::string message)
{
    this->_log->append("PRIVMSG", 0);

    //std::cout << "++ PRIVMSG " << std::endl;
    std::string out = message;
    if (from.irssiIs())
    {
        if (!out.empty() && out[0] == ':')
            out.erase(0, 1);
    }
        
    int type = 2;
    if (this->_bot->dataHas("bad-word", out))
    {
        out = this->_bot->answer(out);
        std::string tmp = "[" + Utils::intToString(from.idGet())  + " " + from.nickGet() +  "] : " + out;
        this->_bot->logGet()->append(tmp, type);
    }

    //std::cout << "++ PRIVMSG " << std::endl;
    std::string tmp = "[" + Utils::intToString(from.idGet()) + " " + from.nickGet() + ", " 
        +  Utils::intToString(to.idGet()) + " " + to.nickGet() + "] : " + out;
    if (this->_server)
    {
        out = RPL_PRIVMSG(from.nickGet(), this->_name, out);
        this->_server->logGet()->append(out, 0);
        this->_server->sendx(to.fdGet(), out.c_str()); 
    }
    this->_log->append(tmp, type);

    return 0;
}