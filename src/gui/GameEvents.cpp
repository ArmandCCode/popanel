#include "GameEvents.h"

GameEvents::GameEvents()
{

}

GameEvents::~GameEvents()
{

}

void GameEvents::AddEvent(int p_IDGameEvent)
{
    this->m_EventList.push(p_IDGameEvent);
}

bool GameEvents::isQueueEmpty()
{
    return (this->m_EventList.size() == 0);
}

int& GameEvents::GetCurrentEvent()
{
    if ( !this->isQueueEmpty() )
    {
        return this->m_EventList.front();
    }

    SPException e("EXCEPTION - There is no event in the event queue. please check before calling the event itself.");
    throw e;
}

void GameEvents::StepToNextEvent()
{
    if ( !this->isQueueEmpty() )
    {
        this->m_EventList.pop();
    }
}
