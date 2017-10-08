#ifndef SPINPUTDEFINITION_H
#define SPINPUTDEFINITION_H

#include <SDL/SDL.h>

class SPInputDefinition
{
    public:
        SPInputDefinition();
        virtual ~SPInputDefinition();

        // Keyboard
        void MapInputIDToKeyboardButton(int p_ActionID, int p_PlayerNumber, SDLKey p_SDLKey);

        // Gamepad
        void MapActionIDToGamepadButton(int p_ActionID, int p_PlayerNumber, long p_GamepadIndex, long p_ButtonNumber);
        void MapActionIDToGamepadDPad(int p_ActionID, int p_PlayerNumber, long p_GamepadIndex, long p_DirectionIndex);
        void MapActionIDToGamepadAxisThreshold(int p_ActionID, int p_PlayerNumber, long p_GamepadIndex, long p_AxisNumber, long p_AxisThreshold); //TODO

        static const int NONE = 0;
        static const int KEYBOARD = 1;
        static const int GAMEPADBUTTON = 2;
        static const int GAMEPADDPAD = 3;
        static const int GAMEPADAXIS = 4;

        static const int DPAD_UP = SDL_HAT_UP;
        static const int DPAD_DOWN = SDL_HAT_DOWN;
        static const int DPAD_LEFT = SDL_HAT_LEFT;
        static const int DPAD_RIGHT = SDL_HAT_RIGHT;

        int GetInputSource() { return m_InputSource; }
        int GetActionID() { return m_ActionID; }
        int GetPlayerNumber() { return m_PlayerNumber; }

        // Keyboard
        SDLKey GetInputSDLKey() { return m_InputSDLKey; }

        // Gamepad
        long GetGamepadIndex() { return m_GamepadIndex; }
        long GetButtonNumber() { return m_ButtonNumber; }
        long GetDirectionIndex() { return m_DirectionIndex; }
        long GetAxisNumber() { return m_AxisNumber; }
        long GetAxisThreshold() { return m_AxisThreshold; }

    protected:

    private:
        int m_InputSource;
        int m_ActionID;
        int m_PlayerNumber;

        // Keyboard
        SDLKey m_InputSDLKey;

        // Gamepad
        long m_GamepadIndex;
        long m_ButtonNumber;
        long m_DirectionIndex;
        long m_AxisNumber;
        long m_AxisThreshold;

};

#endif // SPINPUTDEFINITION_H
