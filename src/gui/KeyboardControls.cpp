#include "KeyboardControls.h"

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
    p_Ctrls.p1_Pause = SDLK_ESCAPE;

    // Player 2 controls
    p_Ctrls.p2_Up = SDLK_w;
    p_Ctrls.p2_Down = SDLK_s;
    p_Ctrls.p2_Left = SDLK_a;
    p_Ctrls.p2_Right = SDLK_d;
    p_Ctrls.p2_Swap = SDLK_g;
    p_Ctrls.p2_Raise = SDLK_f;
    p_Ctrls.p2_Pause = SDLK_ESCAPE;
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
