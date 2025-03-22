# include "server/Server.hpp"

/*
	Dans nick, doit commencer par une lettre et accepte -, _, [, ], {, }, \\, |, ^
*/

int main(int argc, char **argv)
{
    Server  serv;

    try
    {
        serv.initialize(argc, argv);
        serv.start();
    }
    catch (const std::exception& e) {std::cerr << e.what() << '\n'; return 1;}
} 