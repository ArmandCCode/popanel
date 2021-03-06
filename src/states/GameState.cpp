#include "GameState.h"

GameState::GameState(SPSDL& p_SDLRef, StateRequestObject& p_StateRequest, KeyboardControls& p_ControlsSettings, MatchSettings& p_MatchSettings)
    : SPGameState(p_SDLRef),
    m_stateRequest(p_StateRequest),
    m_Controls(p_ControlsSettings),
    m_MatchSettings(p_MatchSettings),
    m_StrBufGameElements(6),
    m_StrBufGameValues(6),
    m_StrBufMenuElements(1),
    m_sndList( p_SDLRef.audio() ),
    m_isPanicMusic(false), m_CursorBlink(true), m_BlockBlink(true), m_IsFrameAdvance(false),
    m_GameStatus(0), m_PreviousGameStatus(0)
{

    // On prend une nouvelle seed pour les nombres al�atoires
    NumHelper::RandomizeSeed(SDL_GetTicks());

    // Calcul de la graine de blocs
    p_MatchSettings.CreateNewBlockSet();

    // Initialisation des joueurs
    for (unsigned int nPlayer = 0; nPlayer < p_MatchSettings.GetNumPlayers() ; nPlayer++ )
    {
        Player tmpPlayer;
        this->m_Player.push_back(tmpPlayer);
        this->m_CurDir.push_back(-1);
        SPTimer tmpTimer;
        this->m_CurMoveTimer.push_back(tmpTimer);
    }

    // ressources vid�o et audio (le chargement est mis ici car certaines ressources d�pendent du nombre de joueurs)
    this->LoadResources();

    for (unsigned int nPlayer = 0; nPlayer < this->m_Player.size() ; nPlayer++ )
    {
        this->m_Player[nPlayer].GetGameTimer().SetLength( p_MatchSettings.GetGameDuration() );
        this->m_Player[nPlayer].GetGameTimer().SetPeriod(1000); // Toutes les secondes, cela aidera � faire le d�compte de fin
        this->m_Player[nPlayer].SetDifficulty( m_MatchSettings.GetDifficulty() );
        this->m_Player[nPlayer].GetField().SetSpeed( m_MatchSettings.GetInitialSpeed() );
        this->m_Player[nPlayer].GetField().ResetNewLineTimer();
        this->m_PanelGame.InitPlayer( this->m_Player[nPlayer], m_MatchSettings);
        this->m_CurMoveTimer[nPlayer].SetLength(500); // 500ms au premier coup, 100ms aux suivants
    }

    // Calcul du gagnant
    this->m_MatchWinner = GameState::WINNER_NONE;

    // Timers Graphismes et effets graphiques
    this->m_BlockEffect.SetLength(60);
    this->m_CursorEffect.SetLength(400);
    this->m_BGScrollEffect.SetLength(6000);

    // Fondu de d�part
    this->SetGameStatus(GameState::GAME_FADEIN);
    this->m_Fader.SetFadingLength(C_FadingTimers::FADEIN);
    this->m_Fader.StartFadingNow();

    // Timers jeu
    this->m_Countdown.SetPeriod(1000);
    this->m_Countdown.SetLength(3000);

    // Musiques et effets sonores
    m_SDL.audio().SetMusic( this->m_sndList.GetMusic(GameSnd::MUS_MAIN) );
}

GameState::~GameState()
{

    this->UnloadResources();
}

void GameState::PauseAnimationTimers()
{
    this->m_BlockEffect.PauseTimer();
    this->m_CursorEffect.PauseTimer();
    this->m_BGScrollEffect.PauseTimer();
}

void GameState::ResumeAnimationTimers()
{
    this->m_BlockEffect.ResumeTimer();
    this->m_CursorEffect.ResumeTimer();
    this->m_BGScrollEffect.ResumeTimer();
}

void GameState::SetGameStatus(int val)
{
    // Save the previous game status (useful for pausing)
    m_PreviousGameStatus = m_GameStatus;
    m_GameStatus = val;
}

void GameState::GoToPreviousGameStatus()
{
    // Used to help the pause status
    SetGameStatus(GetPreviousGameStatus());
}

