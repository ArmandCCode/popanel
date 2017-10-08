#include "KeyboardControls.h"

void SetDictionaryFromControls(SPInputMger& p_InputMgr, KeyboardControls& p_Ctrls)
{
    p_InputMgr.ResetActionDictionnary();

    // General menu controls
    p_InputMgr.AddSDLKeyboardInputToActionDictionnary(C_Actions::MENUUP, 0, p_Ctrls.m_Up);
    p_InputMgr.AddSDLKeyboardInputToActionDictionnary(C_Actions::MENUDOWN, 0, p_Ctrls.m_Down);
    p_InputMgr.AddSDLKeyboardInputToActionDictionnary(C_Actions::MENULEFT, 0, p_Ctrls.m_Left);
    p_InputMgr.AddSDLKeyboardInputToActionDictionnary(C_Actions::MENURIGHT, 0, p_Ctrls.m_Right);
    p_InputMgr.AddSDLKeyboardInputToActionDictionnary(C_Actions::MENUVALIDATE, 0, p_Ctrls.m_Validate);
    p_InputMgr.AddSDLKeyboardInputToActionDictionnary(C_Actions::MENUCANCEL, 0, p_Ctrls.m_Cancel);

    // Player 1 in-game controls
    p_InputMgr.AddSDLKeyboardInputToActionDictionnary(C_Actions::UP, 1, p_Ctrls.p1_Up);
    p_InputMgr.AddSDLKeyboardInputToActionDictionnary(C_Actions::DOWN, 1, p_Ctrls.p1_Down);
    p_InputMgr.AddSDLKeyboardInputToActionDictionnary(C_Actions::LEFT, 1, p_Ctrls.p1_Left);
    p_InputMgr.AddSDLKeyboardInputToActionDictionnary(C_Actions::RIGHT, 1, p_Ctrls.p1_Right);
    p_InputMgr.AddSDLKeyboardInputToActionDictionnary(C_Actions::SWAP, 1, p_Ctrls.p1_Swap);
    p_InputMgr.AddSDLKeyboardInputToActionDictionnary(C_Actions::RAISE, 1, p_Ctrls.p1_Raise);
    p_InputMgr.AddSDLKeyboardInputToActionDictionnary(C_Actions::PAUSE, 1, p_Ctrls.p1_Pause);

    // Player 2 in-game controls
    p_InputMgr.AddSDLKeyboardInputToActionDictionnary(C_Actions::UP, 2, p_Ctrls.p2_Up);
    p_InputMgr.AddSDLKeyboardInputToActionDictionnary(C_Actions::DOWN, 2, p_Ctrls.p2_Down);
    p_InputMgr.AddSDLKeyboardInputToActionDictionnary(C_Actions::LEFT, 2, p_Ctrls.p2_Left);
    p_InputMgr.AddSDLKeyboardInputToActionDictionnary(C_Actions::RIGHT, 2, p_Ctrls.p2_Right);
    p_InputMgr.AddSDLKeyboardInputToActionDictionnary(C_Actions::SWAP, 2, p_Ctrls.p2_Swap);
    p_InputMgr.AddSDLKeyboardInputToActionDictionnary(C_Actions::RAISE, 2, p_Ctrls.p2_Raise);
    p_InputMgr.AddSDLKeyboardInputToActionDictionnary(C_Actions::PAUSE, 2, p_Ctrls.p2_Pause);
}

void SetDictionnaryAsXboxGamepad(SPInputMger& p_InputMgr, long p_PlayerNumber, long p_GamepadIndex)
{
    p_InputMgr.AddSDLJoystickDPadDirectionToActionDictionnary(C_Actions::MENUUP, p_PlayerNumber, p_GamepadIndex, SPInputDefinition::DPAD_UP);
    p_InputMgr.AddSDLJoystickDPadDirectionToActionDictionnary(C_Actions::MENUDOWN, p_PlayerNumber, p_GamepadIndex, SPInputDefinition::DPAD_DOWN);
    p_InputMgr.AddSDLJoystickDPadDirectionToActionDictionnary(C_Actions::MENULEFT, p_PlayerNumber, p_GamepadIndex, SPInputDefinition::DPAD_LEFT);
    p_InputMgr.AddSDLJoystickDPadDirectionToActionDictionnary(C_Actions::MENURIGHT, p_PlayerNumber, p_GamepadIndex, SPInputDefinition::DPAD_RIGHT);
    p_InputMgr.AddSDLJoystickButtonToActionDictionnary(C_Actions::MENUVALIDATE, p_PlayerNumber, p_GamepadIndex, 0);
    p_InputMgr.AddSDLJoystickButtonToActionDictionnary(C_Actions::MENUCANCEL, p_PlayerNumber, p_GamepadIndex, 1);

    // TODO: Axis
    p_InputMgr.AddSDLJoystickDPadDirectionToActionDictionnary(C_Actions::UP, p_PlayerNumber, p_GamepadIndex, SPInputDefinition::DPAD_UP);
    p_InputMgr.AddSDLJoystickDPadDirectionToActionDictionnary(C_Actions::DOWN, p_PlayerNumber, p_GamepadIndex, SPInputDefinition::DPAD_DOWN);
    p_InputMgr.AddSDLJoystickDPadDirectionToActionDictionnary(C_Actions::LEFT, p_PlayerNumber, p_GamepadIndex, SPInputDefinition::DPAD_LEFT);
    p_InputMgr.AddSDLJoystickDPadDirectionToActionDictionnary(C_Actions::RIGHT, p_PlayerNumber, p_GamepadIndex, SPInputDefinition::DPAD_RIGHT);
    p_InputMgr.AddSDLJoystickButtonToActionDictionnary(C_Actions::SWAP, p_PlayerNumber, p_GamepadIndex, 0);
    p_InputMgr.AddSDLJoystickButtonToActionDictionnary(C_Actions::RAISE, p_PlayerNumber, p_GamepadIndex, 2);
    p_InputMgr.AddSDLJoystickButtonToActionDictionnary(C_Actions::PAUSE, p_PlayerNumber, p_GamepadIndex, 7);

}

