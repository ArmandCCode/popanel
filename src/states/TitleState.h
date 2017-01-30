#ifndef TitleState_H
#define TitleState_H

#include "SPEngine/spsdl/SPGameState.h"
#include "StateRequestObject.h"
#include "SPEngine/spsdl/video/SPTextureMger.h"
#include "SPEngine/spsdl/audio/SPSoundsMger.h"
#include "SPEngine/spsdl/timer/SPTimer.h"
#include "gui/consts/PanelTexLoader.h"
#include "gui/consts/PanelSndLoader.h"
#include "SPEngine/spsdl/video/SPAsciiBMP.h"
#include "game/containers/MatchSettings.h"
#include "game/actors/Player.h"
#include "SPEngine/spsdl/video/SPTTFFont.h"
#include "gui/MenuControl.h"
#include "gui/FaderColor.h"
#include "gui/TextScroller.h"
#include "gui/MessageBox.h"
#include "gui/KeyboardControls.h"

class TitleState : public SPGameState
{
    public:
        TitleState(SPSDL& p_SDLRef, StateRequestObject& p_StateRequest, KeyboardControls& p_ControlsSettings, MatchSettings& p_MatchSettings);
        virtual ~TitleState();
        bool Play(long p_Delta);
        bool Frame();
    protected:
    private:
        // State request info
        StateRequestObject& m_stateRequest;

        // Game variables
        KeyboardControls& m_Controls;

        // Texture list
        SPTextureMger m_texList;
        SPAsciiBMP m_TextRenderer;
        SPTTFFont m_FontMenu;
        SPTTFFont m_FontMessage;

        bool LoadResources();
        bool UnloadResources();

        // Sounds list
        SPSoundsMger m_sndList;

        // Match Settings
        MatchSettings& m_MatchSettings;

        // Menu
        MenuControl m_MenuCtl;
        static const int MENU_NUMPLAYERS = 0;
        static const int MENU_DIFFICULTY = 1;
        static const int MENU_STACKSPEED = 2;
        static const int MENU_KEYCONFIG = 3;
        static const int MENU_STARTGAME = 5;
        static const int MENU_QUITGAME = 6;

        // Arrière plan défilant
        SPTimer m_BGScrollEffect;

        // Fondu
        FaderColor m_Fader;

        // Statuts de l'écran titre
        int m_MenuStatus;
        int GetMenuStatus() { return m_MenuStatus; }
        void SetMenuStatus(int val) { m_MenuStatus = val; }
        static const int MENU_FADEIN = 0;
        static const int MENU_PRESSANYKEY = 1;
        static const int MENU_INMENU = 2;
        static const int MENU_FADEOUT = 3;

        // Textes défilants
        MessageBox m_PressAnyKey;
        TextScroller m_HelpScroll;
};

#endif // TitleState_H
