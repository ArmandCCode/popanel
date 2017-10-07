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
            // Only support keyboard commands for the moment
            {
                this->m_KeyboardKeyWasPressed = true;
                // Find if the pressed key is in the dictionnary
                for (unsigned long i = 0; i < this->m_InputDictionnary.size(); i++)
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
