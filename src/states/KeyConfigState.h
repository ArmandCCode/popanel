#ifndef KEYCONFIGSTATE_H
#define KEYCONFIGSTATE_H

#include "SPEngine/spsdl/SPGameState.h"
#include "StateRequestObject.h"
#include "SPEngine/spsdl/video/SPTextureMger.h"
#include "SPEngine/spsdl/audio/SPSoundsMger.h"
#include "SPEngine/spsdl/timer/SPTimer.h"
#include "gui/consts/PanelTexLoader.h"
#include "gui/consts/PanelSndLoader.h"
#include "SPEngine/spsdl/video/SPAsciiBMP.h"
#include "SPEngine/spsdl/video/SPTTFFont.h"
#include "gui/MenuControl.h"
#include "gui/FaderColor.h"
#include "gui/MessageBox.h"
#include "gui/KeyboardControls.h"

class KeyConfigState : public SPGameState
{
    public:
        KeyConfigState(SPSDL& p_SDLRef, StateRequestObject& p_StateRequest, KeyboardControls& p_ControlsSettings);
        virtual ~KeyConfigState();
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

        // Menu
        MenuControl m_MenuCtl;
        static const int MENU_P1KEYUP = 0;
        static const int MENU_P1KEYDOWN = 1;
        static const int MENU_P1KEYLEFT = 2;
        static const int MENU_P1KEYRIGHT = 3;
        static const int MENU_P1KEYSWAP = 4;
        static const int MENU_P1KEYRAISE = 5;
        static const int MENU_P1KEYPAUSE = 6;
        static const int MENU_P1P2SEPARATOR = 7;
        static const int MENU_P2KEYUP = 8;
        static const int MENU_P2KEYDOWN = 9;
        static const int MENU_P2KEYLEFT = 10;
        static const int MENU_P2KEYRIGHT = 11;
        static const int MENU_P2KEYSWAP = 12;
        static const int MENU_P2KEYRAISE = 13;
        static const int MENU_P2KEYPAUSE = 14;
        static const int MENU_P2SAVESEPARATOR = 15;
        static const int MENU_SAVEANDQUIT = 16;
        static const int MENU_QUITNOSAVE = 17;
        static const int MENU_RESETDEFAULTS = 18;

        // "Press a new key" message box
        MessageBox m_MessageBox;
        MessageBox m_KeyPressCountdownBox;
        SPTimer m_KeyPressCountdown;


        // Arrière plan défilant
        SPTimer m_BGScrollEffect;

        // Fondu
        FaderColor m_Fader;

        // Statuts
        int m_MenuStatus;
        int GetMenuStatus() { return m_MenuStatus; }
        void SetMenuStatus(int val) { m_MenuStatus = val; }
        static const int MENU_FADEIN = 0;
        static const int MENU_INMENU = 2;
        static const int MENU_FADEOUT = 3;
        static const int MENU_WAITFORINPUT = 4;
};

#endif // KEYCONFIGSTATE_H
