#include "SPEngine/spsdl/SPGameState.h"

SPGameState::SPGameState(SPSDL& p_SDLRef) : m_SDL(p_SDLRef), m_SDLTimestampOfLastFrame(0)
{

}

SPGameState::~SPGameState()
{

}

void SPGameState::EnterState()
{
    bool bGo = true;

    this->m_SDLTimestampOfLastFrame = SDL_GetTicks();

    while (bGo)
    {
        bGo = this->Play( this->ComputeDelta() );
        this->Frame();
    }

}

Uint32 SPGameState::ComputeDelta()
{

    Uint32 CurrentTick = SDL_GetTicks();
    Uint32 tmpDelta = 0;

    // Edge case: if SDL_GetTicks reaches the maximum, it loops back to the start. The delta should account for the loopback in that case.
    if (CurrentTick < this->m_SDLTimestampOfLastFrame) {
        tmpDelta = CurrentTick + ~(this->m_SDLTimestampOfLastFrame);
    }
    else
    {
        tmpDelta = CurrentTick - this->m_SDLTimestampOfLastFrame;
    }

    this->m_SDLTimestampOfLastFrame = CurrentTick;
    return tmpDelta;
}
