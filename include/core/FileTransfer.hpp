#pragma once

#include <fstream>
#include <sstream>

class FileTransfer
{
    public:
    
        FileTransfer();
        ~FileTransfer();

        void send(const char *filePath, void (*callback)(const char *, std::streamsize));
        void receive(const char *filePath, std::streamsize (*callback)(char *, std::streamsize));

};

