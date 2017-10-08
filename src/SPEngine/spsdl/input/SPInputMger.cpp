#include "SPInputMger.h"

SPInputMger::SPInputMger()
{
    //ctor
    this->m_PendingExitEvent = false;
    this->m_KeyboardKeyWasPressed = false;
}

SPInputMger::~SPInputMger()
{
    //dtor
}

int SPInputMger::SDLPollEvent(SDL_Event* event)
{
    // It is still necessary to access this, if only for the custom controls state
    return SDL_PollEvent(event);
}

void SPInputMger::AddSDLKeyboardInputToActionDictionnary(int p_DesiredAction, int p_AssignedPlayerNumber, SDLKey p_DesiredKey)
{
    // Add the definition
    SPInputDefinition newDef;
    newDef.MapInputIDToKeyboardButton(p_DesiredAction, p_AssignedPlayerNumber, p_DesiredKey);

    this->m_InputDictionnary.push_back(newDef);
}

void SPInputMger::AddSDLJoystickButtonToActionDictionnary(int p_DesiredAction, int p_AssignedPlayerNumber, long p_GamepadIndex, long p_ButtonNumber)
{
    // Add the definition
    SPInputDefinition newDef;
    newDef.MapActionIDToGamepadButton(p_DesiredAction, p_AssignedPlayerNumber, p_GamepadIndex, p_ButtonNumber);

    this->m_InputDictionnary.push_back(newDef);
}

void SPInputMger::AddSDLJoystickDPadDirectionToActionDictionnary(int p_DesiredAction, int p_AssignedPlayerNumber, long p_GamepadIndex, long p_DirectionIndex)
{
    // Add the definition
    SPInputDefinition newDef;
    newDef.MapActionIDToGamepadDPad(p_DesiredAction, p_AssignedPlayerNumber, p_GamepadIndex, p_DirectionIndex);

    this->m_InputDictionnary.push_back(newDef);
}

void SPInputMger::ResetActionDictionnary()
{
    this->m_InputDictionnary.clear();
}

