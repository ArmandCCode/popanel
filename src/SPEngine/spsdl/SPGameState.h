#ifndef SPGAMESTATE_H
#define SPGAMESTATE_H

#include "SPSDL.h"
#include "SPBaseState.h"

class SPGameState : public SPBaseState
{
    public:
        SPGameState(SPSDL& p_SDLRef);
        virtual ~SPGameState();
        void EnterState();
        virtual bool Play(long p_Delta) = 0; // Events and game logic
    protected:
        SPSDL& m_SDL;
        Uint32 ComputeDelta();
    private:
        Uint32 m_SDLTimestampOfLastFrame;

};

#endif // SPGAMESTATE_H
