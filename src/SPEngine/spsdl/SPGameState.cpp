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

    // Initialiser le delta à la première frame
    this->m_SDLTimestampOfLastFrame = SDL_GetTicks();

    while (bGo)
    {
        bGo = this->Play( this->ComputeDelta() );
        this->Frame(); // Cela permet de dessiner la dernière image quand on quitte l'état
    }

}

Uint32 SPGameState::ComputeDelta()
{

    Uint32 CurrentTick = SDL_GetTicks();
    Uint32 tmpDelta = 0;

    // Cas particulier : le compteur SDL_GetTicks arrive au maximum de sa capacité :
    // - il faut lui renvoyer le temps écoulé avant la fin de la capacité également
    // - ce cas pourrait être ignoré (peu de chances que le jeu tourne pendant 49 jours...)
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
