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

int Channel::_ID = 0;

Channel::Channel(Server *server) : 
  _topic(""), _clients_max(0), _mode("")
{
    Channel::_ID++;
    this->_name = Utils::nameGenerateUnique("#channel-", Utils::intToString(Channel::_ID));
    this->_server = server;
    this->_log = new Log("channel.", "logs");
    this->_log->append("The Channel " + this->_name + " has been created.", 0);
    this->_bot = new Bot("assets/bot.txt");
}

Channel::Channel(Server *server, Client &client) : 
  _topic(""), _clients_max(0), _mode("")
{
    Channel::_ID++;
    this->_name = Utils::nameGenerateUnique("#channel-", Utils::intToString(Channel::_ID));
    this->_server = server;
    //client.modeAdd("+i+t+k+o+l");
    this->moderatorsAdd(client);
    this->moderatorsModeUpdate(client, "+i+t+k+o+l");
    this->_clients.push_back(&client);
    client.channelsAdd(this);
    this->_log = new Log("channel.", "logs");
    this->_log->append("The Channel " + this->_name + " has been created.", 0);
    this->_bot = new Bot("assets/bot.txt");
}

Channel::~Channel()
{

    for (std::vector<Moderator*>::iterator it = this->_moderators.begin(); it != this->_moderators.end(); ++it) 
    {
        delete *it;
        *it = NULL;
    }
    if (this->_log != NULL)
        delete this->_log;
    if (this->_bot != NULL)
        delete this->_bot;
}

Log *Channel::logGet()
{
    return this->_log;
}

Bot *Channel::botGet()
{
    return this->_bot; 
}

std::string Channel::nameGet()
{
    return this->_name;
}

void Channel::nameSet(std::string name)
{
    this->_name = name;
}

std::string Channel::topicGet()
{
    return this->_topic;
}

void Channel::topicSet(std::string topic)
{
    this->_topic = topic;
}

std::string Channel::passwordGet()
{
    return this->_password;
}

void Channel::passwordSet(std::string password)
{
    this->_password = password;
}

// -- MODERTORS

std::vector<Moderator*> &Channel::moderatorsGet()
{
    return this->_moderators;
}

void Channel::moderatorsAdd(Client &cli)
{
    std::vector<Moderator*>::iterator it;
    Moderator *m = NULL;
    for (it = this->_moderators.begin(); it != this->_moderators.end(); ++it)
    {
        if ((*it)->id == cli.idGet())
        {
            m = *it;
        }
    }

    if (m != NULL)
        return;

    m = new Moderator;
    m->id = cli.idGet();
    m->mode = "+o";
    this->_moderators.push_back(m);
}

void Channel::moderatorsModeUpdate(Client &cli, std::string mode)
{
    std::vector<Moderator*>::iterator it;
    Moderator *m = NULL;
    for (it = this->_moderators.begin(); it != this->_moderators.end(); ++it)
    {
        if ((*it) && (*it)->id == cli.idGet())
        {
            m = *it;
        }
    }

    if (m == NULL)
        return;

    const char* validSubstrings[] = {"+i", "+t", "+k", "+o", "+l"};
    const std::size_t numValidSubstrings = sizeof(validSubstrings) / sizeof(validSubstrings[0]);

    for (std::size_t i = 0; i < numValidSubstrings; ++i) 
    {
        if (mode.find(validSubstrings[i]) != std::string::npos) 
        {
            // Vérifiez si le mode existe déjà avant de l'ajouter
            if (m->mode.find(validSubstrings[i]) == std::string::npos)
                m->mode.append(validSubstrings[i]);
        }
    }

    const char* validSubstringsRem[] = {"-i", "-t", "-k", "-o", "-l"};
    const std::size_t numValidSubstringsRem = sizeof(validSubstringsRem) / sizeof(validSubstringsRem[0]);

    for (std::size_t i = 0; i < numValidSubstringsRem; ++i) 
    {
        if (mode.find(validSubstringsRem[i]) != std::string::npos) 
        {
            // Construire le mode correspondant à ajouter (+i, +t, +k, +o, +l)
            std::string correspondingAddMode = "+" + std::string(validSubstringsRem[i]).substr(1);

            // Si le mode correspondant à ajouter est présent dans _mode, le supprimer
            std::size_t pos = m->mode.find(correspondingAddMode);
            if (pos != std::string::npos)
            {
                m->mode.erase(pos, correspondingAddMode.length());
            }
        }
    }
}

bool Channel::moderatorsCan(Client &cli, std::string mode)
{
    std::vector<Moderator*>::iterator it;
    Moderator *m = NULL;
    for (it = this->_moderators.begin(); it != this->_moderators.end(); ++it)
    {
        if ((*it) && (*it)->id == cli.idGet())
        {
            m = *it;
            break;
        }
    }

    if (m == NULL)
        return false;

    return m->mode.find(mode) != std::string::npos;
}

