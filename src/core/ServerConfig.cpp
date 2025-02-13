#include "../../include/core/ServerConfig.hpp"
#include "../../include/core/Server.hpp"
#include "../../include/core/Channel.hpp"
#include "../../include/core/Utils.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <limits> 

ServerConfig::ServerConfig(Server *server) 
{
    this->loadConfig(server, "assets/ircd.conf");
}

ServerConfig::~ServerConfig() 
{
}

void ServerConfig::loadConfig(Server *server, std::string filename) 
{
    std::ifstream configFile(filename.c_str());

    if (!configFile.is_open()) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier de configuration." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(configFile, line)) {
        std::istringstream iss(line);
        std::string key;
        if (iss >> key) {
            if (key == "serverinfo") {
                this->loadConfigServerInfo(server, configFile);
            } else if (key == "admin") {
                this->loadConfigAdmin(server, configFile);
            } else if (key == "channels") {
                this->loadConfigChannels(server, configFile);
            }
            // Ajoutez d'autres conditions pour d'autres sections du fichier
        }
    }

    configFile.close();
}

void ServerConfig::loadConfigServerInfo(Server *server, std::ifstream& configFile) {
    std::string line;
    while (std::getline(configFile, line) && line.find("}") == std::string::npos) {
        std::istringstream issLine(line);
        std::string subKey;
        if (issLine >> subKey) {
            if (subKey == "name") 
            {
                std::string name;
                issLine.ignore(std::numeric_limits<std::streamsize>::max(), '=');
                std::getline(issLine, name);
                name = Utils::trim(name);
                name = name.substr(name.find_first_of("\"")+1, name.find_last_of("\"")-1);
                //std::cout << name << std::endl;
                server->nameSet(name);
            } 
            else if (subKey == "network") 
            {
                std::string network;
                issLine.ignore(std::numeric_limits<std::streamsize>::max(), '=');
                std::getline(issLine, network);
                network = Utils::trim(network);
                network = network.substr(network.find_first_of("\"")+1, network.find_last_of("\"")-1);
                //std::cout << network << std::endl;
                server->networkSet(network);
            }
        }
    }
}

void ServerConfig::loadConfigAdmin(Server *server, std::ifstream& configFile) {
    std::string line;
    Admin *admin = new Admin;

    while (std::getline(configFile, line) && line.find("}") == std::string::npos) {
        std::istringstream issLine(line);
        std::string subKey;

        if (issLine >> subKey) {
            if (subKey == "name") 
            {
                issLine.ignore(std::numeric_limits<std::streamsize>::max(), '=');
                std::getline(issLine, admin->name);

                admin->name = Utils::trim(admin->name);
                admin->name = admin->name.substr(admin->name.find_first_of("\"")+1, admin->name.find_last_of("\"")-1);
                //std::cout << admin->name << std::endl;

                admin->name = admin->name;
            } 
            else if (subKey == "password") 
            {
                issLine.ignore(std::numeric_limits<std::streamsize>::max(), '=');
                std::getline(issLine, admin->password);

                admin->password = Utils::trim(admin->password);
                admin->password = admin->password.substr(admin->password.find_first_of("\"")+1, admin->password.find_last_of("\"")-1);
                //std::cout << admin->password << std::endl;

                admin->password = admin->password;
            }
        }
    }
    server->adminsAdd(admin);
}

void ServerConfig::loadConfigChannels(Server *server, std::ifstream& configFile) {
    std::string line;
    while (std::getline(configFile, line) && line.find("}") == std::string::npos) {
        std::istringstream issChannel(line);
        std::string channelKey;

        if (issChannel >> channelKey && channelKey == "channel") {
            Channel *channel = new Channel(server);
            while (std::getline(configFile, line) && line.find("}") == std::string::npos) {
                std::istringstream issChannelLine(line);
                std::string channelSubKey;

                if (issChannelLine >> channelSubKey) 
                {
                    if (channelSubKey == "name") 
                    {
                        issChannelLine.ignore(std::numeric_limits<std::streamsize>::max(), '=');
                        std::string tmpName;
                        std::getline(issChannelLine, tmpName);

                        tmpName = Utils::trim(tmpName);
                        tmpName = tmpName.substr(tmpName.find_first_of("\"")+1, tmpName.find_last_of("\"")-1);
                        //std::cout << tmpName << std::endl;

                        channel->nameSet(tmpName);
                    } 
                    else if (channelSubKey == "password") 
                    {
                        issChannelLine.ignore(std::numeric_limits<std::streamsize>::max(), '=');
                        std::string tmpPassword;
                        std::getline(issChannelLine, tmpPassword);

                        tmpPassword = Utils::trim(tmpPassword);
                        tmpPassword = tmpPassword.substr(tmpPassword.find_first_of("\"")+1, tmpPassword.find_last_of("\"")-1);
                        //std::cout << tmpPassword << std::endl;

                        channel->passwordSet(tmpPassword);
                    }
                }
            }

            //std::cout << *channel << std::endl;
            server->channelsAdd(channel);
        }
    }
}




