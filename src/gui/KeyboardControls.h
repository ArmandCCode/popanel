#ifndef KEYBOARDCONTROLS_H
#define KEYBOARDCONTROLS_H

#include <SDL/SDL.h>
#include <fstream>
#include "SPEngine/SPException.h"
#include "SPEngine/spsdl/input/SPInputMger.h"
#include "gui/consts/PanelActionsConst.h"

struct KeyboardControls
{
        // Menu controls
        SDLKey m_Validate;
        SDLKey m_Cancel;
        SDLKey m_Up;
        SDLKey m_Down;
        SDLKey m_Left;
        SDLKey m_Right;

        // Player 1 controls
        SDLKey p1_Swap;
        SDLKey p1_Raise;
        SDLKey p1_Up;
        SDLKey p1_Down;
        SDLKey p1_Left;
        SDLKey p1_Right;
        SDLKey p1_Pause;

        // Player 2 controls
        SDLKey p2_Swap;
        SDLKey p2_Raise;
        SDLKey p2_Up;
        SDLKey p2_Down;
        SDLKey p2_Left;
        SDLKey p2_Right;
        SDLKey p2_Pause;
};

void SetDictionaryFromControls(SPInputMger& p_InputMgr, KeyboardControls& p_Ctrls);
void SetDictionnaryAsXboxGamepad(SPInputMger& p_InputMgr, long p_PlayerNumber, long p_GamepadIndex);
void SetDefaultControls(KeyboardControls& p_Ctrls);

bool ControlsIniExist();
bool ControlsLoadFromIni(KeyboardControls& p_Ctrls);
bool ControlsSaveToIni(KeyboardControls& p_Ctrls);

#endif // KEYBOARDCONTROLS_H
