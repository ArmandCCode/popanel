#ifndef GameState_H
#define GameState_H

#include "SPEngine/spsdl/SPGameState.h"
#include "SPEngine/spsdl/video/SPTextureMger.h"
#include "SPEngine/spsdl/audio/SPSoundsMger.h"
#include "SPEngine/spsdl/video/SPAsciiBMP.h"
#include "SPEngine/sputil/SPConvertTools.h"

#include "StateRequestObject.h"
#include "game/rules/PanelGameBehaviour.h"
#include "gui/consts/PanelTexLoader.h"
#include "gui/consts/PanelSndLoader.h"
#include "gui/consts/PanelGUIConsts.h"
#include "game/actors/Player.h"
#include "gui/GameEvents.h"
#include "gui/MenuControl.h"
#include "SPEngine/spsdl/video/SPTTFFont.h"
#include "SPEngine/spsdl/video/SPStringBuffer.h"
#include "SPEngine/spsdl/timer/SPTimer.h"
#include "gui/FaderColor.h"
#include "gui/TextScroller.h"
#include "gui/KeyboardControls.h"

#include <SDL/SDL.h>

#include "game/containers/MatchSettings.h"

#include <cmath>

class GameState : public SPGameState
{
    public:
        GameState(SPSDL& p_SDLRef, StateRequestObject& p_StateRequest, KeyboardControls& p_ControlsSettings, MatchSettings& p_MatchSettings);
        virtual ~GameState();
        bool Play(long p_Delta);
        bool Frame();
    protected:
    private:
        // State request info
        StateRequestObject& m_stateRequest;

        // Game variables
        KeyboardControls& m_Controls;
        PanelGameBehaviour m_PanelGame;
        MatchSettings& m_MatchSettings;
        std::vector<Player> m_Player;

        // Textures list
        SPTextureMger m_texList;
        SPAsciiBMP m_TextRenderer;

        SPTTFFont m_FontMenu; // Taille "normale"
        SPTTFFont m_FontGUI; // Gros caractères

        // Elements d'interface
        // Game interface
        std::vector<SPStringBuffer> m_StrBufGameElements;
        std::vector<SPStringBuffer> m_StrBufGameValues;
        static const int GUI_TIME = 0;
        static const int GUI_SCORE = 1;
        static const int GUI_DIFFICULTY = 2;
        static const int GUI_STACKSPEED = 3;
        static const int GUI_ENDTITLE = 4; // "Game over"
        static const int GUI_ENDMESSAGE = 5; // "You win/You lose/Draw"

        // Pause interface
        std::vector<SPStringBuffer> m_StrBufMenuElements;
        static const int GUI_PAUSEMESSAGE = 0;
        // Menu GUI control
        MenuControl m_PauseMenu;
        static const int MENU_CONTINUE = 0;
        static const int MENU_QUIT = 1;
        TextScroller m_HelpText;

        bool LoadResources();
        bool UnloadResources();

        // Sounds list
        SPSoundsMger m_sndList;
        // IsPanic, for music
        bool m_isPanicMusic;

        // Timer pour la répétition des déplacements
        std::vector<SPTimer> m_CurMoveTimer;
        std::vector<int> m_CurDir;

        // Décompte avant début de partie
        ManualTimer m_Countdown;

        // Animation timers
        // Background scrolling effect
        SPTimer m_BGScrollEffect;
        // Cursor stretching effect
        bool m_CursorBlink;
        SPTimer m_CursorEffect;
        // Flashing block effect
        bool m_BlockBlink;
        SPTimer m_BlockEffect;
        void PauseAnimationTimers();
        void ResumeAnimationTimers();

        // Temporary status timer
        SPTimer m_TempStatusTimer;

        // Debug : activer une mise à jour frame de 10Ms par 10Ms
        bool m_IsFrameAdvance;

        // Game status management
        int m_GameStatus;
        int m_PreviousGameStatus;
        int GetGameStatus() { return m_GameStatus; }
        void SetGameStatus(int val);
        int GetPreviousGameStatus() { return m_PreviousGameStatus; }
        void GoToPreviousGameStatus();
        static const int GAME_FADEIN = 0;
        static const int GAME_INTRO = 1;
        static const int GAME_PLAYING = 2;
        static const int GAME_PAUSED = 3;
        static const int GAME_FINISHED = 4;
        static const int GAME_OUTRO = 5;
        static const int GAME_FADEOUT = 6;

        // Winner detection
        int m_MatchWinner;
        static const int WINNER_NONE = -1; // Pas encore terminé
        static const int WINNER_DRAW = -2;

        // Evènements sonores
        GameEvents m_ThisFrameEvent;

        // Fondu
        FaderColor m_Fader;

        // Refactoring : fonctions de rendu séparées pour simplifier la lecture
        void RenderBackground(int p_NumPlayers);
        void RenderGameField(int p_XStart, int p_YStart, Player& ThisPlayer);
        void RenderGUI(int p_NumPlayers);
        void RenderMenu(int p_MenuType);
        static const int MENU_PAUSED = 0;
        static const int MENU_END = 1;
        void RenderFade(bool p_isFadeIn);

};

#endif // GameState_H
