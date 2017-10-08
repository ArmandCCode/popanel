#include "SPInputDefinition.h"

SPInputDefinition::SPInputDefinition()
{
    //ctor
}

SPInputDefinition::~SPInputDefinition()
{
    //dtor
}

void SPInputDefinition::MapInputIDToKeyboardButton(int p_ActionID, int p_PlayerNumber, SDLKey p_SDLKey)
{
    this->m_InputSource = SPInputDefinition::KEYBOARD;
    this->m_ActionID = p_ActionID;
    this->m_PlayerNumber = p_PlayerNumber;
    this->m_InputSDLKey = p_SDLKey;
}

// Gamepad
// TODO: Why not separate those different types of input in different classes ? One for each type, then the SPInputMger goes through every list...
void SPInputDefinition::MapActionIDToGamepadButton(int p_ActionID, int p_PlayerNumber, long p_GamepadIndex, long p_ButtonNumber)
{
    this->m_InputSource = SPInputDefinition::GAMEPADBUTTON;
    this->m_ActionID = p_ActionID;
    this->m_PlayerNumber = p_PlayerNumber;

    this->m_GamepadIndex = p_GamepadIndex;
    this->m_ButtonNumber = p_ButtonNumber;
}

void SPInputDefinition::MapActionIDToGamepadDPad(int p_ActionID, int p_PlayerNumber, long p_GamepadIndex, long p_DirectionIndex)
{
    this->m_InputSource = SPInputDefinition::GAMEPADDPAD;
    this->m_ActionID = p_ActionID;
    this->m_PlayerNumber = p_PlayerNumber;

    this->m_GamepadIndex = p_GamepadIndex;
    this->m_DirectionIndex = p_DirectionIndex;
}

void SPInputDefinition::MapActionIDToGamepadAxisThreshold(int p_ActionID, int p_PlayerNumber, long p_GamepadIndex, long p_AxisNumber, long p_AxisThreshold)
{
     //TODO
}
