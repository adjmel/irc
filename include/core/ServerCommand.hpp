#pragma once

#include <string>
#include <vector>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstdlib>

class Server;
class Client;

enum ServerCommandCommandType {
    INVALID = 0,
    ADMIN,
    OPER,
    PASS,
    NICK,
    USER,
    // important sur le sujet
    JOIN,
    INVITE,
    KICK,
    TOPIC,
    MODE,
    PRIVMSG,
    PART,
    MSG,
    QUIT,
    SQUIT,
    PING,
    PONG, 
    // non standard
    INFO_ME,
    INFO_MODS,
    FILE_TRANSFERT,
    BOT,
};

class ServerCommand
{
    private:

        Server *_server;

    public:

        ServerCommand(Server *server);
        ~ServerCommand();

        static std::vector<std::string> commandArgsGet(std::string &command);
        static std::vector<std::string> commandArgsGetExecptLast(std::string &command, int indexLast);
        ServerCommandCommandType commandGetType(std::string &command);
        void commandTypePrint(ServerCommandCommandType type);

        bool commandIsPartial(std::string &command);
        bool commandIsValidName(std::string command, std::string name);
        bool commandIsValid(std::string &command, ServerCommandCommandType type);
        bool commandIsValidPass(std::string &command);
        bool commandIsValidNick(std::string &command);
        bool commandIsValidUser(std::string &command);
        
        bool commandIsValidKick(std::string &command);
        bool commandIsValidInvite(std::string &command);
        bool commandIsValidTopic(std::string &command);
        bool commandIsValidMode(std::string &command);
        static bool commandIsValidModeOption(std::string mode, std::string option);
        bool commandIsValidJoinChannelName(std::string name);
        bool commandIsValidJoin(std::string &command);
        bool commandIsValidJoinChannel(std::string &command);
        bool commandIsValidPrivMsg(std::string &command);
        bool commandIsValidQuit(std::string &command);
        bool commandIsValidServerQuit(std::string &command);

        bool commandIsValidPart(std::string &command);

        bool commandIsValidMsg(std::string &command);

        bool commandIsValidPing(std::string &command);
        bool commandIsValidPong(std::string &command);
        
        bool commandIsValidInfoMe(std::string &command);
        bool commandIsValidInfoMods(std::string &command);

        bool commandIsValidFileTransfert(std::string &command);
        bool commandIsValidBot(std::string &command);


        void commandRun(Client &cli, std::string &message);
        void commandRunPass(Client &cli);
        void commandRunNick(Client &cli);
        void commandRunUser(Client &cli);
        
        void commandRunJoin(Client &cli);
        void commandRunInvite(Client &cli);
        void commandRunKick(Client &cli);
        void commandRunTopic(Client &cli);
        void commandRunPrivMsgSend(Client &from, Client &to, std::string message);
        void commandRunPrivMsg(Client &cli);
    
        void commandRunModeI(Client &cli);
        void commandRunModeT(Client &cli);
        void commandRunModeK(Client &cli);
        void commandRunModeO(Client &cli);
        void commandRunModeL(Client &cli);
        void commandRunMode(Client &cli);

        void commandRunPart(Client &cli);

        void commandRunPing(Client &cli);
        void commandRunPong(Client &cli);

        void commandRunQuit(Client &cli);
        void commandRunServerQuit(Client &cli);

        int commandRunMsgSend(Client &from, Client &to, std::string message);
        void commandRunMsg(Client &cli);

        void commandRunInfoMe(Client &cli);
        void commandRunInfoMods(Client &cli);

        void commandRunFileTransfertReceive(const char *filename, in_addr_t clientIp, int clientPort, std::streamsize fileSize);
        void commandRunFileTransfertSend(const char *filename, const char *clientIp, int clientPort);
        void commandRunFileTransfert(Client &cli);
        void commandRunBot(Client &cli);
};