bool GameState::Play(long p_Delta)
{
    static const int CURDIR_UP = 0;
    static const int CURDIR_DOWN = 1;
    static const int CURDIR_LEFT = 2;
    static const int CURDIR_RIGHT = 3;

    this->m_SDL.input().ConvertSDLEventsToSPEvents();

    if (this->m_SDL.input().IsExitEventPending())
    {
        this->m_stateRequest.SetDesiredState(StateRequestObject::CLOSEAPP);
        return false;
    }

    // fading out effect
    bool bCausesExitState = false;

    // Everything else depends on the status of the game
    switch (this->GetGameStatus())
    {
        case GameState::GAME_FADEIN:
            // No action to take during fading
        break;

        case GameState::GAME_INTRO:
            {
                SPEvent event;

                while(this->m_SDL.input().SPPollEvent(event))
                {
                    if (event.IsPressEvent() && (event.GetActionNumber() == C_Actions::PAUSE || event.GetActionNumber() == C_Actions::MENUCANCEL ) ) // TODO:Executed two times. Give priority to one player or find something else...
                    {
                        this->SetGameStatus(GameState::GAME_PAUSED);
                        m_SDL.audio().PauseCurrentMusic();
                        this->PauseAnimationTimers();
                    }
                }
            }

        break;

        case GameState::GAME_PAUSED:
            {
                SPEvent event;

                while(this->m_SDL.input().SPPollEvent(event))
                {
                    if (event.IsPressEvent())
                    {
                        switch(event.GetActionNumber())
                        {
                            case C_Actions::MENUUP:
                                m_SDL.audio().PlaySample(this->m_sndList.GetSample(GameSnd::SPL_CURSORMOVE), 1);
                                this->m_PauseMenu.PrevEntry();
                                break;
                            case C_Actions::MENUDOWN:
                                m_SDL.audio().PlaySample(this->m_sndList.GetSample(GameSnd::SPL_CURSORMOVE), 1);
                                this->m_PauseMenu.NextEntry();
                                break;
                            case C_Actions::MENUVALIDATE:
                                m_SDL.audio().PlaySample(this->m_sndList.GetSample(GameSnd::SPL_VALIDATE), 1);
                                switch (this->m_PauseMenu.GetSelectedEntry())
                                {
                                    case GameState::MENU_CONTINUE:
                                    {
                                        this->GoToPreviousGameStatus();
                                        this->ResumeAnimationTimers();

                                        if ( this->GetPreviousGameStatus() == GameState::GAME_INTRO )
                                        {
                                            this->m_TempStatusTimer.ResumeTimer();
                                        }
                                        else
                                        {
                                            m_SDL.audio().ResumeCurrentMusic();
                                        }

                                    }
                                    break;

                                    case GameState::MENU_QUIT:
                                        this->m_stateRequest.SetDesiredState(StateRequestObject::TITLE);
                                        bCausesExitState = true;
                                    break;
                                }
                                break;
                            case C_Actions::MENUCANCEL:
                            case C_Actions::PAUSE:
                                this->m_PauseMenu.JumpToEntry(GameState::MENU_CONTINUE);
                                break;
                        }
                    }
                }
            }
            break;

        case GameState::GAME_PLAYING:
            {
                // Cursor move repetition
                for (unsigned int nPlayer = 0; nPlayer < this->m_Player.size(); nPlayer++ )
                {

                    if ( this->m_CurDir[nPlayer] >= 0 && this->m_CurDir[nPlayer] <= 3 )
                    {
                        while ( this->m_CurMoveTimer[nPlayer].IsTimerEnd() )
                        {
                            bool cursorHasMoved = false;

                            switch ( this->m_CurDir[nPlayer] )
                            {
                            case CURDIR_UP:
                                cursorHasMoved = this->m_PanelGame.RequestCursorMoveUp( this->m_Player[nPlayer] );
                                break;
                            case CURDIR_DOWN:
                                cursorHasMoved = this->m_PanelGame.RequestCursorMoveDown( this->m_Player[nPlayer] );
                                break;
                            case CURDIR_LEFT:
                                cursorHasMoved = this->m_PanelGame.RequestCursorMoveLeft( this->m_Player[nPlayer] );
                                break;
                            case CURDIR_RIGHT:
                                cursorHasMoved = this->m_PanelGame.RequestCursorMoveRight( this->m_Player[nPlayer] );
                                break;
                            default:
                                // this case is here just to clear up a compiler warning
                                break;
                            }

                            this->m_CurMoveTimer[nPlayer].Restart( this->m_CurMoveTimer[nPlayer].GetOvertime() );
                            this->m_CurMoveTimer[nPlayer].SetLength(20);

                            if (cursorHasMoved)
                            {
                                this->m_ThisFrameEvent.AddEvent(GameEvents::EVT_CURSORMOVE);
                            }
                        }
                    }
                }

                // Player input events
                SPEvent event;

                while(this->m_SDL.input().SPPollEvent(event))
                {
                    if (event.IsPressEvent() && (event.GetPlayerNumber() <= m_Player.size()) ) // Key Down + filter out non active players
                    {
                        int playerArrayId = event.GetPlayerNumber() - 1;
                        bool cursorHasMoved = false;

                        switch(event.GetActionNumber())
                        {
                            case C_Actions::PAUSE:
                            case C_Actions::MENUCANCEL:
                                // Pause action gets priority
                                this->SetGameStatus(GameState::GAME_PAUSED);
                                m_SDL.audio().PauseCurrentMusic();
                                this->PauseAnimationTimers();
                                break;
                            case C_Actions::UP:
                                if ( this->m_PanelGame.RequestCursorMoveUp( this->m_Player[playerArrayId] ) )
                                {
                                    this->m_CurDir[playerArrayId] = CURDIR_UP;
                                    cursorHasMoved = true;
                                }
                                break;
                            case C_Actions::DOWN:
                                if (this->m_PanelGame.RequestCursorMoveDown( this->m_Player[playerArrayId] ))
                                {
                                    this->m_CurDir[playerArrayId] = CURDIR_DOWN;
                                    cursorHasMoved = true;
                                }
                                break;
                            case C_Actions::LEFT:
                                if (this->m_PanelGame.RequestCursorMoveLeft( this->m_Player[playerArrayId] ))
                                {
                                    this->m_CurDir[playerArrayId] = CURDIR_LEFT;
                                    cursorHasMoved = true;
                                }
                                break;
                            case C_Actions::RIGHT:
                                if (this->m_PanelGame.RequestCursorMoveRight( this->m_Player[playerArrayId] ))
                                {
                                    this->m_CurDir[playerArrayId] = CURDIR_RIGHT;
                                    cursorHasMoved = true;
                                }
                                break;
                            case C_Actions::SWAP:
                                if ( this->m_Countdown.IsTimerEnd() )
                                {
                                    this->m_PanelGame.RequestSwapping( this->m_Player[playerArrayId] , this->m_ThisFrameEvent);
                                }
                                break;
                            case C_Actions::RAISE:
                                this->m_PanelGame.RequestFieldRise( this->m_Player[playerArrayId] );
                                break;
                        }

                        if (cursorHasMoved)
                        {
                            this->m_CurMoveTimer[playerArrayId].SetLength(200);
                            this->m_CurMoveTimer[playerArrayId].Restart();
                            this->m_ThisFrameEvent.AddEvent(GameEvents::EVT_CURSORMOVE);
                        }

                    }
                    else // Key Up
                    {
                        int playerArrayId = event.GetPlayerNumber() - 1;
                        switch(event.GetActionNumber())
                        {
                            case C_Actions::UP:
                                if (this->m_CurDir[playerArrayId] == CURDIR_UP)
                                {
                                    this->m_CurMoveTimer[playerArrayId].Restart(0);
                                    this->m_CurDir[playerArrayId] = -1;
                                }
                                break;
                            case C_Actions::DOWN:
                                if (this->m_CurDir[playerArrayId] == CURDIR_DOWN)
                                {
                                    this->m_CurMoveTimer[playerArrayId].Restart(0);
                                    this->m_CurDir[playerArrayId] = -1;
                                }
                                break;
                            case C_Actions::LEFT:
                                if (this->m_CurDir[playerArrayId] == CURDIR_LEFT)
                                {
                                    this->m_CurMoveTimer[playerArrayId].Restart(0);
                                    this->m_CurDir[playerArrayId] = -1;
                                }
                                break;
                            case C_Actions::RIGHT:
                                if (this->m_CurDir[playerArrayId] == CURDIR_RIGHT)
                                {
                                    this->m_CurMoveTimer[playerArrayId].Restart(0);
                                    this->m_CurDir[playerArrayId] = -1;
                                }
                                break;
                            case C_Actions::RAISE:
                                this->m_PanelGame.RequestFieldRiseStop( this->m_Player[playerArrayId] );
                                break;
                        }
                    }
                }
            }

        break;

        case GameState::GAME_FINISHED:
        break;

        case GameState::GAME_OUTRO:
            {
                SPEvent event;

                while(this->m_SDL.input().SPPollEvent(event))
                {
                    if (event.IsPressEvent())
                    {
                        switch(event.GetActionNumber())
                        {
                            case C_Actions::MENUUP:
                                m_SDL.audio().PlaySample(this->m_sndList.GetSample(GameSnd::SPL_CURSORMOVE), 1);
                                this->m_PauseMenu.PrevEntry();
                                break;
                            case C_Actions::MENUDOWN:
                                m_SDL.audio().PlaySample(this->m_sndList.GetSample(GameSnd::SPL_CURSORMOVE), 1);
                                this->m_PauseMenu.NextEntry();
                                break;
                            case C_Actions::MENUVALIDATE:
                                m_SDL.audio().PlaySample(this->m_sndList.GetSample(GameSnd::SPL_VALIDATE), 1);
                                switch (this->m_PauseMenu.GetSelectedEntry())
                                {
                                    case GameState::MENU_CONTINUE:
                                    {
                                        this->GoToPreviousGameStatus();
                                        this->ResumeAnimationTimers();

                                        if ( this->GetPreviousGameStatus() == GameState::GAME_INTRO )
                                        {
                                            this->m_TempStatusTimer.ResumeTimer();
                                        }
                                        else
                                        {
                                            m_SDL.audio().ResumeCurrentMusic();
                                        }

                                    }
                                    break;

                                    case GameState::MENU_QUIT:
                                        this->m_stateRequest.SetDesiredState(StateRequestObject::TITLE);
                                        bCausesExitState = true;
                                    break;
                                }
                                break;
                            case C_Actions::MENUCANCEL:
                            case C_Actions::PAUSE:
                                this->m_PauseMenu.JumpToEntry(GameState::MENU_QUIT);
                                break;
                        }
                    }
                }
            }
        break;

        case GameState::GAME_FADEOUT:
        break;

    }

    if (bCausesExitState)
    {
        // start fading out effect
        this->SetGameStatus(GameState::GAME_FADEOUT);
        this->m_Fader.SetFadingType(FaderColor::FADEOUT);
        this->m_Fader.SetFadingLength(C_FadingTimers::FADEOUT);
        this->m_Fader.StartFadingNow();
        this->m_SDL.audio().StopCurrentMusic();
    }

    // Traitements automatiques
    switch (this->GetGameStatus())
    {
        case GameState::GAME_FADEIN:
            if ( m_Fader.isFadingDone() )
            {
                this->SetGameStatus(GameState::GAME_INTRO);
                this->m_TempStatusTimer.SetLength(C_GameStateEventTimers::INTRO);
                this->m_TempStatusTimer.Restart();
            }
        break;

        case GameState::GAME_INTRO:
            if ( this->m_TempStatusTimer.IsTimerEnd() )
            {
                this->SetGameStatus(GameState::GAME_PLAYING);
            }
        break;

        case GameState::GAME_PAUSED:

        break;

        case GameState::GAME_PLAYING:
            // Starting countdown
            if ( !this->m_Countdown.IsTimerEnd() )
            {
                if (this->m_Countdown.GetElapsed() == 0)
                {
                    this->m_ThisFrameEvent.AddEvent(GameEvents::EVT_COUNTDOWN);
                }

                this->m_Countdown.UpdateTimer(p_Delta);

                if ( m_Countdown.HasPeriodPassed() )
                {
                    if (this->m_Countdown.GetElapsed() < 3000)
                    {
                        this->m_ThisFrameEvent.AddEvent(GameEvents::EVT_COUNTDOWN);
                    }
                }

            }
            else
            {
                bool bMadeGameFinished = false;

                // Once the game timer starts, play the "go" sound
                if (this->m_Player[0].GetGameTimer().GetElapsed() == 0)
                {
                    this->m_ThisFrameEvent.AddEvent(GameEvents::EVT_COUNTEND);
                }

                if (!m_IsFrameAdvance)
                {
                    for (unsigned int nPlayer = 0; nPlayer < this->m_Player.size(); nPlayer++ )
                    {
                        if (this->m_Player[nPlayer].GetStatus() == Player::PLAYER_STAT_PLAYING )
                        {
                            this->m_PanelGame.UpdateGameLogic(this->m_Player[nPlayer], p_Delta, this->m_ThisFrameEvent, m_MatchSettings);
                        }
                    }
                }

                // On the 10 last seconds of the game, play a bell sound every second
                // NOTE : at the moment, only the first player's timer creates a sound
                // (all the players play time are the same length, it might change depending on future game modes)
                if ( m_Player[0].GetGameTimer().HasPeriodPassed() )
                {
                    if ( m_Player[0].GetGameTimer().GetRemaining() < 9000 && !(m_Player[0].GetGameTimer().IsTimerEnd()) )
                    {
                        this->m_ThisFrameEvent.AddEvent(GameEvents::EVT_COUNTDOWN);
                    }
                }


                // Find out if game is over
                if (this->m_Player.size() == 1)
                {
                    // S'il n'y a qu'un joueur, il y a juste a v�rifier que le temps soit �coul� ou que le joueur ait perdu
                    if ( this->m_Player[0].GetStatus() != Player::PLAYER_STAT_PLAYING )
                    {
                        bMadeGameFinished = true;
                    }
                }
                else
                {
                    // In multiplayer there are different victory conditions
                    int nbSurvivants = 0;
                    // Who remains in the game
                    for (unsigned int nPlayer = 0; nPlayer < this->m_Player.size(); nPlayer++ )
                    {
                        if ( this->m_Player[nPlayer].GetStatus() == Player::PLAYER_STAT_PLAYING )
                        {
                            nbSurvivants++;
                        }
                    }

                    switch (nbSurvivants)
                    {
                    case 0:
                        {
                            // 2 possible cases:
                            // 1) Draw, because everyone lost at the same time
                            // 2) time is up, winner is calculated based on score

                            long tmpHiScore = 0;

                            for (unsigned int nPlayer = 0; nPlayer < this->m_Player.size(); nPlayer++ )
                            {
                                // Only account for scores of players who timed out
                                if ( this->m_Player[nPlayer].GetStatus() == Player::PLAYER_STAT_ENDTIMER )
                                {
                                    if (this->m_Player[nPlayer].GetScore() > tmpHiScore )
                                    {
                                        tmpHiScore = this->m_Player[nPlayer].GetScore();
                                        this->m_MatchWinner = nPlayer;
                                    }
                                    else if ( (this->m_Player[nPlayer].GetScore() == tmpHiScore) && (tmpHiScore > 0) )
                                    {
                                        // 2 identical scores. It's a draw unless beat by a later player
                                        this->m_MatchWinner = GameState::WINNER_DRAW;
                                    }

                                }
                            }

                            // no winner was detected, and no-one is left playing. It means everyone lost at the same time. it's a draw
                            if (this->m_MatchWinner == GameState::WINNER_NONE)
                            {
                                this->m_MatchWinner = GameState::WINNER_DRAW;
                            }


                        }

                        break;
                    case 1:
                        // Only 1 player left, find out who it is
                        for (unsigned int nPlayer = 0; nPlayer < this->m_Player.size(); nPlayer++ )
                        {
                            if ( this->m_Player[nPlayer].GetStatus() == Player::PLAYER_STAT_PLAYING )
                            {
                                this->m_MatchWinner = nPlayer;
                            }
                        }

                        break;
                    default:
                        this->m_MatchWinner = GameState::WINNER_NONE;
                        break;
                    }

                    switch (this->m_MatchWinner)
                    {
                    case GameState::WINNER_DRAW:
                        this->SetGameStatus(GameState::GAME_FINISHED);
                        break;
                    case GameState::WINNER_NONE:
                        break;
                    default:
                        // end remaining player's game
                        this->m_Player[this->m_MatchWinner].SetStatus(Player::PLAYER_STAT_VICTORY);
                        bMadeGameFinished = true;
                        break;

                    }

                }


                if (bMadeGameFinished)
                {
                    this->SetGameStatus(GameState::GAME_FINISHED);
                    this->m_TempStatusTimer.SetLength(C_GameStateEventTimers::FINISHED);
                    this->m_TempStatusTimer.Restart();
                }

            }

        break;

        case GameState::GAME_FINISHED:
            if (this->m_TempStatusTimer.IsTimerEnd())
            {
                this->SetGameStatus(GameState::GAME_OUTRO);
            }

        break;

        case GameState::GAME_OUTRO:
        break;

        case GameState::GAME_FADEOUT:
            if ( this->m_Fader.isFadingDone() )
            {
                return false;
            }
        break;
    }

    // Animation timers
    if ( m_BGScrollEffect.IsTimerEnd() )
    {
        m_BGScrollEffect.Restart( m_BGScrollEffect.GetOvertime() );
    }
    if ( m_CursorEffect.IsTimerEnd() )
    {
        m_CursorBlink = !m_CursorBlink;
        m_CursorEffect.Restart( m_CursorEffect.GetOvertime() );
    }
    if ( m_BlockEffect.IsTimerEnd() )
    {
        m_BlockBlink = !m_BlockBlink;
        m_BlockEffect.Restart( m_BlockEffect.GetOvertime() );
    }


    return true;
}

