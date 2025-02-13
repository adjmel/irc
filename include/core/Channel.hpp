#pragma once

#include <iostream>
#include <string>
#include <vector>

class Log;
class Server;
class Client;
class Bot;


struct Moderator
{
    int id;
    std::string mode;
};
typedef struct Moderator Moderator;

class Channel
{
    private:
        
        static int _ID;
        std::string _name;
        std::string _topic;
        std::string _password;
        int _clients_max;
        std::string _mode;
        std::vector<Moderator*> _moderators;
        std::vector<Client*> _clients;
        std::vector<Client*> _clients_ban;
        std::vector<Client*> _clients_invited;
        Log *_log;
        Server *_server;
        Bot *_bot;

        std::string nameGenerateUnique(const std::string &prefix);
        std::string intToString(int value);

    public:
        
        Channel(Server *server);
        Channel(Server *server, Client &client);
        ~Channel();

        Log *logGet();
        
        Bot *botGet();

        Server *serverGet();
        void serverSet(Server *server);

        std::string nameGet();
        void nameSet(std::string name);

        std::string topicGet();
        void topicSet(std::string topic);

        std::string passwordGet();
        void passwordSet(std::string password);

        std::vector<Moderator*> &moderatorsGet();
        void moderatorsAdd(Client &cli);
        void moderatorsRemove(int id);
        void moderatorsModeUpdate(Client &cli, std::string mode);
        bool moderatorsCan(Client &cli, std::string mode);
        void moderatorsAddIfNeed();


        int clientsMaxGet();
        void clientsMaxSet(int clientsMax);
        bool clientsMaxIsMax();
        std::vector<Client*> &clientsGet();
        bool clientsBanIs(Client &cli);
        void clientsBanRemove(Client &cli);
        bool clientsHas(Client &cli);
        Client* clientsFindByNick(std::string& nick);
        void clientsRemove(int id);

        void clientsInvitedAdd(Client &cli);
        bool clientsInvitedHas(Client &cli);
        bool clientsInvitedConfirm(Client &cli);

        void modeSet(std::string mode);
        std::string modeGet();
        void modeAdd(std::string mode);
        bool modeCan(std::string mode);

        // --- cmds
        int join(Client &cli, std::string &password);
        int invite(Client &mod, Client &cli);
        int kick(Client &mod, Client &cli, std::string reason);
        int topic(Client &mod);
        int topic(Client &mod, std::string topic);
        int privmsg(Client &from, std::string message);
        int privmsg(Client &from, Client &cli, std::string message);
        
        int modeI(Client &mod, std::string mode);
        int modeT(Client &mod, std::string mode);
        int modeK(Client &mod, std::string mode, std::string password);
        int modeO(Client &mod, std::string mode, Client &cli);
        int modeL(Client &mod, std::string mode);
        int modeL(Client &mod, std::string mode, int clients_max);

        int part(Client &cli, std::string reason);

        int bot(Client &from, Bot &to, std::string message); 
        // ---

        static bool restoreIsValid(Client &cli, std::string line);
        int restore(Client &cli, Channel *channel);
        static void restoreCallback(const std::string& line, void* arg);

        friend std::ostream &operator<<(std::ostream &out, const Channel &channel);
};

struct RestoreData
{
    Client *cli;
    Channel *cha;
};




