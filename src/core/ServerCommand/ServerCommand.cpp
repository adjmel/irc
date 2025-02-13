#include "../../../include/core/ServerCommand.hpp"
#include "../../../include/core/Server.hpp"
#include "../../../include/core/Client.hpp"
#include "../../../include/core/Channel.hpp"
#include "../../../include/core/Utils.hpp"
#include <string>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <cctype>

ServerCommand::ServerCommand(Server *server)
{
    this->_server = server;
}

ServerCommand::~ServerCommand()
{

}

std::vector<std::string> ServerCommand::commandArgsGet(std::string &command)
{
    std::vector<std::string> args;
    std::istringstream iss(command.c_str());
    std::string token;

    // Utiliser un flux pour séparer la commande et les arguments par des espaces
    while (iss >> token) {
        args.push_back(token);
    }

    return args;
}

std::vector<std::string> ServerCommand::commandArgsGetExecptLast(std::string &command, int indexLast)
{
    std::vector<std::string> args;
    std::istringstream iss(command);

    std::string token;
    for (int i = 0; i < indexLast && iss >> token; ++i) {
        args.push_back(token);
    }

    std::string restOfCommand;
    while (iss >> token) {
        if (!restOfCommand.empty()) {
            restOfCommand += " ";
        }
        restOfCommand += token;
    }

    if (!restOfCommand.empty())
        args.push_back(restOfCommand);

    return args;
}

ServerCommandCommandType ServerCommand::commandGetType(std::string &command)
{
    std::vector<std::string> args = ServerCommand::commandArgsGet(command);
    
    std::string cmd = Utils::strClean(args[0]);
    for (size_t i = 0; i < cmd.length(); ++i) {
        cmd[i] = std::toupper(static_cast<unsigned char>(cmd[i]));
    }
    
    if (!cmd.empty() && cmd[0] == '/') {
        cmd.erase(0, 1);
    }

    std::cout << "--- " << args.size() << "--- " << command << std::endl;
    if (this->commandIsValidFileTransfert(command))
        cmd = "FILE_TRANSFERT";

    std::cout << cmd << std::endl;
    
    const char* validCommands[] = {
        "ADMIN", "OPER","PASS", "NICK", "USER",
        "JOIN", "INVITE", "KICK",  "TOPIC", "MODE", "PRIVMSG", 
        "PART", "MSG",
        "QUIT", "SQUIT",
        "PING", "PONG",
        "INFO_ME", "INFO_MODS",
        "FILE_TRANSFERT", "BOT",
    };
    const int numValidCommands = sizeof(validCommands) / sizeof(validCommands[0]);

    for (int i = 0; i < numValidCommands; ++i) {
        if (cmd == validCommands[i]) {
            // La commande commence par un nom de commande valide
            //std::cout << cmd << validCommands[i] << std::endl;
            return static_cast<ServerCommandCommandType>(i + 1);  // L'indice correspondant + 1 (1-indexé)
        }
    }

    // La commande n'est pas valide
    return INVALID;
}

void ServerCommand::commandTypePrint(ServerCommandCommandType type)
{
    const char* commands[] = {
        "INVALID", "ADMIN", "OPER","PASS", "NICK", "USER",
        "JOIN", "INVITE", "KICK",  "TOPIC", "MODE", "PRIVMSG", 
        "PART", "MSG",
        "QUIT", "SQUIT",
        "PING", "PONG",
        "INFO_ME", "INFO_MODS",
        "FILE_TRANSFERT", "BOT",
    };

    std::cout << "Command: " << commands[type] << std::endl;
}

bool ServerCommand::commandIsPartial(std::string &command)
{   
   // Nettoyer la commande des caractères non imprimables à la fin
    std::string cleanedCommand = Utils::strClean(command);

    // Vérifier si la commande est partielle en regardant le dernier caractère
    return !cleanedCommand.empty() && cleanedCommand[cleanedCommand.size() - 1] != '\n';
}

bool ServerCommand::commandIsValidName(std::string command, std::string name)
{
     // Vérifier si les commandes sont égales (ignorer la casse)
    std::transform(command.begin(), command.end(), command.begin(), ::toupper);
    std::transform(name.begin(), name.end(), name.begin(), ::toupper);

    if (command != name)
    {
        // Enlever le '/' si présent dans la commande
        if (command[0] == '/')
        {
            command = command.substr(1); // Supprimer le '/' initial
        }

        // Vérifier à nouveau après avoir retiré le '/'
        std::transform(command.begin(), command.end(), command.begin(), ::toupper);
        if (command != name)
        {
            return false; // Les commandes ne correspondent toujours pas
        }
    }

    return true; // Les commandes correspondent
}

bool ServerCommand::commandIsValid(std::string &command, ServerCommandCommandType type)
{
    bool (ServerCommand::*functions[])(std::string&) = {
        NULL, NULL, NULL, &ServerCommand::commandIsValidPass, 
        &ServerCommand::commandIsValidNick, &ServerCommand::commandIsValidUser,
        &ServerCommand::commandIsValidJoin, &ServerCommand::commandIsValidInvite,
        &ServerCommand::commandIsValidKick, &ServerCommand::commandIsValidTopic,
        &ServerCommand::commandIsValidMode, &ServerCommand::commandIsValidPrivMsg,
        &ServerCommand::commandIsValidPart, &ServerCommand::commandIsValidMsg,
        &ServerCommand::commandIsValidQuit, &ServerCommand::commandIsValidServerQuit,
        &ServerCommand::commandIsValidPing, &ServerCommand::commandIsValidPong,
        &ServerCommand::commandIsValidInfoMe, &ServerCommand::commandIsValidInfoMods,
        &ServerCommand::commandIsValidFileTransfert, &ServerCommand::commandIsValidBot, 
    };
    if (functions[type] == NULL)
        return false;
    return (this->*functions[type])(command);
}

void ServerCommand::commandRun(Client &cli, std::string &message)
{   
    ServerCommandCommandType type = this->commandGetType(message);
    this->commandTypePrint(type);
    //std::cout << "type: " << type << std::endl;
    if (this->commandIsValid(message, type))
    {
        //std::cout << type << std::endl;
        // dans l'ordre de enumeration 
        void (ServerCommand::*functions[])(Client&) = {
            NULL, NULL, NULL, &ServerCommand::commandRunPass,
            &ServerCommand::commandRunNick, &ServerCommand::commandRunUser,
            &ServerCommand::commandRunJoin, &ServerCommand::commandRunInvite, 
            &ServerCommand::commandRunKick, &ServerCommand::commandRunTopic,
            &ServerCommand::commandRunMode, &ServerCommand::commandRunPrivMsg,
            &ServerCommand::commandRunPart, &ServerCommand::commandRunMsg,
            &ServerCommand::commandRunQuit, &ServerCommand::commandRunServerQuit,
            &ServerCommand::commandRunPing, &ServerCommand::commandRunPong,
            &ServerCommand::commandRunInfoMe, &ServerCommand::commandRunInfoMods,
            &ServerCommand::commandRunFileTransfert, &ServerCommand::commandRunBot,
        };
        cli.messageSet(message);
        (this->*functions[type])(cli);
        if (type != QUIT)
            cli.messageSet("");
    }
    else
    {
        std::string cmdJoin = Utils::extractFrom(message, "JOIN");
        if (cmdJoin != "")
            this->commandRun(cli, cmdJoin);
        else 
            cli.messageSet("");
    }
}

