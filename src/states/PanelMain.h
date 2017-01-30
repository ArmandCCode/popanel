#ifndef PANELMAIN_H
#define PANELMAIN_H

#include <string>
#include <iostream>

#ifdef GP2X
    #define DISP_BPP 24
#else
    #define DISP_BPP 32
#endif

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "SPEngine/spsdl/SPSDL.h"
#include "SPEngine/spsdl/SPBaseState.h"
#include "LogoState.h"
#include "TitleState.h"
#include "GameState.h"
#include "KeyConfigState.h"
#include "StateRequestObject.h"
#include "game/containers/MatchSettings.h"
#include "gui/KeyboardControls.h"

class PanelMain : public SPBaseState
{
    public:
        PanelMain();
        virtual ~PanelMain();

        bool Play(long p_Delta = 0); // Events and game logic
        bool Frame(); // Drawing graphics

    protected:
        // Internal functions : loading window and resources
        bool LoadResources();
        bool UnloadResources();

        bool LoadControls();

    private:
        SPSDL m_SDL;

        // State request info
        StateRequestObject m_StateRequest;

        // Match Settings
        MatchSettings m_MatchSettings;

        // Keyboard controls
        KeyboardControls m_Controls;
};

#endif // PANELMAIN_H
