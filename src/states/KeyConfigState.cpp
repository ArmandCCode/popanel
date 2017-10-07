#include "KeyConfigState.h"

KeyConfigState::KeyConfigState(SPSDL& p_SDLRef, StateRequestObject& p_StateRequest, KeyboardControls& p_ControlsSettings)
    : SPGameState( p_SDLRef ),
    m_stateRequest(p_StateRequest),
    m_Controls(p_ControlsSettings),
    m_sndList( p_SDLRef.audio() )
{

    this->LoadResources();

    this->SetMenuStatus(KeyConfigState::MENU_FADEIN);
    this->m_Fader.SetFadingType(FaderColor::FADEIN);
    this->m_Fader.SetFadingLength(C_FadingTimers::FADEIN);
    this->m_Fader.StartFadingNow();
    this->m_BGScrollEffect.SetLength(6000);
}

KeyConfigState::~KeyConfigState()
{

    this->UnloadResources();
}

bool KeyConfigState::Play(long p_Delta)
{
    // Fading-out effect
    bool bCausesExitState = false;

    // User input
    SDL_Event event;
    // Use normal SDL event loop, because we want to capture the pressed key
    while (this->m_SDL.input().SDLPollEvent(&event))
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
            switch (this->m_MenuStatus)
            {
                case KeyConfigState::MENU_INMENU:
                    if (event.key.keysym.sym == m_Controls.m_Up) {
                        this->m_SDL.audio().PlaySample(m_sndList.GetSample(TitleSnd::SPL_CURSORMOVE), 2);
                        this->m_MenuCtl.PrevEntry();
                    }
                    else if (event.key.keysym.sym == m_Controls.m_Down) {
                        this->m_SDL.audio().PlaySample(m_sndList.GetSample(TitleSnd::SPL_CURSORMOVE), 2);
                        this->m_MenuCtl.NextEntry();
                    }
                    else if (event.key.keysym.sym == m_Controls.m_Left) {
                        this->m_SDL.audio().PlaySample(m_sndList.GetSample(TitleSnd::SPL_CURSORMOVE), 2);
                        this->m_MenuCtl.PrevValue(this->m_MenuCtl.GetSelectedEntry());
                    }
                    else if (event.key.keysym.sym == m_Controls.m_Right) {
                        this->m_SDL.audio().PlaySample(m_sndList.GetSample(TitleSnd::SPL_CURSORMOVE), 2);
                        this->m_MenuCtl.NextValue(this->m_MenuCtl.GetSelectedEntry());
                    }
                    else if (event.key.keysym.sym == m_Controls.m_Validate) {
                        this->m_SDL.audio().PlaySample(m_sndList.GetSample(TitleSnd::SPL_VALIDATE), 1);

                        if (this->m_MenuCtl.GetSelectedEntry() == MENU_QUITNOSAVE)
                        {
                            // Go back to menu without saving
                            this->m_stateRequest.SetDesiredState( StateRequestObject::TITLE );
                            bCausesExitState = true;

                        }
                        else if (this->m_MenuCtl.GetSelectedEntry() == MENU_SAVEANDQUIT)
                        {
                            // Player 1
                            this->m_Controls.p1_Up      = (SDLKey)this->m_MenuCtl.GetValue(MENU_P1KEYUP);
                            this->m_Controls.p1_Down    = (SDLKey)this->m_MenuCtl.GetValue(MENU_P1KEYDOWN);
                            this->m_Controls.p1_Left    = (SDLKey)this->m_MenuCtl.GetValue(MENU_P1KEYLEFT);
                            this->m_Controls.p1_Right   = (SDLKey)this->m_MenuCtl.GetValue(MENU_P1KEYRIGHT);
                            this->m_Controls.p1_Swap    = (SDLKey)this->m_MenuCtl.GetValue(MENU_P1KEYSWAP);
                            this->m_Controls.p1_Raise   = (SDLKey)this->m_MenuCtl.GetValue(MENU_P1KEYRAISE);
                            this->m_Controls.p1_Pause   = (SDLKey)this->m_MenuCtl.GetValue(MENU_P1KEYPAUSE);
                            // Player 2
                            this->m_Controls.p2_Up      = (SDLKey)this->m_MenuCtl.GetValue(MENU_P2KEYUP);
                            this->m_Controls.p2_Down    = (SDLKey)this->m_MenuCtl.GetValue(MENU_P2KEYDOWN);
                            this->m_Controls.p2_Left    = (SDLKey)this->m_MenuCtl.GetValue(MENU_P2KEYLEFT);
                            this->m_Controls.p2_Right   = (SDLKey)this->m_MenuCtl.GetValue(MENU_P2KEYRIGHT);
                            this->m_Controls.p2_Swap    = (SDLKey)this->m_MenuCtl.GetValue(MENU_P2KEYSWAP);
                            this->m_Controls.p2_Raise   = (SDLKey)this->m_MenuCtl.GetValue(MENU_P2KEYRAISE);
                            this->m_Controls.p2_Pause   = (SDLKey)this->m_MenuCtl.GetValue(MENU_P2KEYPAUSE);

                            // Save in config file
                            ControlsSaveToIni(m_Controls);

                            // Affect new controls to action dictionary
                            SetDictionaryFromControls(m_SDL.input(), m_Controls);

                            // Go back to menu
                            this->m_stateRequest.SetDesiredState( StateRequestObject::TITLE );
                            bCausesExitState = true;
                        }
                        else if (this->m_MenuCtl.GetSelectedEntry() == MENU_RESETDEFAULTS)
                        {
                            // Reset contents of config file
                            SetDefaultControls(m_Controls);
                            ControlsSaveToIni(m_Controls);
                            // Affect new controls to action dictionary
                            SetDictionaryFromControls(m_SDL.input(), m_Controls);
                            // This will cause the state to reload with the default values
                            this->m_stateRequest.SetDesiredState( StateRequestObject::KEYCONFIG );
                            bCausesExitState = true;
                        }
                        else
                        {
                            this->SetMenuStatus(MENU_WAITFORINPUT);
                            this->m_MessageBox.SetLineString(0, "Set key for " + this->m_MenuCtl.GetLabel(this->m_MenuCtl.GetSelectedEntry()), m_FontMenu, C_FontProps::MENUELTFONTR, C_FontProps::MENUELTFONTG, C_FontProps::MENUELTFONTB);
                            this->m_MessageBox.SetLineString(1, "Or wait 5 seconds to cancel.", m_FontMenu, C_FontProps::MENUELTFONTR, C_FontProps::MENUELTFONTG, C_FontProps::MENUELTFONTB);
                            this->m_MessageBox.SetLineString(2, "---", m_FontMenu, C_FontProps::MENUELTFONTR, C_FontProps::MENUELTFONTG, C_FontProps::MENUELTFONTB);
                            this->m_MessageBox.SetLineString(3, "5", m_FontMenu, C_FontProps::MENUELTFONTR, C_FontProps::MENUELTFONTG, C_FontProps::MENUELTFONTB);
                            this->m_KeyPressCountdown.SetLength(5000);
                            this->m_KeyPressCountdown.Restart();
                        }
                    }
                    else if (event.key.keysym.sym == m_Controls.m_Cancel) {
                        this->m_SDL.audio().PlaySample(m_sndList.GetSample(TitleSnd::SPL_VALIDATE), 1);
                        this->m_MenuCtl.SelectEntry(MENU_QUITNOSAVE);
                    }
                break;

                case KeyConfigState::MENU_WAITFORINPUT:
                    {
                        // Check that the chosen key is not used by another control
                        bool bCanSetKey = false;
                        // Exception to this rule : Escape can be affected to both pause buttons, but no other option
                        if (event.key.keysym.sym == SDLK_ESCAPE )
                        {
                            m_KeyPressCountdown.Restart();
                            this->m_MessageBox.SetLineString(0, "Escape is already set to pausing at all times.", m_FontMenu, C_FontProps::MENUELTFONTR, C_FontProps::MENUELTFONTG, C_FontProps::MENUELTFONTB);
                            this->m_MessageBox.SetLineString(1, "Please press another key or wait 5 seconds.", m_FontMenu, C_FontProps::MENUELTFONTR, C_FontProps::MENUELTFONTG, C_FontProps::MENUELTFONTB);
                            this->m_MessageBox.SetLineString(2, "---", m_FontMenu, C_FontProps::MENUELTFONTR, C_FontProps::MENUELTFONTG, C_FontProps::MENUELTFONTB);
                            this->m_MessageBox.SetLineString(3, "5", m_FontMenu, C_FontProps::MENUELTFONTR, C_FontProps::MENUELTFONTG, C_FontProps::MENUELTFONTB);
                        }
                        else
                        {
                            bool bAlreadyUsed = false;

                            // Cycle through the already bound keys
                            for (unsigned long i = this->MENU_P1KEYUP; i <= this->MENU_P2KEYPAUSE; i++)
                            {
                                switch (i)
                                {
                                case MENU_P1P2SEPARATOR: // Ignore separators
                                    break;
                                default:
                                    if (this->m_MenuCtl.GetValue(i) == (long)event.key.keysym.sym )
                                    {
                                        bAlreadyUsed = true;
                                        this->m_MessageBox.SetLineString(0, "The " + (std::string)SDL_GetKeyName(event.key.keysym.sym) + " key is already used by " + this->m_MenuCtl.GetLabel(i) + ".", m_FontMenu, C_FontProps::MENUELTFONTR, C_FontProps::MENUELTFONTG, C_FontProps::MENUELTFONTB);
                                        this->m_MessageBox.SetLineString(1, "Please press another key or wait 5 seconds.", m_FontMenu, C_FontProps::MENUELTFONTR, C_FontProps::MENUELTFONTG, C_FontProps::MENUELTFONTB);
                                        this->m_MessageBox.SetLineString(2, "---", m_FontMenu, C_FontProps::MENUELTFONTR, C_FontProps::MENUELTFONTG, C_FontProps::MENUELTFONTB);
                                        this->m_MessageBox.SetLineString(3, "5", m_FontMenu, C_FontProps::MENUELTFONTR, C_FontProps::MENUELTFONTG, C_FontProps::MENUELTFONTB);
                                        this->m_KeyPressCountdown.Restart();
                                    }
                                    break;
                                }
                            }

                            if (!bAlreadyUsed)
                            {
                                bCanSetKey = true;
                            }
                        }

                        if (bCanSetKey)
                        {
                            this->m_MenuCtl.SetValueNumAndString(this->m_MenuCtl.GetSelectedEntry(), event.key.keysym.sym, SDL_GetKeyName(event.key.keysym.sym));
                            this->m_KeyPressCountdown.Restart(0);
                            this->SetMenuStatus(MENU_INMENU);
                        }

                    }
                break;
            }
            break;
        default:
            break;
        }
    }

    if (bCausesExitState)
    {
        // start fading out effect
        this->SetMenuStatus(KeyConfigState::MENU_FADEOUT);
        this->m_Fader.SetFadingType(FaderColor::FADEOUT);
        this->m_Fader.SetFadingLength(C_FadingTimers::FADEOUT);
        this->m_Fader.StartFadingNow();
        this->m_SDL.audio().StopCurrentMusic();
    }

    // Traitements automatisés (timers)
    switch (this->m_MenuStatus)
    {
        case KeyConfigState::MENU_FADEIN:
            if (m_Fader.isFadingDone())
            {
                this->SetMenuStatus(KeyConfigState::MENU_INMENU);
            }
        break;

        case KeyConfigState::MENU_INMENU:
            // Aucun traitement automatisé
        break;

        case KeyConfigState::MENU_WAITFORINPUT:
            // Aucun traitement automatisé
            if (m_KeyPressCountdown.IsTimerEnd())
            {
                this->SetMenuStatus(MENU_INMENU);
            }
        break;

        case KeyConfigState::MENU_FADEOUT:
            if (m_Fader.isFadingDone())
                return false;
        break;
    }

    // Maj timers d'interface
    // Arrière plan défilant
    if ( m_BGScrollEffect.IsTimerEnd() )
    {
        m_BGScrollEffect.Restart( m_BGScrollEffect.GetOvertime() );
    }

    return true;
}