bool GameState::Frame()
{
    // TODO: refactor this
    m_SDL.window().EmptyFrame();

    int ScreenW = m_SDL.window().GetWidth();
    int ScreenH = m_SDL.window().GetHeight();

    m_SDL.window().DrawSquare(0, 0, ScreenW, ScreenH, 128, 128, 128, 255);

    this->RenderBackground(this->m_Player.size());

    // On s�pare l'�cran en deux s'il y a 2 joueurs, en 3 s'il y a 3 joueurs et ainsi de suite...
    for (unsigned int nPlayer = 0; nPlayer < this->m_Player.size() ; nPlayer++)
    {
        // R�f�rence au joueur qui va �tre utilis�
        Player& ThisPlayer = this->m_Player[nPlayer];

        int xStart = 0;
        int yStart = 0;

        // Coordonn�es de d�marrage du dessin de chaque pile de blocs
        switch ( m_Player.size() )
        {
            case 1:
                xStart = 224;
                yStart = 64;
                break;

            case 2:
                switch (nPlayer + 1)
                {
                    case 1:
                        xStart = 32;
                        break;
                    case 2:
                        xStart = 416;
                        break;
                }
                yStart = 64;
                break;
            default:
                // Divide the screen evenly. This is only a fallback method.
                xStart = (ScreenW / this->m_Player.size()) - (ScreenW / (this->m_Player.size() * 2) ) - ((C_TextureSizes::PANELW*C_Field::BLOCKSPERLINE) / 2) - 4 + ((ScreenW / this->m_Player.size()) * nPlayer);
                yStart = 64;
                break;

        }

        // Modification de certaines donn�es en fonction de l'�tat du jeu
        switch ( ThisPlayer.GetStatus() )
        {
        case Player::PLAYER_STAT_DEFEAT:
            if (this->GetGameStatus() == GameState::GAME_FINISHED)
            {
                yStart = yStart - (8 * sin(this->m_TempStatusTimer.GetElapsedPc()*(32* M_PI ))*(1-this->m_TempStatusTimer.GetElapsedPc()) );
            }
            break;
        case Player::PLAYER_STAT_ENDTIMER:
            // Quel effet effectuer ?
            break;
        default:
            break;
        }

        this->RenderGameField(xStart, yStart, ThisPlayer);

    }

    this->RenderGUI( m_Player.size() );

    // Afficher l'interface du menu pause / fin de partie
    this->RenderMenu(0);

    #ifdef DEBUGMODE
        // FPS
        this->m_TextRenderer.RenderTextOnScreen(m_SDL.window(), 0, 0, SPConv::NumToStr(m_SDL.window().FPSLimiter().GetLastFPS()) );
    #endif

    if ( this->GetGameStatus() == GameState::GAME_FADEIN || this->GetGameStatus() == GameState::GAME_FADEOUT )
        this->RenderFade(true);

    m_SDL.window().DisplayFrame();


    // Ev�nements sonores !
    // Musique - si en jeu, panique, si � la fin, on attend 3 secondes pour jouer la musique suivante
    if ( this->GetGameStatus() == GameState::GAME_FINISHED )
    {
        // Pendant 5 secondes, on reste muet
        if ( !this->m_TempStatusTimer.IsTimerEnd() )
        {
            if (m_SDL.audio().IsMusicPlaying() )
            {
                m_SDL.audio().StopCurrentMusic();
            }
        }
    }
    else if ( this->GetGameStatus() == GameState::GAME_OUTRO )
    {
        if ( !m_SDL.audio().IsMusicPlaying() )
        {
            // en mode un joueur, on a deux th�mes diff�rents
            if (m_Player.size() == 1)
            {
                switch (m_Player[0].GetStatus())
                {
                    case Player::PLAYER_STAT_DEFEAT:
                        m_SDL.audio().SetMusic( this->m_sndList.GetMusic(GameSnd::MUS_DEFEAT) );
                    break;

                    default:
                        m_SDL.audio().SetMusic( this->m_sndList.GetMusic(GameSnd::MUS_VICTORY) );
                    break;
                }
            }
            else
            {
                if ( this->m_MatchWinner == GameState::WINNER_DRAW )
                {
                    m_SDL.audio().SetMusic( this->m_sndList.GetMusic(GameSnd::MUS_DEFEAT) );
                }
                else
                {
                    m_SDL.audio().SetMusic( this->m_sndList.GetMusic(GameSnd::MUS_VICTORY) );
                }
            }

            m_SDL.audio().PlayCurrentMusic();
        }
    }
    else if ( this->GetGameStatus() == GameState::GAME_PLAYING )
    {
        // music : panic mode or not ?
        bool bIsBlockAtPanicLimit = false;

        // Music will go in panic mode if at least one player is in panic mode
        int i = 0;
        do {
            if (m_Player[i].isInPanicMode())
            {
                bIsBlockAtPanicLimit = true;
            }
            i++;
        } while (!bIsBlockAtPanicLimit && (i < m_Player.size()) );

        // If the music is currently in panic mode
        if (this->m_isPanicMusic)
        {
            // But the game is not in panic mode
            if (!bIsBlockAtPanicLimit)
            {
                // Put back the normal music
                m_SDL.audio().StopCurrentMusic();
                m_SDL.audio().SetMusic( this->m_sndList.GetMusic(GameSnd::MUS_MAIN) );
                m_isPanicMusic = false;
            }
        }
        else if (!m_isPanicMusic)
        {
            if (bIsBlockAtPanicLimit)
            {
                // put the music back in panic mode
                m_SDL.audio().StopCurrentMusic();
                m_SDL.audio().SetMusic( this->m_sndList.GetMusic(GameSnd::MUS_PANIC) );
                m_isPanicMusic = true;
            }
        }

        // If it's not playing, start the music
        if ( !m_SDL.audio().IsMusicPlaying() )
        {
            // Music starts 1 second after the beginning of a game
            if ( this->m_Countdown.IsTimerEnd() && this->m_Player[0].GetGameTimer().GetElapsed() > 1000 )
            {
                m_SDL.audio().PlayCurrentMusic();
            }
        }

    }



    // Effets sonores
    while ( !this->m_ThisFrameEvent.isQueueEmpty() )
    {
        switch ( this->m_ThisFrameEvent.GetCurrentEvent() )
        {
            case GameEvents::EVT_CURSORMOVE:
                m_SDL.audio().PlaySample(this->m_sndList.GetSample(GameSnd::SPL_CURSORMOVE), 2);
            break;

            case GameEvents::EVT_PANELMOVE:
                m_SDL.audio().PlaySample(this->m_sndList.GetSample(GameSnd::SPL_PANELMOVE), 3);
            break;

            case GameEvents::EVT_PANELBUMP:
                m_SDL.audio().PlaySample(this->m_sndList.GetSample(GameSnd::SPL_PANELBUMP), 4);
            break;

            case GameEvents::EVT_PANELPOP:
                m_SDL.audio().PlaySample(this->m_sndList.GetSample(GameSnd::SPL_PANELPOP), 5);
            break;

            case GameEvents::EVT_COMBOCHAIN:
                m_SDL.audio().PlaySample(this->m_sndList.GetSample(GameSnd::SPL_COMBOCHAIN), 6);
            break;

            case GameEvents::EVT_COUNTDOWN:
                m_SDL.audio().PlaySample(this->m_sndList.GetSample(GameSnd::SPL_COUNTDOWN));
            break;

            case GameEvents::EVT_COUNTEND:
                m_SDL.audio().PlaySample(this->m_sndList.GetSample(GameSnd::SPL_COUNTEND));
            break;

            case GameEvents::EVT_DEFEAT:
                m_SDL.audio().PlaySample(this->m_sndList.GetSample(GameSnd::SPL_GAMEOVERLOSE));

            default:
            break;
        }

        this->m_ThisFrameEvent.StepToNextEvent();
    }

    return true;
}

