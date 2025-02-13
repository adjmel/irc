#include "../../include/core/Utils.hpp"
#include <string>
#include <ctime>
#include <cstring>
#include <sstream>
#include <fstream>
#include <sys/time.h>
#include <vector>

Utils::Utils() {}

Utils::~Utils() {}

std::string Utils::nameGenerateUnique(const std::string& prefix,
    const std::string& suffix)
{   
    struct timeval tv;
    gettimeofday(&tv, NULL);

    struct tm localTime;
    localtime_r(&tv.tv_sec, &localTime); // Utilise localtime_r pour éviter des problèmes potentiels
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y%m%d%H%M%S", &localTime);
    sprintf(buffer + 14, "%06ld", tv.tv_usec);
    return prefix + std::string(buffer) + suffix;
}

std::string Utils::intToString(int value)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

std::string Utils::trim(const std::string& str)
{
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");

    if (start == std::string::npos || end == std::string::npos)
        return ""; // La chaîne est vide ou composée uniquement d'espaces

    return str.substr(start, end - start + 1);
}

std::string Utils::replaceByStars(const std::string &input) 
{
    std::istringstream iss(input);
    std::string word;
    std::string result;

    while (iss >> word) {
        if (word.size() > 3) {
            // Remplace les mots de plus de 3 caractères par des étoiles entre le premier et le dernier caractère
            result += word[0] + std::string(word.size() - 2, '*') + word[word.size() - 1] + " ";
        } else {
            // Remplace les mots de 3 caractères ou moins par des étoiles
            result += std::string(word.size(), '*') + " ";
        }
    }

    // Supprime l'espace final ajouté
    if (!result.empty()) {
        result.erase(result.size() - 1);
    }

    return result;
}

std::string Utils::datetime()
{
    // Get the current time
    time_t currentTime = time(NULL);
    struct tm *localTime = localtime(&currentTime);

    // Format the date and time
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H-%M-%S:", localTime);

    // Get milliseconds
    std::ostringstream millisecondsStream;
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        millisecondsStream << (tv.tv_usec / 1000);
    }
    std::string milliseconds = millisecondsStream.str();

    // Pad milliseconds with zeros if needed
    while (milliseconds.length() < 3) 
    {
        milliseconds = "0" + milliseconds;
    }

    std::ostringstream oss;
    oss << buffer
        << milliseconds;
    return oss.str();
}

std::string Utils::now()
{
    // Obtenir le temps actuel
    std::time_t currentTime = std::time(NULL);

    // Convertir le temps actuel en une structure tm
    struct std::tm *localTime = std::localtime(&currentTime);

    // Créer une chaîne de caractères pour le format spécifié
    char buffer[25];
    std::strftime(buffer, sizeof(buffer), "[%Y-%m-%d %H:%M:%S:0]", localTime);

    return std::string(buffer);
}

std::string &Utils::strClean(std::string &message)
{
    while (!message.empty() && !isprint(static_cast<unsigned char>(message[message.size() - 1])) &&
           message[message.size() - 1] != '\n') {
        message.erase(message.size() - 1);
    }
    return message;
}

std::vector<std::string> Utils::split(const std::string str, const std::string delimiter)
{
    std::vector<std::string> tokens;
    size_t pos = 0;
    size_t found;
    
    while ((found = str.find(delimiter, pos)) != std::string::npos) 
    {
        tokens.push_back(str.substr(pos, found - pos));
        pos = found + delimiter.size();
    }
    
    // Ajouter la dernière sous-chaîne
    tokens.push_back(str.substr(pos));

    return tokens;
}

std::vector<std::string> Utils::extract(std::string str)
{
    str = Utils::trim(str);
    size_t firstNonBracket = str.find_first_not_of("[");
    std::string withoutFirstBracket = str.substr(firstNonBracket);

    size_t lastNonBracket = withoutFirstBracket.find_last_not_of("]");
    std::string tmp = withoutFirstBracket.substr(0, lastNonBracket + 1);
    
    std::vector<std::string> tokens = Utils::split(tmp, ",");
    return tokens;
}

std::vector<std::string> Utils::extract(std::string str, std::string delimiter)
{
    str = Utils::trim(str);
    size_t firstNonBracket = str.find_first_not_of("[");
    std::string withoutFirstBracket = str.substr(firstNonBracket);

    size_t lastNonBracket = withoutFirstBracket.find_last_not_of("]");
    std::string tmp = withoutFirstBracket.substr(0, lastNonBracket + 1);
    
    std::vector<std::string> tokens = Utils::split(tmp, delimiter);
    return tokens;
}

std::string Utils::extractFrom(const std::string& str, const std::string& from) 
{
    size_t pos = str.find(from);
    if (pos != std::string::npos) {
        return str.substr(pos);
    }
    return "";
}


std::string  Utils::joinExceptFirstN(const std::vector<std::string>& strings, int N) 
{
    std::string result;

    // Vérifier si le vecteur contient suffisamment d'éléments à exclure
    if (strings.size() > static_cast<size_t>(N)) 
    {
        // Concaténer les éléments à partir de l'indice N
        for (size_t i = N; i < strings.size(); ++i) {
            result += strings[i];
            // Ajouter ":" entre les éléments, sauf pour le dernier élément
            if (i < strings.size() - 1) {
                result += ":";
            }
        }
    }

    return result;
}