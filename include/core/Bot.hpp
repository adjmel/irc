#pragma once

#include <map>
#include <vector>
#include <string>
#include <iostream>

class Log;

class Bot
{
    private:
    
        static int _ID;
        std::string _name;
        std::string _filename;
        std::map< std::string, std::vector<std::string> > _data;
        Log *_log;

        void load();

    public:
    
        Bot(std::string filename);
        ~Bot();
        
        std::string& nameGet();
        std::string answer(std::string& message);
        bool dataHas(const std::string& value) const;
        bool dataHas(const std::string& key, const std::string& value) const;
        void dataPrint();

        Log *logGet();

        friend std::ostream &operator<<(std::ostream& out, const Bot& bot);
};