void SetDefaultControls(KeyboardControls& p_Ctrls)
{
    // Menu controls
    p_Ctrls.m_Validate = SDLK_RETURN;
    p_Ctrls.m_Cancel = SDLK_ESCAPE;
    p_Ctrls.m_Up = SDLK_UP;
    p_Ctrls.m_Down = SDLK_DOWN;
    p_Ctrls.m_Left = SDLK_LEFT;
    p_Ctrls.m_Right = SDLK_RIGHT;

    // Player 1 controls
    p_Ctrls.p1_Up = SDLK_UP;
    p_Ctrls.p1_Down = SDLK_DOWN;
    p_Ctrls.p1_Left = SDLK_LEFT;
    p_Ctrls.p1_Right = SDLK_RIGHT;
    p_Ctrls.p1_Swap = SDLK_SPACE;
    p_Ctrls.p1_Raise = SDLK_RSHIFT;
    p_Ctrls.p1_Pause = SDLK_BACKSPACE;

    // Player 2 controls
    p_Ctrls.p2_Up = SDLK_w;
    p_Ctrls.p2_Down = SDLK_s;
    p_Ctrls.p2_Left = SDLK_a;
    p_Ctrls.p2_Right = SDLK_d;
    p_Ctrls.p2_Swap = SDLK_g;
    p_Ctrls.p2_Raise = SDLK_f;
    p_Ctrls.p2_Pause = SDLK_TAB;
}

bool ControlsIniExist()
{
    std::ifstream configFile("controls.cfg", std::ios::in);

    if (configFile)
    {
        configFile.close();
        return true;
    }
    else
    {
        return false;
    }
}

bool ControlsLoadFromIni(KeyboardControls& p_Ctrls)
{
    std::ifstream configFile("controls.cfg", std::ios::in);

    if (configFile)
    {
        long tmp;

        // Player 1 controls
        configFile >> tmp;
        p_Ctrls.p1_Up = (SDLKey)tmp;
        configFile >> tmp;
        p_Ctrls.p1_Down = (SDLKey)tmp;
        configFile >> tmp;
        p_Ctrls.p1_Left = (SDLKey)tmp;
        configFile >> tmp;
        p_Ctrls.p1_Right = (SDLKey)tmp;
        configFile >> tmp;
        p_Ctrls.p1_Swap = (SDLKey)tmp;
        configFile >> tmp;
        p_Ctrls.p1_Raise = (SDLKey)tmp;
        configFile >> tmp;
        p_Ctrls.p1_Pause = (SDLKey)tmp;

        // Player 2 controls
        configFile >> tmp;
        p_Ctrls.p2_Up = (SDLKey)tmp;
        configFile >> tmp;
        p_Ctrls.p2_Down = (SDLKey)tmp;
        configFile >> tmp;
        p_Ctrls.p2_Left = (SDLKey)tmp;
        configFile >> tmp;
        p_Ctrls.p2_Right = (SDLKey)tmp;
        configFile >> tmp;
        p_Ctrls.p2_Swap = (SDLKey)tmp;
        configFile >> tmp;
        p_Ctrls.p2_Raise = (SDLKey)tmp;
        configFile >> tmp;
        p_Ctrls.p2_Pause = (SDLKey)tmp;

        configFile.close();
    }
    else
    {
        SPException e("EXCEPTION - KeyBoardControls::ControlsLoadFromIni : File opening failed");
        throw e;
    }

    return true;
}

bool ControlsSaveToIni(KeyboardControls& p_Ctrls)
{
    std::ofstream configFile("controls.cfg", std::ios::out | std::ios::trunc );

    if (configFile)
    {
        // Player 1 controls
        configFile << (long)p_Ctrls.p1_Up << std::endl;
        configFile << (long)p_Ctrls.p1_Down << std::endl;
        configFile << (long)p_Ctrls.p1_Left << std::endl;
        configFile << (long)p_Ctrls.p1_Right << std::endl;
        configFile << (long)p_Ctrls.p1_Swap << std::endl;
        configFile << (long)p_Ctrls.p1_Raise << std::endl;
        configFile << (long)p_Ctrls.p1_Pause << std::endl;

        // Player 2 controls
        configFile << (long)p_Ctrls.p2_Up << std::endl;
        configFile << (long)p_Ctrls.p2_Down << std::endl;
        configFile << (long)p_Ctrls.p2_Left << std::endl;
        configFile << (long)p_Ctrls.p2_Right << std::endl;
        configFile << (long)p_Ctrls.p2_Swap << std::endl;
        configFile << (long)p_Ctrls.p2_Raise << std::endl;
        configFile << (long)p_Ctrls.p2_Pause << std::endl;

        configFile.close();
    }
    else
    {
        SPException e("EXCEPTION - KeyBoardControls::ControlsSaveToIni : File opening failed");
        throw e;
    }

    return true;
}
