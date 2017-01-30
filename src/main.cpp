#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif

#include <iostream>

#include <SDL/SDL.h>

#include "states/PanelMain.h"

int main ( int argc, char** argv )
{

    try
    {
        PanelMain PanelGame;

        while ( PanelGame.Play() )
        {
            PanelGame.Frame();
        }

        // Quitter normalement
    }
    catch (std::exception & e)
    {
        // Quitter avec une erreur
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
