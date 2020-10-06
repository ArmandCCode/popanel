#include "TitleState.h"

TitleState::TitleState(SPSDL& p_SDLRef, StateRequestObject& p_StateRequest, KeyboardControls& p_ControlsSettings, MatchSettings& p_MatchSettings)
    : SPGameState( p_SDLRef ),
    m_stateRequest(p_StateRequest),
    m_Controls(p_ControlsSettings),
    m_sndList( p_SDLRef.audio() ),
    m_MatchSettings(p_MatchSettings)
{

    this->LoadResources();
    this->SetMenuStatus(TitleState::MENU_FADEIN);
    this->m_Fader.SetFadingType(FaderColor::FADEIN);
    this->m_Fader.SetFadingLength(C_FadingTimers::FADEIN);
    this->m_Fader.StartFadingNow();
    this->m_BGScrollEffect.SetLength(6000);
    this->m_SDL.audio().SetMusic(this->m_sndList.GetMusic(TitleSnd::MUS_MENU));
    this->m_SDL.audio().PlayCurrentMusic();
}

TitleState::~TitleState()
{

    this->UnloadResources();
}

bool TitleState::Play(long p_Delta)
{
    // fading out effect
    bool bCausesExitState = false;

    // Traitements évènementiels
    this->m_SDL.input().ConvertSDLEventsToSPEvents();

    // Quit the app immediately
    if (this->m_SDL.input().IsExitEventPending())
    {
        this->m_stateRequest.SetDesiredState( StateRequestObject::CLOSEAPP );
        return false;
    }

    SPEvent event;
    while (this->m_SDL.input().SPPollEvent(event))
    {
        if (event.IsPressEvent())
        {
            switch (this->m_MenuStatus)
            {
                case TitleState::MENU_PRESSANYKEY:
                    // Afficher le menu
                    this->SetMenuStatus(TitleState::MENU_INMENU);
                    this->m_SDL.audio().PlaySample(m_sndList.GetSample(TitleSnd::SPL_VALIDATE), 1);
                    this->m_HelpScroll.RestartScroll();
                break;

                case TitleState::MENU_INMENU:
                    switch (event.GetActionNumber())
                    {
                        case C_Actions::MENUUP:
                            this->m_SDL.audio().PlaySample(m_sndList.GetSample(TitleSnd::SPL_CURSORMOVE), 2);
                            this->m_MenuCtl.PrevEntry();
                            break;
                        case C_Actions::MENUDOWN:
                            this->m_SDL.audio().PlaySample(m_sndList.GetSample(TitleSnd::SPL_CURSORMOVE), 2);
                            this->m_MenuCtl.NextEntry();
                            break;
                        case C_Actions::MENULEFT:
                            this->m_SDL.audio().PlaySample(m_sndList.GetSample(TitleSnd::SPL_CURSORMOVE), 2);
                            this->m_MenuCtl.PrevValue(this->m_MenuCtl.GetSelectedEntry());
                            break;
                        case C_Actions::MENURIGHT:
                            this->m_SDL.audio().PlaySample(m_sndList.GetSample(TitleSnd::SPL_CURSORMOVE), 2);
                            this->m_MenuCtl.NextValue(this->m_MenuCtl.GetSelectedEntry());
                            break;
                        case C_Actions::MENUVALIDATE:
                            this->m_SDL.audio().PlaySample(m_sndList.GetSample(TitleSnd::SPL_VALIDATE), 1);
                            if (this->m_MenuCtl.GetSelectedEntry() == MENU_STARTGAME)
                            {
                                // Récupérer les valeurs du menu
                                this->m_MatchSettings.SetNumPlayers(this->m_MenuCtl.GetValue(MENU_NUMPLAYERS));
                                this->m_MatchSettings.SetDifficulty(this->m_MenuCtl.GetValue(MENU_DIFFICULTY));
                                this->m_MatchSettings.SetInitialSpeed(this->m_MenuCtl.GetValue(MENU_STACKSPEED));

                                this->m_MatchSettings.SetGameDuration(120000); // Pour l'instant, la durée n'est pas personnalisable : 2 minutes
                                this->m_stateRequest.SetDesiredState( StateRequestObject::GAME );
                                bCausesExitState = true;

                            }
                            else if (this->m_MenuCtl.GetSelectedEntry() == MENU_QUITGAME)
                            {
                                this->m_stateRequest.SetDesiredState( StateRequestObject::CLOSEAPP );
                                bCausesExitState = true;
                            }
                            else if (this->m_MenuCtl.GetSelectedEntry() == MENU_KEYCONFIG)
                            {
                                this->m_stateRequest.SetDesiredState( StateRequestObject::KEYCONFIG );
                                bCausesExitState = true;
                            }
                            else
                            {
                                this->m_MenuCtl.JumpToEntry(MENU_STARTGAME);
                            }
                            break;
                        case C_Actions::MENUCANCEL:
                            this->m_SDL.audio().PlaySample(m_sndList.GetSample(TitleSnd::SPL_VALIDATE), 1);
                            this->m_MenuCtl.JumpToEntry(MENU_QUITGAME);
                            break;
                    }

                break;
                default:
                    // Rien a faire pendant les transitions

                break;
            }
        }
    }

    if (bCausesExitState)
    {
        // start fading out effect
        this->SetMenuStatus(TitleState::MENU_FADEOUT);
        this->m_Fader.SetFadingType(FaderColor::FADEOUT);
        this->m_Fader.SetFadingLength(C_FadingTimers::FADEOUT);
        this->m_Fader.StartFadingNow();
        this->m_SDL.audio().StopCurrentMusic();
    }

    // Traitements automatisés (timers)
    switch (this->m_MenuStatus)
    {
        case TitleState::MENU_FADEIN:
            if (m_Fader.isFadingDone())
            {
                this->SetMenuStatus(TitleState::MENU_PRESSANYKEY);
            }
        break;

        case TitleState::MENU_PRESSANYKEY:
            // Aucun traitement automatisé
        break;

        case TitleState::MENU_INMENU:
            // Aucun traitement automatisé
        break;

        case TitleState::MENU_FADEOUT:
            if (m_Fader.isFadingDone())
                return false;
        break;
    }

    // UI timers
    // Scrolling background
    if ( m_BGScrollEffect.IsTimerEnd() )
    {
        m_BGScrollEffect.Restart( m_BGScrollEffect.GetOvertime() );
    }

    return true;
}

