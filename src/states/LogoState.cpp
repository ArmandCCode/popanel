#include "LogoState.h"

LogoState::LogoState(SPSDL& p_SDLRef, StateRequestObject& p_StateRequest, KeyboardControls& p_ControlsSettings)
    : SPGameState(p_SDLRef),
    m_stateRequest(p_StateRequest),
    m_Controls(p_ControlsSettings),
    m_sndList( p_SDLRef.audio() )
{

    this->LoadResources();

    // Fondu en entrée
    this->m_Fader.SetFadingType(FaderColor::FADEIN);
    this->m_Fader.SetFadingLength(C_FadingTimers::FADEIN);
    this->m_Fader.StartFadingNow();
    this->m_TimerUI.SetLength(4000);
    this->m_TimerUI.Restart();
}

LogoState::~LogoState()
{

    this->UnloadResources();
}

bool LogoState::Play(long p_Delta)
{

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        // check for messages
        switch (event.type)
        {
            // exit if the window is closed
        case SDL_QUIT:
            this->m_stateRequest.SetDesiredState( StateRequestObject::CLOSEAPP );
            return false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == m_Controls.m_Validate)
            {
                // Forcer la fin du chrono
                if (  !this->m_TimerUI.IsTimerEnd() )
                {
                    m_SDL.audio().PlaySample( m_sndList.GetSample( LogoSnd::SPL_VALIDATE), 1 );
                    this->m_TimerUI.SetLength(0);
                }
            }
            break;
        default:
            break;
        }
    }

    // Traitements automatisés
    if ( this->m_TimerUI.IsTimerEnd() )
    {
        // Paramétrer l'objet de fondu... une seule fois
        if (this->m_Fader.GetFadingType() == FaderColor::FADEIN)
        {
            this->m_Fader.SetFadingType(FaderColor::FADEOUT);
            this->m_Fader.SetFadingLength(C_FadingTimers::FADEOUT);
            this->m_Fader.StartFadingNow();
        }

        // Si la transition de sortie est terminée
        if (this->m_Fader.GetFadingType() == FaderColor::FADEOUT && this->m_Fader.isFadingDone() )
        {
            this->m_stateRequest.SetDesiredState( StateRequestObject::TITLE );
            return false;
        }

    }

    return true;
}

bool LogoState::Frame()
{
    m_SDL.window().EmptyFrame();

    //this->m_texList.GetTextureByID(LogoTex::TX_LOGO).RenderToWindow(m_SDL.window(), 109, 190);
    this->m_GameNameText.RenderToWindow(m_SDL.window(), 320, 176, SPTexture::RENDER_FROMCENTER);
    this->m_DevelopedByText.RenderToWindow(m_SDL.window(), 320, 240, SPTexture::RENDER_FROMCENTER);
    this->m_HyperlinkText.RenderToWindow(m_SDL.window(), 320, 272, SPTexture::RENDER_FROMCENTER);

    #ifdef DEBUGMODE
        // FPS
        this->m_TextRenderer.RenderTextOnScreen(m_SDL.window(), 0, 0, SPConv::NumToStr(m_SDL.window().FPSLimiter().GetLastFPS()) );
    #endif

    this->m_Fader.RenderToWindow( m_SDL.window() );

    m_SDL.window().DisplayFrame();
    return true;
}


bool LogoState::LoadResources()
{
    LogoTex::LoadLogoTex( this->m_texList );

    LogoSnd::LoadLogoSnd( this->m_sndList );

    // Texte
    this->m_TextRenderer.LoadFont("./fonts/armandc-ascii-font-8.png", 8, 8);

    // Texte TTF
    this->m_LogoFont.LoadFont("./fonts/BubblegumSans-Regular.otf", C_FontProps::SMALLFONTHEIGHT);

    this->m_GameNameText.SetColor(128, 128, 128);
    this->m_GameNameText.RasterizeString("POPanel - 2016", m_LogoFont);
    this->m_DevelopedByText.SetColor(128, 128, 128);
    this->m_DevelopedByText.RasterizeString("Game developed by Armand Christophe", m_LogoFont);
    this->m_HyperlinkText.SetColor(128, 128, 128);
    this->m_HyperlinkText.RasterizeString("http://www.armandchristophe.com", m_LogoFont);

    return true;
}

bool LogoState::UnloadResources()
{
    this->m_texList.UnloadEverything();

    this->m_sndList.UnloadAllSamples();
    this->m_sndList.UnloadAllMusic();

    return true;
}
