#include "../../include/core/Bot.hpp"
#include "../../include/core/Log.hpp"
#include "../../include/core/Utils.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>

int Bot::_ID = 0;

Bot::Bot(std::string filename) : _filename(filename) 
{
    (this->_ID)++;
    this->_name = Utils::nameGenerateUnique("Bot-", Utils::intToString(this->_ID));
    this->load();
    this->_log = new Log("bot.", "logs");
}

Bot::~Bot() 
{
    delete this->_log;
}

std::string &Bot::nameGet()
{
    return this->_name;
}

Log *Bot::logGet()
{
    return this->_log;
}

void Bot::load()
{
    std::ifstream file(_filename.c_str());

    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            std::istringstream iss(line);
            std::string key;
            std::string word;

            if (std::getline(iss, key, ':'))
            {
                // Lire la liste de mots après les deux points
                std::vector<std::string> words;
                while (std::getline(iss, word, ';'))
                {
                    words.push_back(Utils::trim(word));
                }

                // Ajouter la paire clé-valeur à la map
                _data[key] = words;
            }
        }

        file.close();
    }
    else
    {
        // Gérer l'erreur si le fichier ne peut pas être ouvert
        std::cerr << "Erreur: Impossible d'ouvrir le fichier " << _filename << std::endl;
    }
}

void Bot::dataPrint()
{
    std::cout << "Contenu de la map _data :" << std::endl;

    for (std::map<std::string, std::vector<std::string> >::iterator it = _data.begin(); it != _data.end(); ++it)
    {
        std::cout << it->first << ": ";
        for (std::vector<std::string>::iterator vecIt = it->second.begin(); vecIt != it->second.end(); ++vecIt)
        {
            std::cout << *vecIt << "; ";
        }
        std::cout << std::endl;
    }
}

std::string Bot::answer(std::string &message)
{
    std::string key = "unknown";
    std::string msg = "...";

     // Vérifier si la clé "dono" existe dans la map
    std::map< std::string, std::vector<std::string> >::iterator it = _data.find("dono");

    if (it != _data.end() && !it->second.empty()) {
        // La clé "dono" existe et le vecteur associé n'est pas vide

        // Utiliser srand et rand si nécessaire
        std::srand(static_cast<unsigned>(std::time(0)));

        // Sélectionner un élément aléatoire du vecteur
        int randomIndex = std::rand() % it->second.size();
        msg = it->second[randomIndex];
    }

    it = _data.begin();

    for (; it != _data.end(); ++it)
    {
        // Imprime les éléments du vecteur associé à la clé avant la vérification
        // std::cout << "Search: '" << message << "' in " << it->first << ", Values: " << std::endl;
        // for (std::vector<std::string>::const_iterator valueIt = it->second.begin(); valueIt != it->second.end(); ++valueIt)
        // {
        //     std::cout << *valueIt << std::endl;
        //     if (*valueIt == message)
        //         break;
        // }
        // std::cout << std::endl;
        // std::cout << "key: " << key << std::endl;

        // Puis verifier
        if (std::find(it->second.begin(), it->second.end(), message) != it->second.end())
        {
            key = it->first;
            break;
        }
    }

    //std::cout << "key: " << key << std::endl;

    if (it != _data.end()) // Vérifiez si la boucle a atteint la fin avant d'accéder à it->second
    {
        if (key != "bad-word")
        {
            std::srand(static_cast<unsigned>(std::time(0)));
            if (!it->second.empty())
            {
                int randomIndex = std::rand() % it->second.size();
                msg = it->second[randomIndex];
            }
        }
        else
        {
            std::vector<std::string>::iterator wordIt = std::find(it->second.begin(), it->second.end(), message);
            if (wordIt != it->second.end())
            {
                msg = *wordIt;
            }
            msg = Utils::replaceByStars(msg);
        }
    }

    return msg;
}

bool Bot::dataHas(const std::string &value) const 
{
    for (std::map<std::string, std::vector<std::string> >::const_iterator it = _data.begin(); it != _data.end(); ++it) 
    {
        if (std::find(it->second.begin(), it->second.end(), value) != it->second.end()) 
        {
            return true;
        }
    }

    return false;
}

bool Bot::dataHas(const std::string &key, const std::string &value) const
{
    std::map<std::string, std::vector<std::string> >::const_iterator it = _data.find(key);

    if (it != _data.end()) {
            const std::vector<std::string>& valuesVector = it->second;

            // Recherche de la valeur dans le vecteur associé à la clé
            if (std::find(valuesVector.begin(), valuesVector.end(), value) != valuesVector.end()) {
                return true;
            }
        }

    return false;
}

std::ostream &operator<<(std::ostream &out, const Bot &bot)
{
    out << "Bot [";
    out << "Name: " << bot._name << ", ";
    out << "]";
    return out;
}
