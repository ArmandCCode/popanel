#ifndef SPINPUTMGER_H
#define SPINPUTMGER_H

#include <string>
#include <vector>
#include <list>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

#include "SPEngine/SPException.h"
#include "SPInputDefinition.h"
#include "SPEvent.h"

// Contains various SDL specific event management stuff
//TODO: Will contain gamepad pointers in the future

class SPInputMger
{
    public:
        SPInputMger();
        virtual ~SPInputMger();
        // Direct access to SDL PollEvent if necessary
        int SDLPollEvent(SDL_Event* event);

        // Creating action dictionnary
        void AddSDLKeyboardInputToActionDictionnary(int p_DesiredAction, int p_AssignedPlayerNumber, SDLKey p_DesiredKey);
        void AddSDLJoystickButtonToActionDictionnary(int p_DesiredAction, int p_AssignedPlayerNumber, long p_GamepadIndex, long p_ButtonNumber);
        void AddSDLJoystickDPadDirectionToActionDictionnary(int p_DesiredAction, int p_AssignedPlayerNumber, long p_GamepadIndex, long p_DirectionIndex);
        void ResetActionDictionnary();

        // Processing SPEvents from SDL_Events, needs to be manually called
        void ConvertSDLEventsToSPEvents();
        bool SPPollEvent(SPEvent& event);
        bool IsExitEventPending() { return m_PendingExitEvent; };
        bool WasKeyboardKeyPressed() { return m_KeyboardKeyWasPressed; };

        // Gamepad support
        bool OpenAllAvailableGamepads(); // returns false if no gamepad was found
        int GetNumberOfGamepads();
        std::string GetGamepadName(int p_Index);
        void CloseAllGamepads();

    protected:

    private:
        std::vector<SPInputDefinition> m_InputDictionnary;

        // This frame's SPEvent list
        bool m_PendingExitEvent;
        bool m_KeyboardKeyWasPressed;
        std::list<SPEvent> m_EventList;

        void ResetSpecialEvents();

        // Gamepad support
        std::vector<SDL_Joystick*> m_JoystickList;
        std::vector<int> m_JoysticksLastPressedHatDirection;
};

#endif // SPINPUTMGER_H