void Channel::moderatorsRemove(int id)
{
    for (std::vector<Moderator*>::iterator it = this->_moderators.begin(); it != this->_moderators.end();)
    {
        if ((*it)->id == id)
        {
            delete *it; // Libère la mémoire de l'objet Moderator
            it = this->_moderators.erase(it); // Efface l'élément du vecteur et met à jour l'itérateur
            break; // Sort de la boucle
        }
        else
        {
            ++it; // Passe à l'élément suivant si l'élément actuel ne correspond pas à l'ID
        }
    }
}

void Channel::moderatorsAddIfNeed()
{   
    if (this->_clients.size() != 1)
        return ;

    if (this->_moderators.size() != 0)
        return ;

    Client *c = NULL;
    for (std::vector<Client*>::iterator it = this->_clients.begin(); it != this->_clients.end();) 
    {
        c = *it;
        break ;
    }

    if (c)
    {
        if (this->moderatorsCan(*c, "+o"))
            return ;
        this->moderatorsAdd(*c);
        this->moderatorsModeUpdate(*c, "+i+t+k+o+l");
    }
}

// --

// -- CLIENTS
int Channel::clientsMaxGet()
{
    return this->_clients_max;
}

void Channel::clientsMaxSet(int clientsMax)
{
    this->_clients_max = clientsMax;
}
bool Channel::clientsMaxIsMax()
{
    return (int)this->_clients.size() >= this->_clients_max;
}

Server *Channel::serverGet()
{
    return this->_server;
}

void Channel::serverSet(Server *server)
{
    this->_server = server;
}

std::vector<Client*> &Channel::clientsGet()
{
    return this->_clients;
}

bool Channel::clientsBanIs(Client &cli)
{
    std::vector<Client*>::iterator it;
    for (it = _clients_ban.begin(); it != _clients_ban.end(); ++it)
    {
        if (*it == &cli)
        {
            return 1;
        }
    }
    return 0;
}

void Channel::clientsBanRemove(Client &cli)
{
    std::vector<Client*>::iterator it;
    for (it = _clients_ban.begin(); it != _clients_ban.end(); ++it)
    {
        if (*it == &cli)
            break ;
    }

    if (it != _clients_ban.end()) 
    {
        // Le client est présent dans le vecteur, on le supprime
        _clients_ban.erase(it);
    }
}

bool Channel::clientsHas(Client &cli)
{
    std::vector<Client*>::iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (*it == &cli)
        {
            return 1;
        }
    }
    return 0;
}

Client* Channel::clientsFindByNick(std::string& nick) 
{
    for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if ((*it)->nickGet() == nick) {
            return *it;
        }
    }
    return NULL; // Retourne NULL si le pseudonyme n'est pas trouvé
}

void Channel::clientsRemove(int id) 
{
    Client *c = NULL;

    for (std::vector<Client*>::iterator it = this->_clients.begin(); it != this->_clients.end();) 
    {
        if ((*it)->idGet() == id) 
        {
            c = *it;
            it = this->_clients.erase(it);
            break;
        } 
        else 
        {
            ++it;
        }
    }

    if (c)
    {
        this->moderatorsRemove(c->idGet());
        c->channelsDeleteByName(this->_name);
        this->moderatorsAddIfNeed();
    }
}

void Channel::clientsInvitedAdd(Client &cli)
{   
    if(!this->clientsInvitedHas(cli))
    {
        //std::cout << "Add ..." << std::endl;
        this->_clients_invited.push_back(&cli);
    }
}

bool Channel::clientsInvitedHas(Client &cli)
{
    std::vector<Client*>::iterator it;
    for (it = _clients_invited.begin(); it != _clients_invited.end(); ++it)
    {
        if (*it == &cli)
        {
            return 1;
        }
    }
    return 0;
}

bool Channel::clientsInvitedConfirm(Client &cli)
{
    std::vector<Client*>::iterator it;
    for (it = _clients_invited.begin(); it != _clients_invited.end(); ++it)
    {
        if (*it == &cli)
        {
            this->_clients.push_back(&cli);
            cli.channelsAdd(this);
            cli.channelRecentSet(this);
            it = this->_clients_invited.erase(it);
            return 1;
        }
    }
    return 0;
}

// --

void Channel::modeSet(std::string mode)
{
    this->_mode = mode;
}

std::string Channel::modeGet()
{
    return this->_mode;
}

