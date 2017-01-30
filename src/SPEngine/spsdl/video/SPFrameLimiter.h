#ifndef SPFRAMELIMITER_H
#define SPFRAMELIMITER_H

#include <SDL/SDL.h>

#include <math.h>
#include <iostream>

class SPFrameLimiter
{
    public:
        SPFrameLimiter();
        virtual ~SPFrameLimiter();

        // Frame limiter
        void SetFrameLimit(int p_FPS);
        void RestUntilNextFrame(); // Stops the process until the next Frame has to be drawn

        // Fps calculator
        void FrameDone();
        float GetLastFPS();

        // Enable or disable the limiter
        void Enable(bool p_Enable) { m_Enabled = p_Enable; }

    protected:
    private:
        // Frame limiter
        // members
        Uint32 m_SDLDelayTime; // Time spent by the computer after a SDL_Delay(10)
        float m_FPSCap;
        Uint32 m_TimePerFrame;

        float m_FramesLastSecond; // Calculated FPS last second
        float m_FramesThisSecond; // FPS in progress of being calculated
        Uint32 m_SecondStart;
        Uint32 m_TimeOfPreviousFrame;

        // Enabled or disabled ?
        bool m_Enabled;

        // internal functions
        void CalculateDelayTime();
};

#endif // SPFRAMELIMITER_H
