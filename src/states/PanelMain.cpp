#include "PanelMain.h"

PanelMain::PanelMain()
{

    m_SDL.StartEngine();
    this->LoadResources();
    this->LoadControls();
}

PanelMain::~PanelMain()
{

    this->UnloadResources();
    m_SDL.StopEngine();
}

bool PanelMain::LoadResources() // Load everything
{

    // créer la fenêtre
    m_SDL.CreateWindow(640, 480, DISP_BPP, false, "Popanel");
    m_SDL.CreateAudioOutput(44100, 1024);
    m_SDL.window().FPSLimiter().SetFrameLimit(60);
    m_SDL.window().FPSLimiter().Enable(true);

    return true;
}

bool PanelMain::UnloadResources() // Unload everything
{
    m_SDL.window().DestroyWindow();

    return true;
}


bool PanelMain::LoadControls() // Load everything
{
    // Load default controls
    SetDefaultControls(m_Controls);
    if (ControlsIniExist())
    {
        ControlsLoadFromIni(m_Controls);
    }
    else
    {
        ControlsSaveToIni(m_Controls);
    }

    // Set the action dictionary
    SetDictionaryFromControls(m_SDL.input(), m_Controls);

    return true;
}

bool PanelMain::Play(long p_Delta) // Events and game logic
{
    switch( m_StateRequest.GetDesiredState() )
    {
    case StateRequestObject::STARTUP :
        m_StateRequest.SetDesiredState(StateRequestObject::LOGO);
        break;

    case StateRequestObject::LOGO :
        {
            LogoState logo(m_SDL, this->m_StateRequest, this->m_Controls);
            logo.EnterState();
        }
        break;

    case StateRequestObject::TITLE :
        {
            TitleState title(m_SDL, this->m_StateRequest, this->m_Controls, this->m_MatchSettings);
            title.EnterState();
        }
        break;

    case StateRequestObject::MENU :
        m_StateRequest.SetDesiredState(StateRequestObject::GAME);
        break;

    case StateRequestObject::GAME :
        {
            GameState game(m_SDL, this->m_StateRequest, this->m_Controls, this->m_MatchSettings);
            game.EnterState();
        }

        break;

    case StateRequestObject::KEYCONFIG :
        {
            KeyConfigState keyconf(m_SDL, this->m_StateRequest, this->m_Controls);
            keyconf.EnterState();
        }

        break;

    default:
        return false;
        break;
    }

    return true;
}

bool PanelMain::Frame() // Drawing graphics
{
    m_SDL.window().EmptyFrame();

    m_SDL.window().DisplayFrame();

    return true;
}
