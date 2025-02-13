#pragma once

#include <iostream>
#include <string>
#include <vector>

class Channel;
class FileTransfer;

class Client
{
    private:

        static int _ID;
        int _id;
        int _fd;
        int _status;
        std::string _username;
        std::string _nick;
        std::string _name_real;
        std::string _unused;
        std::string _hostname;
        std::string _servername;
        std::string _password;
        std::string _mode; 
        std::string _message;
        int _message_status;
        std::vector<Channel*> _channels;
        Channel *_channel_recent;
        FileTransfer* _file_transfer;
        bool _admin_is;
        bool _irssi_is;

    public:

        Client();
        Client(int fd);
        ~Client();

        int idGet();

        int fdGet();

        void statusSet(int status);
        int statusGet();

        void usernameSet(std::string username);
        std::string usernameGet();

        void nickSet(std::string nick);
        std::string nickGet();
        static bool nickIsValid(std::string& nick);
        static std::string nickGenerateUnique();

        void nameRealSet(std::string nameReal);
        std::string nameRealGet();

        void unusedSet(std::string unused);
        std::string unusedGet();

        void servernameSet(std::string servername);
        std::string servernameGet();

        void hostnameSet(std::string hostname);
        std::string hostnameGet();

        void passwordSet(std::string password);
        std::string passwordGet();

        void modeSet(std::string mode);
        std::string modeGet();
        void modeAdd(std::string mode);
        bool modeCan(std::string mode);

        void messageSet(std::string command);
        std::string messageGet();
        int messageStatusGet();

        void channelsAdd(Channel* channel);
        std::vector<Channel*>& channelsGet();
        Channel* channelsFindByName(std::string name);
        bool channelsDeleteAll();
        bool channelsDeleteByName(std::string name);

        void channelRecentSet(Channel *ch);
        bool channelRecentIs(Channel *ch);

        void adminSet(bool admin_is);
        bool adminIs();

        void irssi(std::string message);
        bool irssiIs();

        std::string toString();

        friend std::ostream &operator<<(std::ostream& out, const Client& client);
};

