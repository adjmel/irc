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

int Channel::bot(Client &from, Bot &to, std::string message)
{
    this->_log->append("MSG", 0);
    
    std::string out = message;
    int type = 3;
    if (this->_bot->dataHas("bad-word", out))
    {
        out = this->_bot->answer(out);
        std::string tmp = "[" + Utils::intToString(from.idGet()) + ", " + this->_bot->nameGet() + "] : " + out;
        this->_bot->logGet()->append(tmp, type);
        //return 1;
    }
    else
    {
        out = to.answer(message);
        std::string tmp = "[" + Utils::intToString(from.idGet()) + ", " + this->_bot->nameGet() + "] : " + out;
        this->_bot->logGet()->append(tmp, type);
    }

    // out = "[" + Utils::intToString(from.idGet()) + ", " + this->_bot->nameGet() + "] : " + message;
    // this->_log->append(out, type);
    // out = to.answer(message);
    // out = "[" + Utils::intToString(from.idGet()) + ", " + this->_bot->nameGet() + "] : " + out;
    // if (this->_server)
    //     this->_server->sendx(from.fdGet(), out.c_str());
    // this->_log->append(out, type);
    // this->_bot->logGet()->append(out, type);

    if (this->_server)
    {
        message = RPL_PRIVMSG(from.nickGet(), this->_name, out);
        this->_server->logGet()->append(message, 0);
        this->_server->sendx(from.fdGet(), message.c_str()); 
    }
    this->_log->append(message, 0);
    return 0;
}