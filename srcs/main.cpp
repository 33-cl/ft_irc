# include "server/Server.hpp"

/*
    Quand on quitte le serv depuis nc avec ctrl+C et non quit ca supprime pas vraiment le client du serv donc il reste OP
*/

int main(int argc, char **argv)
{
    try
    {
        Server* serv = new Server(argc, argv);

        serv->start();

        delete serv;
    }
    catch (const std::exception& e) 
    {
        std::cerr << e.what() << '\n';
        return 1;
    }

    return 0;
} 