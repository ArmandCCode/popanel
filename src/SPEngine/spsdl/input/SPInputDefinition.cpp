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
