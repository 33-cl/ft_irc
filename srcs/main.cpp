# include "../includes/Server.hpp"

int main(int argc, char **argv)
{
    Server  serv;

    try
    {
        serv.initialize(argc, argv);
        serv.infos();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    

}