bool KeyConfigState::Frame()
{
    m_SDL.window().EmptyFrame();

    // Arrière plan défilant
    int ScreenW = m_SDL.window().GetWidth();
    int ScreenH = m_SDL.window().GetHeight();

    long BGSizeH = this->m_texList.GetTextureByID(TitleTex::TX_BACKGROUNDSCROLL).getH();
    long BGSizeW = this->m_texList.GetTextureByID(TitleTex::TX_BACKGROUNDSCROLL).getW();

    // Variables tampon
    long DrawX = 0;
    long DrawY = 0;

    long OffsetX = ( (float)BGSizeW * this->m_BGScrollEffect.GetElapsedPc() );
    long OffsetY = ( (float)BGSizeH * this->m_BGScrollEffect.GetElapsedPc() );

    for (int x = -1; x < ((ScreenW + BGSizeW) / BGSizeW); x++)
    {
        for (int y = -1; y < ((ScreenH + BGSizeH)  / BGSizeH); y++)
        {
            DrawX = OffsetX + (x * BGSizeW);
            DrawY = OffsetY + (y * BGSizeH);
            this->m_texList.GetTextureByID(TitleTex::TX_BACKGROUNDSCROLL).RenderToWindow(m_SDL.window(), DrawX, DrawY);
        }
    }

    switch ( this->GetMenuStatus() )
    {
        case KeyConfigState::MENU_INMENU:
            // Drawing menu
            this->m_MenuCtl.RenderToWindow(m_SDL.window(), 80, 10, this->m_FontMenu);
        break;
        case KeyConfigState::MENU_WAITFORINPUT:
            // Countdown
            this->m_MessageBox.SetLineString(3, SPConv::NumToStr((this->m_KeyPressCountdown.GetRemaining() / 1000) + 1), m_FontMenu, C_FontProps::MENUELTFONTR, C_FontProps::MENUELTFONTG, C_FontProps::MENUELTFONTB);
            this->m_MessageBox.RenderToWindow(m_SDL.window(), 320, 240, true);
            break;
        default:
        break;
    }

    #ifdef DEBUGMODE
        // FPS
        this->m_TextRenderer.RenderTextOnScreen(m_SDL.window(), 0, 0, SPConv::NumToStr(m_SDL.window().FPSLimiter().GetLastFPS()) );
    #endif

    // Fondu
    this->m_Fader.RenderToWindow( m_SDL.window() );

    m_SDL.window().DisplayFrame();

    return true;
}