// Refactoring : fonctions de rendu s�par�es pour simplifier la lecture
void GameState::RenderBackground(int p_NumPlayers)
{
    // Arri�re plan d�filant
    int ScreenW = m_SDL.window().GetWidth();
    int ScreenH = m_SDL.window().GetHeight();

    long BGSizeH = this->m_texList.GetTextureByID(GameTex::TX_BACKGROUNDSCROLL).getH();
    long BGSizeW = this->m_texList.GetTextureByID(GameTex::TX_BACKGROUNDSCROLL).getW();

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
            this->m_texList.GetTextureByID(GameTex::TX_BACKGROUNDSCROLL).RenderToWindow(m_SDL.window(), DrawX, DrawY);
        }
    }

    switch (p_NumPlayers)
    {
        case 1:
            this->m_texList.GetTextureByID(GameTex::TX_FOREGROUND1P).RenderToWindow(m_SDL.window(), 0, 0);
        break;
        case 2:
            this->m_texList.GetTextureByID(GameTex::TX_FOREGROUND2P).RenderToWindow(m_SDL.window(), 0, 0);
        break;
        default:
            // Ne rien dessiner
        break;
    }

}

void GameState::RenderGameField(int p_XStart, int p_YStart, Player& ThisPlayer)
{

    // Contient le nombre de pixels de d�calage � appliquer sur les blocs situ�s imm�diatement au dessus d'un bloc qui tombe
    int iPixColBlockFall[C_Field::BLOCKSPERLINE] = {0};

    // Contient le nombre de pixels de d�calage � appliquer � la pile compl�te
    int yFieldRise = floor(C_TextureSizes::PANELH * ThisPlayer.GetField().GetNewLineTimer().GetElapsedPc() );

    // Dessiner le "carr� d'assombrissement" dans le cadre
    int m_RedValue = 0;
    if (ThisPlayer.isInPanicMode() && this->GetGameStatus() == GameState::GAME_PLAYING)
    {
        // When in panic mode, background will flash red
        m_RedValue = 64 + round(64.00 * cos( ((SDL_GetTicks() % 2000) / 2000.00) * (2 * M_PI) )) ;
    }
    m_SDL.window().DrawSquare(p_XStart, p_YStart, C_TextureSizes::PANELW*C_Field::BLOCKSPERLINE, C_TextureSizes::PANELH*C_Field::LINESPERFIELD, m_RedValue, 0, 0, 128);

    // Draw the game field
    int tmpTexID = -1;

    int BlocksDrawStartX = p_XStart;
    int BlocksDrawStartY = p_YStart + (C_TextureSizes::PANELH*11);

    // Variables tampon pour le rendu
    int DrawCoordX = 0;
    int DrawCoordY = 0;

    // Dessiner tous les blocs sauf ceux qui disparaissent
    for (int y = 0; y < C_TextureSizes::PANELH*C_Field::LINESPERFIELD; y++)
    {
        for (int x = 0; x < C_Field::BLOCKSPERLINE; x++)
        {
            if (ThisPlayer.GetField().isBlockAt(x, y))
            {
                switch ( ThisPlayer.GetField().GetBlock(x, y).GetColor() )
                {
                case Block::COLOR_1:
                    tmpTexID = GameTex::TX_PANEL1;
                    break;
                case Block::COLOR_2:
                    tmpTexID = GameTex::TX_PANEL2;
                    break;
                case Block::COLOR_3:
                    tmpTexID = GameTex::TX_PANEL3;
                    break;
                case Block::COLOR_4:
                    tmpTexID = GameTex::TX_PANEL4;
                    break;
                case Block::COLOR_5:
                    tmpTexID = GameTex::TX_PANEL5;
                    break;
                case Block::COLOR_6:
                    tmpTexID = GameTex::TX_PANEL6;
                    break;
                default:
                    // Do not draw anything
                    tmpTexID = -1;
                    break;
                }

                if (tmpTexID != -1)
                {

                    if (ThisPlayer.GetField().GetBlock(x, y).GetStatus() == Block::STATUS_SWITCHING )
                    {
                        float timerPc = ThisPlayer.GetField().GetBlock(x, y).GetTimer().GetElapsedPc();
                        bool bGoesToLeft = ( ThisPlayer.GetField().GetBlock(x, y).GetXDest() < ThisPlayer.GetField().GetBlock(x, y).GetX() );
                        int iDecalage = (float)C_TextureSizes::PANELW * timerPc;
                        DrawCoordY = BlocksDrawStartY - (C_TextureSizes::PANELH * y) - yFieldRise ;

                        if (bGoesToLeft)
                        {
                            // Vers la gauche, on d�cale � gauche
                            DrawCoordX = BlocksDrawStartX + ( C_TextureSizes::PANELW * x ) - iDecalage;
                        }
                        else
                        {
                            // vers la droite, on d�cale � droite
                            DrawCoordX = BlocksDrawStartX + ( C_TextureSizes::PANELW * x ) + iDecalage;
                        }

                        // Si je suis un bloc qui switche, je n'ai pas de raison de faire tomber des blocs au dessus de moi
                        iPixColBlockFall[x] = 0;
                    }
                    else if (ThisPlayer.GetField().GetBlock(x, y).GetStatus() == Block::STATUS_SUSPEND )
                    {
                        // Tremble avant de tomber
                        float timerPc = ThisPlayer.GetField().GetBlock(x, y).GetTimer().GetElapsedPc();
                        int iDecalage = 4 * sin(timerPc * (6 * M_PI));
                        iPixColBlockFall[x] = 0;
                        DrawCoordX = BlocksDrawStartX + ( C_TextureSizes::PANELW * x ) + iDecalage;
                        DrawCoordY = BlocksDrawStartY - ( C_TextureSizes::PANELH * y) + iPixColBlockFall[x] - yFieldRise;
                    }
                    else if (ThisPlayer.GetField().GetBlock(x, y).GetStatus() == Block::STATUS_FALLING )
                    {
                        // Falls at a fixed speed
                        float timerPc = ThisPlayer.GetField().GetBlock(x, y).GetTimer().GetElapsedPc();
                        iPixColBlockFall[x] = (float)C_TextureSizes::PANELH * timerPc;
                        DrawCoordX = BlocksDrawStartX + ( C_TextureSizes::PANELW * x );
                        DrawCoordY = BlocksDrawStartY - ( C_TextureSizes::PANELH * y) + iPixColBlockFall[x] - yFieldRise;
                    }
                    // Ne pas dessiner les blocs en cours de disparition
                    else if (ThisPlayer.GetField().GetBlock(x, y).GetStatus() == Block::STATUS_POPPING )
                    {
                        // Si un bloc pope, on le dessinera lors d'une deuxi�me phase
                        // Un bloc qui poppe ne peut pas �tre en chute, on coupe donc la chaine de blocs qui tombe au niveau graphique
                        iPixColBlockFall[x] = 0;
                    }
                    else
                    {
                        DrawCoordX = BlocksDrawStartX + ( C_TextureSizes::PANELW * x );
                        DrawCoordY = BlocksDrawStartY - ( C_TextureSizes::PANELH * y) - yFieldRise + iPixColBlockFall[x];
                    }

                    // Ne pas dessiner les blocs en cours de disparition, ce sera fait plus tard dans le rendu
                    if ( ThisPlayer.GetField().GetBlock(x, y).GetStatus() != Block::STATUS_POPPING )
                    {
                        this->m_texList.GetTextureByID(tmpTexID).RenderToWindow(m_SDL.window(), DrawCoordX, DrawCoordY);

                        // Assombrir les blocs quand le jeu est termin�
                        if (ThisPlayer.GetStatus() != Player::PLAYER_STAT_PLAYING)
                        {
                            unsigned long transparentValue = 128;
                            if (this->GetGameStatus() == GameState::GAME_FINISHED)
                            {
                                transparentValue *= this->m_TempStatusTimer.GetElapsedPc();
                            }
                            m_SDL.window().DrawSquare(DrawCoordX, DrawCoordY, C_TextureSizes::PANELW, C_TextureSizes::PANELH, 0, 0, 0, transparentValue );
                        }

                    }

                }

            } // end if isBlockAt(x, y)
            else
            {
                // Pas de bloc ? On r�initialise le d�calage d� � la chute
                iPixColBlockFall[x] = 0;
            }
        }
    }

    // Dessiner les blocs hors champ (qui apparaissent en bas de la pile)
    for (int x = 0; x < C_Field::BLOCKSPERLINE; x++)
    {

        switch ( ThisPlayer.GetField().GetNewLineBlock(x).GetColor() )
        {
        case Block::COLOR_1:
            tmpTexID = GameTex::TX_PANEL1;
            break;
        case Block::COLOR_2:
            tmpTexID = GameTex::TX_PANEL2;
            break;
        case Block::COLOR_3:
            tmpTexID = GameTex::TX_PANEL3;
            break;
        case Block::COLOR_4:
            tmpTexID = GameTex::TX_PANEL4;
            break;
        case Block::COLOR_5:
            tmpTexID = GameTex::TX_PANEL5;
            break;
        case Block::COLOR_6:
            tmpTexID = GameTex::TX_PANEL6;
            break;
        default:
            // Do not draw anything
            tmpTexID = -1;
            break;
        }

        if (tmpTexID != -1)
        {
            DrawCoordX = BlocksDrawStartX + ( C_TextureSizes::PANELW * x );
            DrawCoordY = BlocksDrawStartY + C_TextureSizes::PANELH - yFieldRise ;
            // Limiter la taille de dessin
            this->m_texList.GetTextureByID(tmpTexID).SetLocalRect(-1, 0, -1 , yFieldRise);
            this->m_texList.GetTextureByID(tmpTexID).RenderToWindow(m_SDL.window(), DrawCoordX, DrawCoordY);
            this->m_texList.GetTextureByID(tmpTexID).ResetLocalRect();

            // Les blocs de la prochaine ligne sont plus sombres quand ils ne sont pas encore disponibles
            switch ( ThisPlayer.GetStatus() )
            {
                case Player::PLAYER_STAT_DEFEAT:
                case Player::PLAYER_STAT_ENDTIMER:
                    {
                        // Quand la partie est termin�e, les blocs s'assombrissent au fil du temps
                        unsigned long transparentValue = 128;
                        if (this->GetGameStatus() == GameState::GAME_FINISHED)
                        {
                            transparentValue *= this->m_TempStatusTimer.GetElapsedPc();
                        }
                        m_SDL.window().DrawSquare(DrawCoordX, DrawCoordY, C_TextureSizes::PANELW, yFieldRise, 0, 0, 0, transparentValue );
                    }
                    break;
                default:
                    // En temps normal, les blocs s'�claircissent au fil de leur ascension
                    m_SDL.window().DrawSquare(DrawCoordX, DrawCoordY, C_TextureSizes::PANELW, yFieldRise, 0, 0, 0, 255 - (ThisPlayer.GetField().GetNewLineTimer().GetElapsedPc() * 192) );
                    break;
            }
        }

    }

    // Dessiner les blocs qui disparaissent
    for (unsigned int i = 0; i < ThisPlayer.GetDisappearingBlocks().size(); i++)
    {
        bool isFirstBlock = true;
        PopBlockSet& m_RefPop = ThisPlayer.GetDisappearingBlocks()[i];

        // faire �a de haut en bas et de gauche � droite
        for (int y = C_TextureSizes::PANELH*C_Field::LINESPERFIELD; y >= 0; y--)
        {
            for (int x = 0; x < C_Field::BLOCKSPERLINE; x++)
            {
                // Chercher les blocs dans l'ordre !
                bool bTrouve = false;
                unsigned int tmpIndex = 0;

                while ( (!bTrouve) && (tmpIndex < m_RefPop.GetListPopBlocksCoord().size()) )
                {
                    if ( m_RefPop.GetListPopBlocksCoord()[tmpIndex].GetX() == x && m_RefPop.GetListPopBlocksCoord()[tmpIndex].GetY() == y )
                    {
                        bTrouve = true;
                    }

                    tmpIndex++;
                }

                if (bTrouve)
                {

                    switch ( ThisPlayer.GetField().GetBlock(x, y).GetColor() )
                    {
                    case Block::COLOR_1:
                        tmpTexID = GameTex::TX_PANEL1;
                        break;
                    case Block::COLOR_2:
                        tmpTexID = GameTex::TX_PANEL2;
                        break;
                    case Block::COLOR_3:
                        tmpTexID = GameTex::TX_PANEL3;
                        break;
                    case Block::COLOR_4:
                        tmpTexID = GameTex::TX_PANEL4;
                        break;
                    case Block::COLOR_5:
                        tmpTexID = GameTex::TX_PANEL5;
                        break;
                    case Block::COLOR_6:
                        tmpTexID = GameTex::TX_PANEL6;
                        break;
                    default:
                        // Do not draw anything
                        tmpTexID = -1;
                        break;
                    }

                    if (tmpTexID != -1)
                    {
                        DrawCoordX = BlocksDrawStartX + ( C_TextureSizes::PANELW * x );
                        DrawCoordY = BlocksDrawStartY - ( C_TextureSizes::PANELH * y) - yFieldRise;
                        // Dessiner le bloc
                        this->m_texList.GetTextureByID(tmpTexID).RenderToWindow(m_SDL.window(), DrawCoordX, DrawCoordY);

                        // Faire blanchir les blocs � une seconde de leur disparition
                        if (m_RefPop.GetPopTimer().GetRemaining() < C_BlockTimers::TIMERPOP_GROUPBASE)
                        {
                            unsigned long tempsrestant = m_RefPop.GetPopTimer().GetRemaining();
                            unsigned long transparence = 255 - (255.f * ( (float)tempsrestant / (float)C_BlockTimers::TIMERPOP_GROUPBASE ));

                            m_SDL.window().DrawSquare(DrawCoordX, DrawCoordY, C_TextureSizes::PANELW, C_TextureSizes::PANELH, 255, 255, 255, transparence );
                        }

                        // Assombrir les blocs quand le jeu est termin�
                        if (ThisPlayer.GetStatus() != Player::PLAYER_STAT_PLAYING)
                        {
                            unsigned long transparentValue = 128;
                            if (this->GetGameStatus() == GameState::GAME_FINISHED)
                            {
                                transparentValue *= this->m_TempStatusTimer.GetElapsedPc();
                            }
                            m_SDL.window().DrawSquare(DrawCoordX, DrawCoordY, C_TextureSizes::PANELW, C_TextureSizes::PANELH, 0, 0, 0, transparentValue );
                        }
                        else if ( m_BlockBlink )
                        {
                            m_SDL.window().DrawSquare(DrawCoordX, DrawCoordY, C_TextureSizes::PANELW, C_TextureSizes::PANELH, 255, 255, 255, 128 );
                        }
                    }

                    // Dessiner l'image de combo et de cha�ne sur le premier bloc trouv�
                    if (isFirstBlock)
                    {
                        isFirstBlock = false; // UNIQUEMENT sur le premier de chaque combo
                        switch ( m_RefPop.GetListPopBlocksCoord().size())
                        {
                        case 3:
                            tmpTexID = -1;
                            break;
                        case 4:
                            tmpTexID = GameTex::TX_COMBO4;
                            break;
                        case 5:
                            tmpTexID = GameTex::TX_COMBO5;
                            break;
                        case 6:
                            tmpTexID = GameTex::TX_COMBO6;
                            break;
                        case 7:
                            tmpTexID = GameTex::TX_COMBO7;
                            break;
                        case 8:
                            tmpTexID = GameTex::TX_COMBO8;
                            break;
                        case 9:
                            tmpTexID = GameTex::TX_COMBO9;
                            break;
                        case 10:
                            tmpTexID = GameTex::TX_COMBO10;
                            break;
                        case 11:
                            tmpTexID = GameTex::TX_COMBO11;
                            break;
                        case 12:
                            tmpTexID = GameTex::TX_COMBO12;
                            break;
                        default:
                            tmpTexID = GameTex::TX_COMBOMORE;
                            break;
                        }

                        if (tmpTexID != -1)
                        {
                            DrawCoordX = BlocksDrawStartX + ( C_TextureSizes::PANELW * x );
                            DrawCoordY = BlocksDrawStartY - ( C_TextureSizes::PANELH * y) - yFieldRise;
                            // Dessiner le bloc combo
                            this->m_texList.GetTextureByID(tmpTexID).RenderToWindow(m_SDL.window(), DrawCoordX, DrawCoordY);
                        }

                        // Dessiner l'image de cha�ne sur le premier bloc trouv�
                        // Selon la valeur du multiplicateur de cha�ne
                        switch ( ThisPlayer.GetChainList()[ ThisPlayer.GetField().GetBlock(x, y).GetChainID() ] )
                        {
                        case 1:
                            tmpTexID = -1;
                            break;
                        case 2:
                            tmpTexID = GameTex::TX_CHAIN2;
                            break;
                        case 3:
                            tmpTexID = GameTex::TX_CHAIN3;
                            break;
                        case 4:
                            tmpTexID = GameTex::TX_CHAIN4;
                            break;
                        case 5:
                            tmpTexID = GameTex::TX_CHAIN5;
                            break;
                        case 6:
                            tmpTexID = GameTex::TX_CHAIN6;
                            break;
                        case 7:
                            tmpTexID = GameTex::TX_CHAIN7;
                            break;
                        case 8:
                            tmpTexID = GameTex::TX_CHAIN8;
                            break;
                        case 9:
                            tmpTexID = GameTex::TX_CHAIN9;
                            break;
                        case 10:
                            tmpTexID = GameTex::TX_CHAIN10;
                            break;
                        case 11:
                            tmpTexID = GameTex::TX_CHAIN11;
                            break;
                        case 12:
                            tmpTexID = GameTex::TX_CHAIN12;
                            break;
                        default:
                            tmpTexID = GameTex::TX_CHAINMORE;
                            break;
                        }

                        if (tmpTexID != -1)
                        {
                            DrawCoordX = BlocksDrawStartX + ( C_TextureSizes::PANELW * x ) + 16;
                            DrawCoordY = BlocksDrawStartY - ( C_TextureSizes::PANELH * y) - yFieldRise;
                            // Dessiner le bloc chaine
                            this->m_texList.GetTextureByID(tmpTexID).RenderToWindow(m_SDL.window(), DrawCoordX, DrawCoordY);
                        }
                    }

                }


            }
        }

    }

    // Dessiner le curseur si le jeu est en "intro" ou "en jeu"
    if ( this->GetGameStatus() == GameState::GAME_INTRO ||  this->GetGameStatus() == GameState::GAME_PLAYING )
    {
        if ( ThisPlayer.GetStatus() == Player::PLAYER_STAT_PLAYING )
        {
            if(m_CursorBlink)
            {
                DrawCoordX = BlocksDrawStartX + ( 32 *  ThisPlayer.GetCursor().GetX() ) - 6;
                DrawCoordY = BlocksDrawStartY - ( 32 *  ThisPlayer.GetCursor().GetY() ) - 6 - yFieldRise;
                this->m_texList.GetTextureByID(GameTex::TX_CURSORFIT).RenderToWindow(m_SDL.window(), DrawCoordX, DrawCoordY);
            }
            else
            {
                DrawCoordX = BlocksDrawStartX + ( 32 *  ThisPlayer.GetCursor().GetX() ) - 8;
                DrawCoordY = BlocksDrawStartY - ( 32 *  ThisPlayer.GetCursor().GetY() ) - 8 - yFieldRise;
                this->m_texList.GetTextureByID(GameTex::TX_CURSORLOOSE).RenderToWindow(m_SDL.window(), DrawCoordX, DrawCoordY);
            }
        }
    }

}

