#include "SPEngine/spsdl/video/SPFrameLimiter.h"

SPFrameLimiter::SPFrameLimiter() : m_SDLDelayTime(0), m_FPSCap(30), m_TimePerFrame(0), m_FramesLastSecond(0), m_FramesThisSecond(0), m_SecondStart(0), m_TimeOfPreviousFrame(0), m_Enabled(false)
{

    this->SetFrameLimit(30);
}

SPFrameLimiter::~SPFrameLimiter()
{

}

void SPFrameLimiter::CalculateDelayTime()
{
    // Calcul du temps que met SDL_Delay a passer 10ms
    this->m_SDLDelayTime = SDL_GetTicks();
    SDL_Delay(10);
    this->m_SDLDelayTime = SDL_GetTicks() - this->m_SDLDelayTime;
}

void SPFrameLimiter::SetFrameLimit(int p_FPS)
{
    // recalculate every indicator
    CalculateDelayTime();
    this->m_FPSCap = p_FPS;
    this->m_TimePerFrame = floor(1000.00 / m_FPSCap);
    this->m_FramesLastSecond = 0.0;
    this->m_FramesThisSecond = 0.0;
    this->m_SecondStart = SDL_GetTicks();
}

void SPFrameLimiter::FrameDone()
{
    Uint32 iSDLTicks = SDL_GetTicks();

    if (iSDLTicks > this->m_SecondStart + 1000 )
    {
        this->m_FramesLastSecond = this->m_FramesThisSecond;
        this->m_FramesThisSecond = 1.0;
        this->m_SecondStart = iSDLTicks;
    }
    else
    {
        this->m_FramesThisSecond += 1.0;
    }

}

float SPFrameLimiter::GetLastFPS()
{
    return this->m_FramesLastSecond;
}

void SPFrameLimiter::RestUntilNextFrame() // Stops the process until the next Frame has to be drawn
{
    Uint32 tmpTicks = SDL_GetTicks();

    if ( this->m_Enabled )
    {
        // While we are not late for the next frame...
        while ( tmpTicks - this->m_TimeOfPreviousFrame < this->m_TimePerFrame)
        {
            // Let the process rest for "time between two frames" - "time that has already elapsed since last frame was done"
            SDL_Delay(this->m_TimePerFrame - (tmpTicks - this->m_TimeOfPreviousFrame));
            tmpTicks = SDL_GetTicks();
        }
    }

    this->m_TimeOfPreviousFrame = tmpTicks;
}
