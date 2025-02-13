#include "../../include/core/Client.hpp"
#include "../../include/core/Channel.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <ctime>

int Client::_ID = 0;

Client::Client() : _fd(0), _status(0), _mode(""),
    _channel_recent(NULL), _admin_is(false), _irssi_is(false)
{
    Client::_ID++;
    this->_id = Client::_ID;
}

Client::Client(int fd) : _fd(fd), _status(0), _mode(""),
    _channel_recent(NULL), _admin_is(false), _irssi_is(false)
{
    Client::_ID++;
    this->_id = Client::_ID;
}

Client::~Client() {}

int Client::idGet()
{
    return this->_id;
}

int Client::fdGet()
{
    return this->_fd;
}

void Client::statusSet(int status)
{
    this->_status = status;
}

int Client::statusGet()
{
    return this->_status;
}

void Client::usernameSet(std::string username)
{
    this->_username = username;
}

std::string Client::usernameGet()
{
    return this->_username;
}

void Client::nickSet(std::string nick)
{
    this->_nick = nick;
}

std::string Client::nickGet()
{
    return this->_nick;
}

bool Client::nickIsValid(std::string& nick)
{
    const char underscore = '_';
    const char hyphen = '-';
    const size_t maxNickLength = 9;

    if (nick.length() > maxNickLength) {
        return false;
    }

    for (size_t i = 0; i < nick.length(); ++i) {
        if (!(std::isalnum(nick[i]) || nick[i] == underscore || nick[i] == hyphen)) {
            return false;
        }
    }

    return true;
}

std::string Client::nickGenerateUnique()
{
    std::time_t now = std::time(NULL); // Obtient le temps actuel en secondes depuis l'époque

    // Génération du nom unique avec le timestamp en secondes
    std::stringstream ss;
    ss << "n" << now;

    return ss.str();
}

void Client::nameRealSet(std::string nameReal)
{
    this->_name_real = nameReal;
}

std::string Client::nameRealGet()
{
    return this->_name_real;
}

void Client::unusedSet(std::string unused)
{
    this->_unused = unused;
}

std::string Client::unusedGet()
{
    return this->_unused;
}

void Client::servernameSet(std::string servername)
{
    this->_servername = servername;
}

std::string Client::servernameGet()
{
    return this->_servername;
}

void Client::hostnameSet(std::string hostname)
{
    this->_hostname = hostname;
}

std::string Client::hostnameGet()
{
    return this->_hostname;
}

void Client::passwordSet(std::string password)
{
    this->_password = password;
}

std::string Client::passwordGet()
{
    return this->_password;
}

void Client::modeSet(std::string mode)
{
    this->_mode = mode;
}

std::string Client::modeGet()
{
    return this->_mode;
}

void Client::modeAdd(std::string mode)
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

bool Client::modeCan(std::string mode)
{
    return _mode.find(mode) != std::string::npos;
}

void Client::messageSet(std::string message)
{
    this->_message = message;
}

std::string Client::messageGet()
{
    return this->_message;
}

void Client::channelsAdd(Channel* channel) 
{
    // Vérifier si le canal n'existe pas déjà
    if (channelsFindByName(channel->nameGet()) == NULL)
        _channels.push_back(channel);
}

std::vector<Channel*>& Client::channelsGet() 
{
    return _channels;
}

Channel* Client::channelsFindByName(std::string name) 
{
      for (size_t i = 0; i < _channels.size(); ++i) 
      {
        if (_channels[i]->nameGet() == name) {
            return _channels[i];
        }
    }
    return NULL;
}

bool Client::channelsDeleteAll() 
{
    // Effacer tous les éléments du vecteur

    for (size_t i = 0; i < _channels.size(); ++i) 
    {
        _channels[i]->clientsRemove(this->_id);
    }
    
    _channels.clear();

    std::cout << "Tous les canaux ont été supprimés." << std::endl;
    return true;
}

bool Client::channelsDeleteByName(std::string name) 
{
    for (std::vector<Channel*>::iterator it = this->_channels.begin(); it != this->_channels.end();) 
    {
        if ((*it)->nameGet() == name) 
        {
            it = this->_channels.erase(it);  // Update iterator to the next valid position
            break;  // To prevent iterate after erase
        } 
        else 
        {
            ++it;  // Increment the iterator only if no erasure occurs
        }
    }
    
    // size_t _numChannels = _channels.size();

    // for (size_t i = 0; i < _numChannels; ++i) {
    //     if (_channels[i]->nameGet() == name) {
    //         // Supprimer le canal trouvé
    //         //delete _channels[i];

    //         // Décaler les canaux restants dans le vecteur
    //         for (size_t j = i; j < _numChannels - 1; ++j) {
    //             _channels[j] = _channels[j + 1];
    //         }

    //         // Réduire la taille du vecteur
    //         _channels.resize(_numChannels - 1);

    //         std::cout << "Canal supprimé : " << name << std::endl;
    //         return true; // Canal supprimé avec succès
    //     }
    // }

    std::cout << "Le canal n'a pas été trouvé : " << name << std::endl;
    return false; // Le canal n'a pas été trouvé
}

void Client::channelRecentSet(Channel *ch)
{
    this->_channel_recent = ch;
}

bool Client::channelRecentIs(Channel *ch)
{
    return this->_channel_recent == ch;
}

void Client::adminSet(bool admin_is)
{
    this->_admin_is = admin_is;
}

bool Client::adminIs()
{
    return this->_admin_is;
}

void Client::irssi(std::string message)
{
    std::istringstream iss(message);
    std::string command;
    
    while (iss >> command) 
    {
        if (command == "NICK") 
        {
            iss >> this->_nick;
        } 
        else if (command == "USER") 
        {
            iss >> this->_username >> this->_hostname >> this->_servername;
            std::string name;
            iss >> name; // frist name
            this->_name_real = name;
            iss >> name; // last name
            this->_name_real = this->_name_real + " " + name;
            // suprimmer le :
            if (!this->_name_real.empty() && this->_name_real[0] == ':') 
            {
                this->_name_real = this->_name_real.substr(1);
            }
        } 
        else if (command == "CAP" && (iss >> command) && command == "LS") 
        {

        }
    }
    this->_irssi_is = true;
}

bool Client::irssiIs()
{
    return this->_irssi_is;
}

std::string Client::toString()
{
    std::ostringstream oss;
    oss << *this;
    return oss.str();
}

std::ostream &operator<<(std::ostream &out, const Client &client)
{
    out << "Client [";
    out << "FD: " << client._fd << ", ";
    out << "Status: " << client._status << ", ";
    out << "Nick: " << client._nick << ", ";
    out << "Username: " << client._username << ", ";
    out << "Host Name: " << client._hostname << ", ";
    out << "Server Name: " << client._servername << ", ";
    out << "Real Name: " << client._name_real << ", ";
    out << "Password: " << client._password << ", ";
    out << "Mode: " << client._mode << ", ";
    out << "]";
    return out;
}