void Channel::modeAdd(std::string mode)
{
    const char* validSubstrings[] = {"+i", "+t", "+k", "+o", "+l"};
    const std::size_t numValidSubstrings = sizeof(validSubstrings) / sizeof(validSubstrings[0]);

    for (std::size_t i = 0; i < numValidSubstrings; ++i) 
    {
        if (mode.find(validSubstrings[i]) != std::string::npos) 
        {
            // Vérifiez si le mode existe déjà avant de l'ajouter
            if (this->_mode.find(validSubstrings[i]) == std::string::npos)
                this->_mode.append(validSubstrings[i]);
        }
    }

    const char* validSubstringsRem[] = {"-i", "-t", "-k", "-o", "-l"};
    const std::size_t numValidSubstringsRem = sizeof(validSubstringsRem) / sizeof(validSubstringsRem[0]);

    for (std::size_t i = 0; i < numValidSubstringsRem; ++i) 
    {
        if (mode.find(validSubstringsRem[i]) != std::string::npos) 
        {
            // Construire le mode correspondant à ajouter (+i, +t, +k, +o, +l)
            std::string correspondingAddMode = "+" + std::string(validSubstringsRem[i]).substr(1);

            // Si le mode correspondant à ajouter est présent dans _mode, le supprimer
            std::size_t pos = this->_mode.find(correspondingAddMode);
            if (pos != std::string::npos)
            {
                this->_mode.erase(pos, correspondingAddMode.length());
            }
        }
    }
}

bool Channel::modeCan(std::string mode)
{
    return _mode.find(mode) != std::string::npos;
}

// -- CMD


// --- CMD

int Channel::restore(Client &cli, Channel *channel)
{
    this->_log->append("RESTORE", 0);

    struct RestoreData data;
    data.cli = &cli;
    data.cha = channel;

    //passer status comme cli pour verifier fail ou pas
    this->_log->restore("1,2,3", &Channel::restoreCallback, &data);
    return 0;
}

bool Channel::restoreIsValid(Client &cli, std::string line)
{
    std::vector<std::string> strs = Utils::split(line, ":");
    if (!strs.empty() && strs.size() >= 3)
    {
        strs = Utils::extract(strs[1]);
        //std::cout << "restore: strs[0] = " << strs[0] << std::endl;
        //std::cout << "restore: strs[1] = " << strs[1] << std::endl;

        std::vector<std::string> user = Utils::split(strs[0], " ");
        //std::cout << "restore: user[0] = " << user[0] << std::endl;
        //std::cout << "restore: strs[1] = " << user[1] << std::endl;
        std::string id_str = Utils::trim(user[0]);
        std::string nick = Utils::trim(user[1]);
        int id_1 = std::atoi(id_str.c_str());
        //std::cout << "restore: id = " << id_1 << " nick = " << nick << std::endl;

        strs[1] = Utils::trim(strs[1]);
        user = Utils::split(strs[1], " ");
        //std::cout << "restore: user[0] = " << user[0] << std::endl;
        //std::cout << "restore: strs[1] = " << user[1] << std::endl;
        id_str = Utils::trim(user[0]);
        nick = Utils::trim(user[1]);
        int id_2 = std::atoi(id_str.c_str());
        //std::cout << "restore: id = " << id_2 << " nick = " << nick << std::endl;
        if (id_1 == cli.idGet() || id_2 == cli.idGet())
            return true;
    }
    return false;
}

void Channel::restoreCallback(const std::string& line, void *arg)
{
    struct RestoreData *data = static_cast<struct RestoreData*>(arg);
    Client* client = data->cli;
    Channel* channel = data->cha;

    if (client)
    {
        Server *server = channel->serverGet();
        if (Log::lineIsValidType(line, "2") 
            && !Channel::restoreIsValid(*client, line))
            return ;
        std::string l = line;
        if (server)
        {
            //std::cout << " -- line " << l << std::endl;
            std::vector<std::string> ls = Utils::split(l, ":");
            l = Utils::joinExceptFirstN(ls, 2);
            l = Utils::trim(l);

            //std::cout << ls[1] << std::endl;
            ls[1] = Utils::trim(ls[1]);
            ls = Utils::extract(ls[1]); // remove [] and slipt ,
            ls = Utils::split(ls[0], " ");
            std::string nick = Utils::trim(ls[1]);
            
            //std::cout << nick << std::endl;
            
            l = RPL_PRIVMSG(nick, channel->nameGet(), l);
            server->sendx(client->fdGet(), l.c_str());
        }
        //Log log("tmp-restore","logs");
        // std::string tmp = "channel: " + channel->nameGet() + " " + line;
        // channel->logGet()->append(tmp, 0);
        //std::cout << "restore: " << client->idGet() << " " << l ;
    }
}

std::ostream &operator<<(std::ostream &out, const Channel &channel)
{
    out << "Channel [";
    out << "Name: " << channel._name << ", ";
    out << "Topic: " << channel._topic << ", ";
    out << "Password: " << channel._password << ", ";
    out << "Clients Max.: " << channel._clients_max << ", ";
    out << "Mode: " << channel._mode << ", ";
    out << "]";
    return out;
}




