# include "server/Server.hpp"

/*
    
*/

int main(int argc, char **argv)
{
    Server* serv = NULL;

    try
    {
        serv = new Server(argc, argv);
        serv->start();
        delete serv;
        serv = NULL;
    }
    catch (const std::exception& e) 
    {
        std::cerr << "Error: " << e.what() << '\n';
        if (serv != NULL)
        {
            delete serv;
            serv = NULL;
        }
        return 1;
    }

    return 0;
}