#ifndef SPINPUTDEFINITION_H
#define SPINPUTDEFINITION_H

#include <SDL/SDL.h>

class SPInputDefinition
{
    public:
        SPInputDefinition();
        virtual ~SPInputDefinition();
        void MapInputIDToKeyboardButton(int p_ActionID, int p_PlayerNumber, SDLKey p_SDLKey);

        static const int NONE = 0;
        static const int KEYBOARD = 1;
        static const int GAMEPAD = 2;

        int GetInputSource() { return m_InputSource; }
        int GetActionID() { return m_ActionID; }
        int GetPlayerNumber() { return m_PlayerNumber; }
        SDLKey GetInputSDLKey() { return m_InputSDLKey; }

    protected:

    private:
        int m_InputSource;
        int m_ActionID;
        int m_PlayerNumber;
        SDLKey m_InputSDLKey;
};

#endif // SPINPUTDEFINITION_H
