#include "../../include/core/Server.hpp"
#include <iostream>
#include <cstdlib>

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "Only 3 Args" << std::endl;
        return (1);
    }
    std::cout << "Server IRC:" << std::endl;
    Server server("0.0.0.0", atoi(argv[1]), argv[2]);
    server.start();
    
    return (0);
}