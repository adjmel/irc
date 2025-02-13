#include "../../include/core/Log.hpp"
#include "../../include/core/Server.hpp"
#include "../../include/core/Client.hpp"
#include "../../include/core/Utils.hpp"
#include <iostream>
#include <fstream>
#include <ctime>
#include <sstream>
#include <sys/time.h>
#include <iterator>
#include <memory>
#include <vector>

int Log::_ID = 0;

Log::Log() {}

Log::Log(const std::string& prefix, const std::string& dir)
    : _dir(dir)  
{
    Log::_ID++;
    std::string suffix = Utils::intToString(Log::_ID);
    suffix += ".log";
    this->_filename = Utils::nameGenerateUnique(prefix, suffix);
}

Log::~Log()
{
}

std::string &Log::filenameGet()
{
    return this->_filename;
}

void Log::append(const std::string& message, int type)
{
    std::string path = this->_dir + "/" + this->_filename;
    std::ofstream file(path.c_str(), std::ios::app);

    if (file.is_open()) {
       
        time_t currentTime = time(NULL);
        struct tm *localTime = localtime(&currentTime);

        // Format the date and time
        char buffer[80];
        strftime(buffer, sizeof(buffer), "[%Y-%m-%d %H-%M-%S-", localTime);

        // Get milliseconds
        std::ostringstream millisecondsStream;
        {
            struct timeval tv;
            gettimeofday(&tv, NULL);
            millisecondsStream << (tv.tv_usec / 1000);
        }
        std::string milliseconds = millisecondsStream.str();

        // Pad milliseconds with zeros if needed
        while (milliseconds.length() < 3) {
            milliseconds = "0" + milliseconds;
        }

        // Append date, time, milliseconds, and the message to the file
        file << buffer << milliseconds << "] " + Utils::intToString(type) + " : " << message << std::endl;

        file.close();
    }
}

bool Log::lineIsValid(std::string line, std::string types)
{
    //std::cout << line << std::endl;
    std::vector<std::string> strs = Utils::split(line, ":");
    std::vector<std::string> types_tmp = Utils::split(types, ",");
    if (!strs.empty())
    {
        // Assume that the type is the last part of the first element after trimming
        std::string trimmedFirstElement = Utils::trim(strs[0]);
        size_t lastSpace = trimmedFirstElement.find_last_of(" ");
        if (lastSpace != std::string::npos && lastSpace < trimmedFirstElement.size() - 1) 
        {
            std::string type = trimmedFirstElement.substr(lastSpace + 1);
            //std::cout << "type: " << type << std::endl;
            for (size_t i = 0; i < types_tmp.size(); ++i) {
                if (type == Utils::trim(types_tmp[i]))
                    return true;
            }
        }
    }
    return false;
}

bool Log::lineIsValidType(std::string line, std::string type)
{
    std::vector<std::string> strs = Utils::split(line, ":");
    if (!strs.empty())
    {
        // Assume that the type is the last part of the first element after trimming
        std::string trimmedFirstElement = Utils::trim(strs[0]);
        size_t lastSpace = trimmedFirstElement.find_last_of(" ");
        if (lastSpace != std::string::npos && lastSpace < trimmedFirstElement.size() - 1) 
        {
            std::string t = trimmedFirstElement.substr(lastSpace + 1);
            std::cout << "type: " << t << std::endl;
            if (type == Utils::trim(t))
                return true;
        }
    }
    return false;
}

int Log::restore(std::string types, void (*callback)(const std::string&, void *arg), void *arg) 
{
    int status = 1;

    std::string path = this->_dir + "/" + this->_filename;
    std::ifstream file(path.c_str());
    if (file.is_open()) 
    {
        std::string line;
        while (std::getline(file, line)) 
        {
            if (this->lineIsValid(line, types))
            {
                status = 0;
                callback(line, arg);
            }
        }
        file.close();
    }

    return status;
}

std::ostream &operator<<(std::ostream &out, const Log &log)
{
    out << "Log [";
    out << "FD: " << Log::_ID << ", ";
    out << "Filename: " << log._filename << ", ";
    out << "]";
    return out;
}