bool TitleState::Frame()
{
    m_SDL.window().EmptyFrame();

    // Scrolling background
    int ScreenW = m_SDL.window().GetWidth();
    int ScreenH = m_SDL.window().GetHeight();

    long BGSizeH = this->m_texList.GetTextureByID(TitleTex::TX_BACKGROUNDSCROLL).getH();
    long BGSizeW = this->m_texList.GetTextureByID(TitleTex::TX_BACKGROUNDSCROLL).getW();

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

    // Draw floating title
    const double wobbleAmplitude = 16.00;
    int wobbleOffset = (double)(wobbleAmplitude * sin( ((SDL_GetTicks() % 5000) / 5000.00) * (2 * M_PI)) );
    DrawX = 320;
    DrawY = 128 + wobbleOffset ;
    this->m_texList.GetTextureByID(TitleTex::TX_LOGO).RenderToWindow(m_SDL.window(), DrawX, DrawY, true);

    switch ( this->GetMenuStatus() )
    {
        case TitleState::MENU_PRESSANYKEY:
            this->m_PressAnyKey.RenderToWindow(m_SDL.window(), 320, 360, true);
        break;
        case TitleState::MENU_INMENU:
            // Dessin du menu de jeu
            this->m_MenuCtl.RenderToWindow(m_SDL.window(), 160, 260, this->m_FontMenu);
            // Dessin du bandeau d'aide
            this->m_HelpScroll.RenderToWindow(m_SDL.window(), 10, 440);
        break;
        default:
            // Rien.
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

bool TitleState::LoadResources()
{

    TitleTex::LoadTitleTex( this->m_texList );

    TitleSnd::LoadTitleSnd( this->m_sndList );

    // Texte
    this->m_TextRenderer.LoadFont("./fonts/armandc-ascii-font-8.png", 8, 8);

    // Texte TTF
    this->m_FontMenu.LoadFont("./fonts/BubblegumSans-Regular.otf", C_FontProperties::SMALLFONTHEIGHT);
    this->m_FontMessage.LoadFont("./fonts/BubblegumSans-Regular.otf", C_FontProperties::MEDIUMFONTHEIGHT);

    // Paramétrage du menu principal
    this->m_MenuCtl.SetMenuWidth(320);
    this->m_MenuCtl.AddMenuEntry("Players", MenuControl::TYPE_NUMSELECTOR, 1, 1, 2);
    std::vector<std::string> tmpDiffs;
    tmpDiffs.push_back("Easy");
    tmpDiffs.push_back("Medium");
    tmpDiffs.push_back("Hard");
    this->m_MenuCtl.AddMenuEntry("Difficulty", MenuControl::TYPE_STRINGLIST, 0, 0, 2, tmpDiffs );
    this->m_MenuCtl.AddMenuEntry("Stack Speed", MenuControl::TYPE_NUMSELECTOR, 0, 0, 99);
    this->m_MenuCtl.AddMenuEntry("Keyboard Configuration", MenuControl::TYPE_VALIDATOR);
    this->m_MenuCtl.AddMenuEntry("", MenuControl::TYPE_SPACER);
    this->m_MenuCtl.AddMenuEntry("Start Game !", MenuControl::TYPE_VALIDATOR);
    this->m_MenuCtl.AddMenuEntry("Quit Game", MenuControl::TYPE_VALIDATOR);

    // "Press any key"
    this->m_PressAnyKey.SetLinesInMessage(4);
    this->m_PressAnyKey.SetLineString(0, "Developed by Armand Christophe", m_FontMessage, C_FontProperties::MENUELTFONTR, C_FontProperties::MENUELTFONTG, C_FontProperties::MENUELTFONTB);
    this->m_PressAnyKey.SetLineString(1, "www.armandchristophe.com", m_FontMessage, C_FontProperties::MENUELTFONTR, C_FontProperties::MENUELTFONTG, C_FontProperties::MENUELTFONTB);
    this->m_PressAnyKey.SetLineString(2, "-", m_FontMessage, C_FontProperties::MENUELTFONTR, C_FontProperties::MENUELTFONTG, C_FontProperties::MENUELTFONTB);
    this->m_PressAnyKey.SetLineString(3, "Press any key", m_FontMessage, C_FontProperties::MENUELTFONTR, C_FontProperties::MENUELTFONTG, C_FontProperties::MENUELTFONTB);
    this->m_PressAnyKey.SetMessageBoxWidth(420);
    this->m_PressAnyKey.SetTextCenter(true);

    // Indique les commandes
    this->m_HelpScroll.SetString("Menu controls - "
                                 + std::string(SDL_GetKeyName(m_Controls.m_Up))
                                 + "/" + SDL_GetKeyName(m_Controls.m_Down) + " : Select option - "
                                 + SDL_GetKeyName(m_Controls.m_Left) + "/" + SDL_GetKeyName(m_Controls.m_Right) + " : Modify option - "
                                 + SDL_GetKeyName(m_Controls.m_Validate) + " : Validate"
                                 , m_FontMenu, C_FontProperties::MENUELTFONTR, C_FontProperties::MENUELTFONTG, C_FontProperties::MENUELTFONTB);
    this->m_HelpScroll.SetScrollerSpeed(80);
    this->m_HelpScroll.SetScrollerWidth(620);

    return true;
}

bool TitleState::UnloadResources()
{
    this->m_texList.UnloadEverything();

    this->m_SDL.audio().StopCurrentMusic();
    this->m_sndList.UnloadAllMusic();
    this->m_sndList.UnloadAllSamples();

    return true;
}