void GameState::RenderGUI(int p_NumPlayers)
{

    // Ces variables font office de tampon pour les coordonn�es de rendu
    int DrawX = 0;
    int DrawY = 0;

    // Draw HUD : Solo = Large
    if (this->m_Player.size() == 1)
    {
        // R�f�rence au joueur qui va �tre utilis�
        Player& ThisPlayer = this->m_Player[0];

        // Draw HUD : Time
        int tmpH = 0;
        int tmpM = 0;
        int tmpS = 0;

        // Draw HUD : Remaining time
        ThisPlayer.GetGameTimer().GetRemainingInHMS(tmpH, tmpM, tmpS);

        DrawX = 32;
        DrawY = 32;

        this->m_StrBufGameElements[GUI_TIME].RenderToWindow(m_SDL.window(), DrawX, DrawY);
        if (tmpS < 10)
        {
            this->m_StrBufGameValues[GUI_TIME].RasterizeString( SPConv::NumToStr(tmpM) + ":0" + SPConv::NumToStr(tmpS) , m_FontGUI);
        }
        else
        {
            this->m_StrBufGameValues[GUI_TIME].RasterizeString( SPConv::NumToStr(tmpM) + ":" + SPConv::NumToStr(tmpS) , m_FontGUI);
        }
        this->m_StrBufGameValues[GUI_TIME].RenderToWindow(m_SDL.window(), DrawX + 160, DrawY + 32, SPTexture::RENDER_FROMTOPRIGHT);

        // Draw HUD : Score
        DrawX = 448;
        DrawY = 32;
        this->m_StrBufGameElements[GUI_SCORE].RenderToWindow(m_SDL.window(), DrawX, DrawY);
        this->m_StrBufGameValues[GUI_SCORE].RasterizeString( SPConv::NumToStr( ThisPlayer.GetScore() ) , m_FontGUI);
        this->m_StrBufGameValues[GUI_SCORE].RenderToWindow(m_SDL.window(), DrawX + 160, DrawY + 32, SPTexture::RENDER_FROMTOPRIGHT);

        // Draw HUD : Raising speed
        DrawX = 448;
        DrawY = 176;
        this->m_StrBufGameElements[GUI_STACKSPEED].RenderToWindow(m_SDL.window(), DrawX, DrawY);
        // Afficher des infos diff�rentes selon si la mont�e est interrompue ou non
        if ( ThisPlayer.GetField().IsStopTimeInEffect() )
        {
            // Interrompu : indication du temps restant avec une formule du type "STOP ! 8"
            ThisPlayer.GetField().GetStopTimer().GetRemainingInHMS(tmpH, tmpM, tmpS);
            this->m_StrBufGameValues[GUI_STACKSPEED].SetColor(255, 128, 128);
            this->m_StrBufGameValues[GUI_STACKSPEED].RasterizeString( "STOP ! " + SPConv::NumToStr( tmpS ) , m_FontGUI);
            this->m_StrBufGameValues[GUI_STACKSPEED].RenderToWindow(m_SDL.window(), DrawX + 160, DrawY + 32, SPTexture::RENDER_FROMTOPRIGHT);
        }
        else
        {
            // En cours : afficher la vitesse
            this->m_StrBufGameValues[GUI_STACKSPEED].SetColor(255, 255, 255);
            this->m_StrBufGameValues[GUI_STACKSPEED].RasterizeString( SPConv::NumToStr( ThisPlayer.GetField().GetSpeed() ) , m_FontGUI);
            this->m_StrBufGameValues[GUI_STACKSPEED].RenderToWindow(m_SDL.window(), DrawX + 160, DrawY + 32, SPTexture::RENDER_FROMTOPRIGHT);
        }

        // Draw HUD : Difficulty mode
        DrawX = 448;
        DrawY = 256;
        this->m_StrBufGameElements[GUI_DIFFICULTY].RenderToWindow(m_SDL.window(), DrawX, DrawY);
        std::string sTmpDiff;
        switch ( ThisPlayer.GetDifficulty() )
        {
        case C_Difficulty::DIFF_EASY:
            sTmpDiff = "EASY";
            this->m_StrBufGameValues[GUI_DIFFICULTY].SetColor(0, 160, 192);
            break;
        case C_Difficulty::DIFF_MEDIUM:
            sTmpDiff = "MEDIUM";
            this->m_StrBufGameValues[GUI_DIFFICULTY].SetColor(192, 160, 0);
            break;
        case C_Difficulty::DIFF_HARD:
            sTmpDiff = "HARD";
            this->m_StrBufGameValues[GUI_DIFFICULTY].SetColor(192, 0, 160);
            break;
        default:
            sTmpDiff = "BUG !";
            break;
        }

        this->m_StrBufGameValues[GUI_DIFFICULTY].RasterizeString(sTmpDiff , m_FontGUI);
        this->m_StrBufGameValues[GUI_DIFFICULTY].RenderToWindow(m_SDL.window(), DrawX + 160, DrawY + 32, SPTexture::RENDER_FROMTOPRIGHT);

    }
    // Draw HUD : 2 players
    else if(this->m_Player.size() == 2)
    {
        Player& ThisPlayer1 = this->m_Player[0];
        Player& ThisPlayer2 = this->m_Player[1];

        // Draw HUD : Time
        int tmpH = 0;
        int tmpM = 0;
        int tmpS = 0;

        // Draw HUD : Remaining time
        ThisPlayer1.GetGameTimer().GetRemainingInHMS(tmpH, tmpM, tmpS);

        DrawX = 256;
        DrawY = 32;

        this->m_StrBufGameElements[GUI_TIME].RenderToWindow(m_SDL.window(), DrawX, DrawY);
        if (tmpS < 10)
        {
            this->m_StrBufGameValues[GUI_TIME].RasterizeString( SPConv::NumToStr(tmpM) + ":0" + SPConv::NumToStr(tmpS) , m_FontGUI);
        }
        else
        {
            this->m_StrBufGameValues[GUI_TIME].RasterizeString( SPConv::NumToStr(tmpM) + ":" + SPConv::NumToStr(tmpS) , m_FontGUI);
        }
        this->m_StrBufGameValues[GUI_TIME].RenderToWindow(m_SDL.window(), DrawX + 128, DrawY + 32, SPTexture::RENDER_FROMTOPRIGHT);

        // Draw HUD : Score du joueur 1
        DrawX = 256;
        DrawY = 112;
        this->m_StrBufGameElements[GUI_SCORE].RasterizeString("1P Score", m_FontGUI);
        this->m_StrBufGameElements[GUI_SCORE].RenderToWindow(m_SDL.window(), DrawX, DrawY);
        this->m_StrBufGameValues[GUI_SCORE].RasterizeString( SPConv::NumToStr( ThisPlayer1.GetScore() ) , m_FontGUI);
        this->m_StrBufGameValues[GUI_SCORE].RenderToWindow(m_SDL.window(), DrawX + 128, DrawY + 32, SPTexture::RENDER_FROMTOPRIGHT);

        // Draw HUD : Score du joueur 2
        DrawX = 256;
        DrawY = 112 + 64;
        this->m_StrBufGameElements[GUI_SCORE].RasterizeString("2P Score", m_FontGUI);
        this->m_StrBufGameElements[GUI_SCORE].RenderToWindow(m_SDL.window(), DrawX, DrawY);
        this->m_StrBufGameValues[GUI_SCORE].RasterizeString( SPConv::NumToStr( ThisPlayer2.GetScore() ) , m_FontGUI);
        this->m_StrBufGameValues[GUI_SCORE].RenderToWindow(m_SDL.window(), DrawX + 128, DrawY + 32, SPTexture::RENDER_FROMTOPRIGHT);

    }

    // Elements d'interface li�s � la pile du joueur
    for (unsigned int nPlayer = 0; nPlayer < this->m_Player.size() ; nPlayer++)
    {
        // R�f�rence au joueur qui va �tre utilis�
        Player& ThisPlayer = this->m_Player[nPlayer];
        long xStart = 0; // Coordonn�e du haut � droite de la pile du joueur
        long yStart = 0; // Coordonn�e en haut � droite de la pile du joueur


        // Coordonn�es de d�marrage du dessin de chaque pile de blocs
        switch ( m_Player.size() )
        {
            case 1:
                xStart = 224;
                yStart = 64;
                break;

            case 2:
                switch (nPlayer + 1)
                {
                    case 1:
                        xStart = 32;
                        break;
                    case 2:
                        xStart = 416;
                        break;
                }
                yStart = 64;
                break;
            default:
                break;

        }

        // Lifebar
        float timerPc = ThisPlayer.GetField().GetFieldResistanceTimer().GetElapsedPc();
        if (timerPc > 1.0000)
        {
            timerPc = 1.0000;
        }
        int wLifeBar = (C_Field::BLOCKSPERLINE*C_TextureSizes::PANELW) - (timerPc * (float)(C_Field::BLOCKSPERLINE*C_TextureSizes::PANELW) );
        DrawX = xStart;
        DrawY = yStart - 32;

        m_SDL.window().DrawSquare(DrawX, DrawY, wLifeBar, 16, 128, 255, 128, 255);
        m_SDL.window().DrawSquare(DrawX+wLifeBar, DrawY, 6.f*32.f - wLifeBar, 16, 255, 128, 128, 255);


        // D�compte de d�part
        if ( !this->m_Countdown.IsTimerEnd() )
        {
            // Ne pas afficher "4" pendant la transition !
            if (!this->m_Countdown.GetElapsed() == 0 )
            {
                DrawX = xStart + (3*32);
                DrawY = yStart + (6*32);

                this->m_StrBufGameElements[GUI_ENDMESSAGE].SetColor(192, 255, 255);
                this->m_StrBufGameElements[GUI_ENDMESSAGE].RasterizeString(SPConv::NumToStr( (this->m_Countdown.GetRemaining() / 1000) + 1) , m_FontGUI);
                this->m_StrBufGameElements[GUI_ENDMESSAGE].RenderToWindow(m_SDL.window(), DrawX, DrawY, SPTexture::RENDER_FROMCENTER );
            }
        }

        // Messages de fin


        if ( ThisPlayer.GetStatus() != Player::PLAYER_STAT_PLAYING )
        {

            DrawX = xStart + (3*32);
            DrawY = yStart + (3*32);

            if ( ThisPlayer.GetStatus() == Player::PLAYER_STAT_DEFEAT )
            {
                if (this->m_MatchWinner == GameState::WINNER_DRAW )
                {
                    this->m_StrBufGameElements[GUI_ENDTITLE].SetColor(192, 192, 192);
                    this->m_StrBufGameElements[GUI_ENDTITLE].RasterizeString( "DRAW" , m_FontGUI);
                    this->m_StrBufGameElements[GUI_ENDTITLE].RenderToWindow(m_SDL.window(), DrawX, DrawY, SPTexture::RENDER_FROMCENTER );
                }
                else
                {
                    this->m_StrBufGameElements[GUI_ENDTITLE].SetColor(192, 0, 255);
                    this->m_StrBufGameElements[GUI_ENDTITLE].RasterizeString( "GAME OVER" , m_FontGUI);
                    this->m_StrBufGameElements[GUI_ENDTITLE].RenderToWindow(m_SDL.window(), DrawX, DrawY, SPTexture::RENDER_FROMCENTER );
                }

            }
            else if ( ThisPlayer.GetStatus() == Player::PLAYER_STAT_ENDTIMER )
            {
                // On affiche des choses diff�rentes si on est en solo ou en multi
                if (this->m_Player.size() == 1)
                {
                    this->m_StrBufGameElements[GUI_ENDTITLE].SetColor(0, 255, 128);
                    this->m_StrBufGameElements[GUI_ENDTITLE].RasterizeString( "TIME UP" , m_FontGUI);
                    this->m_StrBufGameElements[GUI_ENDTITLE].RenderToWindow(m_SDL.window(), DrawX, DrawY, SPTexture::RENDER_FROMCENTER );
                }
                else
                {
                    // Afficher le vainqueur et les perdants
                    if (this->m_MatchWinner == nPlayer)
                    {
                        this->m_StrBufGameElements[GUI_ENDTITLE].SetColor(0, 255, 128);
                        this->m_StrBufGameElements[GUI_ENDTITLE].RasterizeString( "YOU WIN !" , m_FontGUI);
                        this->m_StrBufGameElements[GUI_ENDTITLE].RenderToWindow(m_SDL.window(), DrawX, DrawY, SPTexture::RENDER_FROMCENTER );
                    }
                    else if (this->m_MatchWinner == GameState::WINNER_DRAW)
                    {
                        this->m_StrBufGameElements[GUI_ENDTITLE].SetColor(192, 192, 192);
                        this->m_StrBufGameElements[GUI_ENDTITLE].RasterizeString( "DRAW" , m_FontGUI);
                        this->m_StrBufGameElements[GUI_ENDTITLE].RenderToWindow(m_SDL.window(), DrawX, DrawY, SPTexture::RENDER_FROMCENTER );
                    }
                    else
                    {
                        this->m_StrBufGameElements[GUI_ENDTITLE].SetColor(192, 0, 255);
                        this->m_StrBufGameElements[GUI_ENDTITLE].RasterizeString( "YOU LOSE..." , m_FontGUI);
                        this->m_StrBufGameElements[GUI_ENDTITLE].RenderToWindow(m_SDL.window(), DrawX, DrawY, SPTexture::RENDER_FROMCENTER );
                    }
                }

            }
            else if ( ThisPlayer.GetStatus() == Player::PLAYER_STAT_VICTORY )
            {
                // Statut uniquement accessible en multijoueur
                this->m_StrBufGameElements[GUI_ENDTITLE].SetColor(0, 255, 128);
                this->m_StrBufGameElements[GUI_ENDTITLE].RasterizeString( "YOU WIN !" , m_FontGUI);
                this->m_StrBufGameElements[GUI_ENDTITLE].RenderToWindow(m_SDL.window(), DrawX, DrawY, SPTexture::RENDER_FROMCENTER );
            }

            // Score final
            DrawX = xStart + (3*32);
            DrawY = yStart + (4*32);
            this->m_StrBufGameElements[GUI_ENDMESSAGE].SetColor(255, 255, 255);
            this->m_StrBufGameElements[GUI_ENDMESSAGE].RasterizeString( "Final Score :" , m_FontGUI);
            this->m_StrBufGameElements[GUI_ENDMESSAGE].RenderToWindow(m_SDL.window(), DrawX, DrawY, SPTexture::RENDER_FROMCENTER );
            DrawY = yStart + (5*32);
            this->m_StrBufGameElements[GUI_ENDMESSAGE].SetColor(255, 255, 255);
            this->m_StrBufGameElements[GUI_ENDMESSAGE].RasterizeString( SPConv::NumToStr( ThisPlayer.GetScore() ) , m_FontGUI);
            this->m_StrBufGameElements[GUI_ENDMESSAGE].RenderToWindow(m_SDL.window(), DrawX, DrawY, SPTexture::RENDER_FROMCENTER );

        }

    }

}

