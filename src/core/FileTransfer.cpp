#include "../../include/core/FileTransfer.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

FileTransfer::FileTransfer()
{
}

FileTransfer::~FileTransfer()
{
}

void FileTransfer::send(const char *filePath, void (*callback)(const char*, std::streamsize))
{
    std::ifstream file(filePath, std::ios::binary);

    if (!file)
    {
        std::cerr << "Error: Cannot open file." << std::endl;
        return;
    }

    // Obtenir la taille du fichier
    file.seekg(0, std::ios::end);
    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    const std::streamsize bufferSize = 1024;
    char buffer[bufferSize];

    // Lire le fichier et envoyer au callback par morceaux
    while (file)
    {
        file.read(buffer, bufferSize);
        callback(buffer, file.gcount());
    }

    file.close();
}

void FileTransfer::receive(const char *filePath, std::streamsize (*callback)(char*, std::streamsize))
{
    // Ouvrir le fichier en mode binaire
    std::ofstream file(filePath, std::ios::binary);

    if (!file)
    {
        std::cerr << "Error: Cannot open file for writing." << std::endl;
        return;
    }

    // Utiliser un tampon pour gérer des quantités de données plus importantes
    const std::streamsize bufferSize = 1024;
    char buffer[bufferSize];

    // Recevoir les données par morceaux et les écrire dans le fichier
    std::streamsize bytesRead;
    do
    {
        bytesRead = callback(buffer, bufferSize);
        file.write(buffer, bytesRead);
    } while (bytesRead > 0);
    
    file.close();
}