bool KeyConfigState::LoadResources()
{
    TitleTex::LoadTitleTex( this->m_texList );

    TitleSnd::LoadTitleSnd( this->m_sndList );

    // Texte
    this->m_TextRenderer.LoadFont("./fonts/armandc-ascii-font-8.png", 8, 8);

    // Texte TTF
    this->m_FontMenu.LoadFont("./fonts/BubblegumSans-Regular.otf", C_FontProps::SMALLFONTHEIGHT);
    this->m_FontMessage.LoadFont("./fonts/BubblegumSans-Regular.otf", C_FontProps::MEDIUMFONTHEIGHT);

    // Paramétrage du menu principal
    this->m_MenuCtl.SetMenuWidth(480);

    std::vector<std::string> tmpStr;
    tmpStr.push_back( SDL_GetKeyName(m_Controls.p1_Up) );
    this->m_MenuCtl.AddMenuEntry("P1 Up", MenuControl::TYPE_NUMANDSTRING, m_Controls.p1_Up, 0, 0, tmpStr);
    tmpStr.clear();
    tmpStr.push_back(SDL_GetKeyName(m_Controls.p1_Down));
    this->m_MenuCtl.AddMenuEntry("P1 Down", MenuControl::TYPE_NUMANDSTRING, m_Controls.p1_Down, 0, 0, tmpStr);
    tmpStr.clear();
    tmpStr.push_back(SDL_GetKeyName(m_Controls.p1_Left));
    this->m_MenuCtl.AddMenuEntry("P1 Left", MenuControl::TYPE_NUMANDSTRING, m_Controls.p1_Left, 0, 0, tmpStr);
    tmpStr.clear();
    tmpStr.push_back(SDL_GetKeyName(m_Controls.p1_Right));
    this->m_MenuCtl.AddMenuEntry("P1 Right", MenuControl::TYPE_NUMANDSTRING, m_Controls.p1_Right, 0, 0, tmpStr);
    tmpStr.clear();
    tmpStr.push_back(SDL_GetKeyName(m_Controls.p1_Swap));
    this->m_MenuCtl.AddMenuEntry("P1 Swap", MenuControl::TYPE_NUMANDSTRING, m_Controls.p1_Swap, 0, 0, tmpStr);
    tmpStr.clear();
    tmpStr.push_back(SDL_GetKeyName(m_Controls.p1_Raise));
    this->m_MenuCtl.AddMenuEntry("P1 Raise", MenuControl::TYPE_NUMANDSTRING, m_Controls.p1_Raise, 0, 0, tmpStr);
    tmpStr.clear();
    tmpStr.push_back(SDL_GetKeyName(m_Controls.p1_Pause));
    this->m_MenuCtl.AddMenuEntry("P1 Pause", MenuControl::TYPE_NUMANDSTRING, m_Controls.p1_Pause, 0, 0, tmpStr);
    tmpStr.clear();

    this->m_MenuCtl.AddMenuEntry("", MenuControl::TYPE_SPACER);

    tmpStr.push_back(SDL_GetKeyName(m_Controls.p2_Up));
    this->m_MenuCtl.AddMenuEntry("P2 Up", MenuControl::TYPE_NUMANDSTRING, m_Controls.p2_Up, 0, 0, tmpStr);
    tmpStr.clear();
    tmpStr.push_back(SDL_GetKeyName(m_Controls.p2_Down));
    this->m_MenuCtl.AddMenuEntry("P2 Down", MenuControl::TYPE_NUMANDSTRING, m_Controls.p2_Down, 0, 0, tmpStr);
    tmpStr.clear();
    tmpStr.push_back(SDL_GetKeyName(m_Controls.p2_Left));
    this->m_MenuCtl.AddMenuEntry("P2 Left", MenuControl::TYPE_NUMANDSTRING, m_Controls.p2_Left, 0, 0, tmpStr);
    tmpStr.clear();
    tmpStr.push_back(SDL_GetKeyName(m_Controls.p2_Right));
    this->m_MenuCtl.AddMenuEntry("P2 Right", MenuControl::TYPE_NUMANDSTRING, m_Controls.p2_Right, 0, 0, tmpStr);
    tmpStr.clear();
    tmpStr.push_back(SDL_GetKeyName(m_Controls.p2_Swap));
    this->m_MenuCtl.AddMenuEntry("P2 Swap", MenuControl::TYPE_NUMANDSTRING, m_Controls.p2_Swap, 0, 0, tmpStr);
    tmpStr.clear();
    tmpStr.push_back(SDL_GetKeyName(m_Controls.p2_Raise));
    this->m_MenuCtl.AddMenuEntry("P2 Raise", MenuControl::TYPE_NUMANDSTRING, m_Controls.p2_Raise, 0, 0, tmpStr);
    tmpStr.clear();
    tmpStr.push_back(SDL_GetKeyName(m_Controls.p2_Pause));
    this->m_MenuCtl.AddMenuEntry("P2 Pause", MenuControl::TYPE_NUMANDSTRING, m_Controls.p2_Pause, 0, 0, tmpStr);
    tmpStr.clear();

    // Messagebox properties
    this->m_MessageBox.SetLinesInMessage(4);
    this->m_MessageBox.SetTextCenter(true);

    this->m_MenuCtl.AddMenuEntry("", MenuControl::TYPE_SPACER);
    this->m_MenuCtl.AddMenuEntry("Save & Quit", MenuControl::TYPE_VALIDATOR);
    this->m_MenuCtl.AddMenuEntry("Exit without saving", MenuControl::TYPE_VALIDATOR);
    this->m_MenuCtl.AddMenuEntry("Reset Defaults", MenuControl::TYPE_VALIDATOR);

    return true;
}

bool KeyConfigState::UnloadResources()
{
    this->m_texList.UnloadEverything();

    this->m_SDL.audio().StopCurrentMusic();
    this->m_sndList.UnloadAllMusic();
    this->m_sndList.UnloadAllSamples();

    return true;
}