void SPInputMger::ConvertSDLEventsToSPEvents()
{
    // Time to create an actual SDL Loop
    SDL_Event event;
    while (this->SDLPollEvent(&event))
    {
        // check for messages
        switch (event.type)
        {
        // exit if the window is closed, any case
        case SDL_QUIT:
            this->m_PendingExitEvent = true;
            break;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            {
                this->m_KeyboardKeyWasPressed = true;
                // Find if the pressed key is in the dictionnary
                for (unsigned long i = 0; i < this->m_InputDictionnary.size(); i++)
                {
                    // Skip any input definition that is not a keyboard event
                    if (this->m_InputDictionnary[i].GetInputSource() == SPInputDefinition::KEYBOARD)
                    {
                        if (this->m_InputDictionnary[i].GetInputSDLKey() == event.key.keysym.sym)
                        {
                            SPEvent e;
                            e.SetActionNumber(this->m_InputDictionnary[i].GetActionID());
                            e.SetPlayerNumber(this->m_InputDictionnary[i].GetPlayerNumber());
                            e.SetPressEvent( (event.type == SDL_KEYDOWN) );
                            this->m_EventList.push_back(e);
                        }
                    }
                }
            }
            break;
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
            {
                this->m_KeyboardKeyWasPressed = true;
                // Find if the pressed key is in the dictionnary
                for (unsigned long i = 0; i < this->m_InputDictionnary.size(); i++)
                {
                    // Skip any input definition that is not a keyboard event
                    if (this->m_InputDictionnary[i].GetInputSource() == SPInputDefinition::GAMEPADBUTTON)
                    {
                        if ( (this->m_InputDictionnary[i].GetButtonNumber() == event.jbutton.button)
                                && this->m_InputDictionnary[i].GetGamepadIndex() == event.jbutton.which )
                        {
                            SPEvent e;
                            e.SetActionNumber(this->m_InputDictionnary[i].GetActionID());
                            e.SetPlayerNumber(this->m_InputDictionnary[i].GetPlayerNumber());
                            e.SetPressEvent( (event.type == SDL_JOYBUTTONDOWN) );
                            this->m_EventList.push_back(e);
                        }
                    }
                }
            }
            break;
        case SDL_JOYHATMOTION:
            if ( (event.jhat.value != this->m_JoysticksLastPressedHatDirection[event.jhat.which])
                && (this->m_JoysticksLastPressedHatDirection[event.jhat.which] != SDL_HAT_CENTERED) )
            {
                // Send a 'KeyUp' type event of the last used direction
                for (unsigned long i = 0; i < this->m_InputDictionnary.size(); i++)
                {
                    // Skip any input definition that is not a keyboard event
                    if (this->m_InputDictionnary[i].GetInputSource() == SPInputDefinition::GAMEPADDPAD)
                    {
                        if ( (this->m_InputDictionnary[i].GetDirectionIndex() == this->m_JoysticksLastPressedHatDirection[event.jhat.which])
                                && this->m_InputDictionnary[i].GetGamepadIndex() == event.jhat.which )
                        {
                            SPEvent e;
                            e.SetActionNumber(this->m_InputDictionnary[i].GetActionID());
                            e.SetPlayerNumber(this->m_InputDictionnary[i].GetPlayerNumber());
                            e.SetPressEvent( false );
                            this->m_EventList.push_back(e);
                        }
                    }
                }
            }

            this->m_JoysticksLastPressedHatDirection[event.jhat.which] = event.jhat.value;

            // If the DPad reports being centered, ignore the event...
            if ( !(event.jhat.value & SDL_HAT_CENTERED) )
            {

                //TODO: A loop for each direction detected ? This could be refactored....
                if ( event.jhat.value & SDL_HAT_UP )
                {
                    // Loop for that direction, and so on
                    for (unsigned long i = 0; i < this->m_InputDictionnary.size(); i++)
                    {
                        // Skip any input definition that is not a keyboard event
                        if (this->m_InputDictionnary[i].GetInputSource() == SPInputDefinition::GAMEPADDPAD)
                        {
                            if ( (this->m_InputDictionnary[i].GetDirectionIndex() == SPInputDefinition::DPAD_UP)
                                    && this->m_InputDictionnary[i].GetGamepadIndex() == event.jhat.which )
                            {
                                SPEvent e;
                                e.SetActionNumber(this->m_InputDictionnary[i].GetActionID());
                                e.SetPlayerNumber(this->m_InputDictionnary[i].GetPlayerNumber());
                                e.SetPressEvent( true );
                                this->m_EventList.push_back(e);
                            }
                        }
                    }
                }

                if ( event.jhat.value & SDL_HAT_DOWN )
                {
                    // Loop for that direction, and so on
                    for (unsigned long i = 0; i < this->m_InputDictionnary.size(); i++)
                    {
                        // Skip any input definition that is not a keyboard event
                        if (this->m_InputDictionnary[i].GetInputSource() == SPInputDefinition::GAMEPADDPAD)
                        {
                            if ( (this->m_InputDictionnary[i].GetDirectionIndex() == SPInputDefinition::DPAD_DOWN)
                                    && this->m_InputDictionnary[i].GetGamepadIndex() == event.jhat.which )
                            {
                                SPEvent e;
                                e.SetActionNumber(this->m_InputDictionnary[i].GetActionID());
                                e.SetPlayerNumber(this->m_InputDictionnary[i].GetPlayerNumber());
                                e.SetPressEvent( true );
                                this->m_EventList.push_back(e);
                            }
                        }
                    }
                }

                if ( event.jhat.value & SDL_HAT_LEFT )
                {
                    // Loop for that direction, and so on
                    for (unsigned long i = 0; i < this->m_InputDictionnary.size(); i++)
                    {
                        // Skip any input definition that is not a keyboard event
                        if (this->m_InputDictionnary[i].GetInputSource() == SPInputDefinition::GAMEPADDPAD)
                        {
                            if ( (this->m_InputDictionnary[i].GetDirectionIndex() == SPInputDefinition::DPAD_LEFT)
                                    && this->m_InputDictionnary[i].GetGamepadIndex() == event.jhat.which )
                            {
                                SPEvent e;
                                e.SetActionNumber(this->m_InputDictionnary[i].GetActionID());
                                e.SetPlayerNumber(this->m_InputDictionnary[i].GetPlayerNumber());
                                e.SetPressEvent( true );
                                this->m_EventList.push_back(e);
                            }
                        }
                    }
                }

                if ( event.jhat.value & SDL_HAT_RIGHT )
                {
                    // Loop for that direction, and so on
                    for (unsigned long i = 0; i < this->m_InputDictionnary.size(); i++)
                    {
                        // Skip any input definition that is not a keyboard event
                        if (this->m_InputDictionnary[i].GetInputSource() == SPInputDefinition::GAMEPADDPAD)
                        {
                            if ( (this->m_InputDictionnary[i].GetDirectionIndex() == SPInputDefinition::DPAD_RIGHT)
                                    && this->m_InputDictionnary[i].GetGamepadIndex() == event.jhat.which )
                            {
                                SPEvent e;
                                e.SetActionNumber(this->m_InputDictionnary[i].GetActionID());
                                e.SetPlayerNumber(this->m_InputDictionnary[i].GetPlayerNumber());
                                e.SetPressEvent( true );
                                this->m_EventList.push_back(e);
                            }
                        }
                    }
                }
            }
            break;
        default:
            break;
        }
    }
}

bool SPInputMger::SPPollEvent(SPEvent& event)
{
    if (this->m_EventList.size() > 0)
    {
        // return a copy of the first element, and remove it from the list
        event.SetActionNumber(this->m_EventList.front().GetActionNumber());
        event.SetPlayerNumber(this->m_EventList.front().GetPlayerNumber());
        event.SetPressEvent(this->m_EventList.front().IsPressEvent());

        this->m_EventList.pop_front();
        return true;
    }

    this->ResetSpecialEvents();

    return false;
}

void SPInputMger::ResetSpecialEvents()
{
    this->m_PendingExitEvent = false;
    this->m_KeyboardKeyWasPressed = false;
}

bool SPInputMger::OpenAllAvailableGamepads()
{
    if (this->GetNumberOfGamepads() == 0)
    {
        return false;
    }

    for (int i = 0; i < this->GetNumberOfGamepads(); i++)
    {
        SDL_Joystick* joy = SDL_JoystickOpen(i);
        this->m_JoystickList.push_back(joy);
        this->m_JoysticksLastPressedHatDirection.push_back(SDL_HAT_CENTERED);
    }

    return true;
}

int SPInputMger::GetNumberOfGamepads()
{
    return SDL_NumJoysticks();
}

std::string SPInputMger::GetGamepadName(int p_Index)
{
    if (p_Index >= this->m_JoystickList.size())
    {
        return "<Invalid joystick index>";
    }

    return SDL_JoystickName(p_Index);
}

void SPInputMger::CloseAllGamepads()
{
    for (int i = 0; i < this->m_JoystickList.size(); i++)
    {
        SDL_JoystickClose(this->m_JoystickList[i]);
    }
}
