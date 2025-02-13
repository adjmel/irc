#pragma once

#include <string>
#include <iterator>

class Server;
class Client;

class Log
{
    private:

        static int _ID;
        std::string _filename;
        std::string _dir;

        bool lineIsValid(std::string& line);

    public:

        Log();
        Log(const std::string& prefix, const std::string &dir);
        ~Log();

        std::string &filenameGet();

        void append(const std::string& message, int type);
        static bool lineIsValid(std::string line, std::string types);
        static bool lineIsValidType(std::string line, std::string type);
        int restore(std::string types, void (*callback)(const std::string&, void *arg), void *arg);
        friend std::ostream &operator<<(std::ostream &out, const Log &log);
};