void GameState::RenderMenu(int p_MenuType)
{

    if ( this->GetGameStatus() == GameState::GAME_OUTRO || this->GetGameStatus() == GameState::GAME_PAUSED )
    {
        // Darken screen and display "Pause"
        if ( this->GetGameStatus() == GameState::GAME_PAUSED )
        {
            m_SDL.window().DrawSquare(0, 0, C_WindowSize::WIDTH, C_WindowSize::HEIGHT, 0, 0, 0, 128);
            // Pause message
            this->m_StrBufMenuElements[GUI_PAUSEMESSAGE].RenderToWindow(m_SDL.window(), 320, 120, SPTexture::RENDER_FROMCENTER);
            // Help text
            this->m_HelpText.RenderToWindow(m_SDL.window(), 10, 440);
        }

        this->m_PauseMenu.RenderToWindow(m_SDL.window(), 208, 320, m_FontMenu);
    }

}

void GameState::RenderFade(bool p_isFadeIn)
{
    this->m_Fader.RenderToWindow(m_SDL.window());
}

bool GameState::LoadResources()
{
    GameTex::LoadGameTex( this->m_texList );

    // Sounds
    GameSnd::LoadGameSnd( this->m_sndList );

    // Text
    this->m_TextRenderer.LoadFont("./fonts/armandc-ascii-font-8.png", 8, 8);
    this->m_FontMenu.LoadFont("./fonts/BubblegumSans-Regular.otf", C_FontProperties::SMALLFONTHEIGHT);
    this->m_FontGUI.LoadFont("./fonts/BubblegumSans-Regular.otf", C_FontProperties::MEDIUMFONTHEIGHT);

    // Game GUI
    // Game labels
    this->m_StrBufGameElements[GUI_TIME].SetColor(C_FontProperties::TITLEFONTR, C_FontProperties::TITLEFONTG, C_FontProperties::TITLEFONTB);
    this->m_StrBufGameElements[GUI_TIME].RasterizeString("Time", this->m_FontGUI);
    this->m_StrBufGameElements[GUI_SCORE].SetColor(C_FontProperties::TITLEFONTR, C_FontProperties::TITLEFONTG, C_FontProperties::TITLEFONTB);
    this->m_StrBufGameElements[GUI_SCORE].RasterizeString("Score", this->m_FontGUI);
    this->m_StrBufGameElements[GUI_DIFFICULTY].SetColor(C_FontProperties::TITLEFONTR, C_FontProperties::TITLEFONTG, C_FontProperties::TITLEFONTB);
    this->m_StrBufGameElements[GUI_DIFFICULTY].RasterizeString("Difficulty", this->m_FontGUI);
    this->m_StrBufGameElements[GUI_STACKSPEED].SetColor(C_FontProperties::TITLEFONTR, C_FontProperties::TITLEFONTG, C_FontProperties::TITLEFONTB);
    this->m_StrBufGameElements[GUI_STACKSPEED].RasterizeString("Stack Speed", this->m_FontGUI);
    this->m_StrBufGameElements[GUI_ENDTITLE].SetColor(C_FontProperties::TITLEFONTR, C_FontProperties::TITLEFONTG, C_FontProperties::TITLEFONTB);
    this->m_StrBufGameElements[GUI_ENDTITLE].RasterizeString("Game Over !", this->m_FontGUI);
    this->m_StrBufGameElements[GUI_ENDMESSAGE].SetColor(C_FontProperties::TITLEFONTR, C_FontProperties::TITLEFONTG, C_FontProperties::TITLEFONTB);
    this->m_StrBufGameElements[GUI_ENDMESSAGE].RasterizeString("*End message*", this->m_FontGUI);
    // Game values
    this->m_StrBufGameValues[GUI_TIME].SetColor(C_FontProperties::VALUEFONTR, C_FontProperties::VALUEFONTG, C_FontProperties::VALUEFONTB);
    this->m_StrBufGameValues[GUI_SCORE].SetColor(C_FontProperties::VALUEFONTR, C_FontProperties::VALUEFONTG, C_FontProperties::VALUEFONTB);
    this->m_StrBufGameValues[GUI_DIFFICULTY].SetColor(C_FontProperties::VALUEFONTR, C_FontProperties::VALUEFONTG, C_FontProperties::VALUEFONTB);
    this->m_StrBufGameValues[GUI_STACKSPEED].SetColor(C_FontProperties::VALUEFONTR, C_FontProperties::VALUEFONTG, C_FontProperties::VALUEFONTB);

    // Pause GUI
    // Pause message
    this->m_StrBufMenuElements[GUI_PAUSEMESSAGE].SetColor(C_FontProperties::TITLEFONTR, C_FontProperties::TITLEFONTG, C_FontProperties::TITLEFONTB);
    this->m_StrBufMenuElements[GUI_PAUSEMESSAGE].RasterizeString("PAUSE", this->m_FontGUI);
    // Pause menu
    this->m_PauseMenu.SetMenuWidth(224);
    this->m_PauseMenu.AddMenuEntry("Continue", MenuControl::TYPE_VALIDATOR);
    this->m_PauseMenu.AddMenuEntry("Exit to menu", MenuControl::TYPE_VALIDATOR);
    // Pause help text
    this->m_HelpText.SetScrollerSpeed(80);
    this->m_HelpText.SetScrollerWidth(620);
    std::string tmpStringHelp;

    if (this->m_Player.size() >= 1)
    {
        tmpStringHelp += "Player 1 controls - "
                        + std::string(SDL_GetKeyName(m_Controls.p1_Up)) + "/"
                        + SDL_GetKeyName(m_Controls.p1_Down) + "/"
                        + SDL_GetKeyName(m_Controls.p1_Left) + "/"
                        + SDL_GetKeyName(m_Controls.p1_Right) + " : Move cursor - "
                        + SDL_GetKeyName(m_Controls.p1_Swap) + " : Switch blocks - "
                        + SDL_GetKeyName(m_Controls.p1_Raise) + " : Rise stack - "
                        + SDL_GetKeyName(m_Controls.p1_Pause) + " : Pause";
    }
    if (this->m_Player.size() >= 2)
    {
        tmpStringHelp += " --- ";
        tmpStringHelp += "Player 2 controls - "
                        + std::string(SDL_GetKeyName(m_Controls.p2_Up)) + "/"
                        + SDL_GetKeyName(m_Controls.p2_Down) + "/"
                        + SDL_GetKeyName(m_Controls.p2_Left) + "/"
                        + SDL_GetKeyName(m_Controls.p2_Right) + " : Move cursor - "
                        + SDL_GetKeyName(m_Controls.p2_Swap) + " : Switch blocks - "
                        + SDL_GetKeyName(m_Controls.p2_Raise) + " : Rise stack - "
                        + SDL_GetKeyName(m_Controls.p2_Pause) + " : Pause";
    }

    this->m_HelpText.SetString(tmpStringHelp, this->m_FontMenu, C_FontProperties::MENUELTFONTR, C_FontProperties::MENUELTFONTG, C_FontProperties::MENUELTFONTB);

    return true;
}

bool GameState::UnloadResources()
{
    this->m_texList.UnloadEverything();

    this->m_sndList.UnloadAllSamples();
    this->m_sndList.UnloadAllMusic();
    return true;
}

