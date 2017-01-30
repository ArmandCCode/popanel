#include "SPSDL.h"

SPSDL::SPSDL()
{

    this->StartEngine();
}

SPSDL::~SPSDL()
{

    this->StopEngine();
}

void SPSDL::StartEngine()
{
    // Activation des modules video, timer et joystick
    if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK) < 0 )
    {
        std::string sMess("Unable to init SDL: ");
        sMess += SDL_GetError();
        // Lancer une exception pour quitter proprement
        SPException e(sMess);
        throw e;
    }

    // Est-ce que le module audio de la SDL peut être chargé ?
    if( SDL_InitSubSystem( SDL_INIT_AUDIO ) == - 1)
    {
        std::string sMess("SDL : Unable to initialize audio module. ");
        sMess = sMess + SDL_GetError();
        SPException e( sMess );
        throw e;
    }

    // Activation du module SDL_mixer
    // Est-ce que SDL_mixer peut lire des OGG ?
    if ( Mix_Init( MIX_INIT_OGG ) != MIX_INIT_OGG )
    {
        std::string sMess("SDL_mixer : OGG Support not working. ");
        sMess = sMess + Mix_GetError();
        SPException e( sMess );
        throw e;
    }

    // Activation du module SDL_TTF
    if( TTF_Init() ==-1 )
    {
        std::string sMess("SDL_ttf : Unable to initialize. ");
        sMess += TTF_GetError();
        // Lancer une exception pour quitter proprement
        SPException e(sMess);
        throw e;
    }
}

void SPSDL::StopEngine()
{

    // fermer SDL_ttf
    TTF_Quit();

    // fermer SDL_mixer

    Mix_Quit();
    SDL_QuitSubSystem( SDL_INIT_AUDIO );

    // fermer SDL
    SDL_Quit();
}


void SPSDL::CreateWindow(int p_Width, int p_Height, int p_BitPerPixel, bool p_Fullscreen, std::string p_WinName)
{
    this->m_Window.CreateWindow(p_Width, p_Height, p_BitPerPixel, p_Fullscreen);
    this->m_Window.SetWindowName(p_WinName);
}

void SPSDL::DestroyWindow()
{
    if ( this->window().isWindowCreated() )
        this->window().DestroyWindow();
}

void SPSDL::CreateAudioOutput(int p_SampleFreq, int p_SampleSize)
{
    this->audio().DestroyAudioOutput(); // Supprimer la sortie audio existante le cas échéant
    this->audio().CreateAudioOutput(p_SampleFreq, p_SampleSize);
}

void SPSDL::DestroyAudioOutput()
{
    if ( this->audio().isAudioOutputOpen() )
        this->audio().DestroyAudioOutput();
}
