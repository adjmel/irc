#pragma once

#include <string>
#include <vector>

class Utils
{
    private:
        
        Utils();
        ~Utils();

    public:

        static std::string nameGenerateUnique(const std::string& prefix, 
            const std::string& suffix);
        static std::string intToString(int value);
        static std::string trim(const std::string& str);
        static std::string replaceByStars(const std::string &input);
        static std::string datetime();
        static std::string now();
        static std::string &strClean(std::string &message);
        static std::vector<std::string> split(const std::string str, const std::string delimiter);
        static std::vector<std::string> extract(std::string str);
        static std::vector<std::string> extract(std::string str, std::string delimiter);
        static std::string joinExceptFirstN(const std::vector<std::string>& strings, int N);
        static std::string extractFrom(const std::string& str, const std::string& from);
};

