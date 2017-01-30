#include "FaderColor.h"

FaderColor::FaderColor()
{

    SetFadingType(FaderColor::FADEIN);
    SetFadingLength(0); // Setting the default value to 0 prevents isFadingDone() from returning false after initializing the object, which would cause a black screen
    SetColor(0, 0, 0);
}

FaderColor::~FaderColor()
{

}

void FaderColor::SetColor(unsigned long p_red, unsigned long p_green, unsigned long p_blue)
{
    this->m_FadingR = p_red;
    this->m_FadingG = p_green;
    this->m_FadingB = p_blue;
}

void FaderColor::SetFadingLength(unsigned long p_length)
{
    this->m_FadingTimer.SetLength(p_length);
}


void FaderColor::StartFadingNow()
{
    this->m_FadingTimer.Restart();
}

bool FaderColor::isFadingDone()
{
    if ( this->m_FadingTimer.IsTimerEnd() )
    {
        return true;
    }
    return false;
}

void FaderColor::RenderToWindow(SPDisplayWindow& p_Window)
{
    int ScreenW = p_Window.GetWidth();
    int ScreenH = p_Window.GetHeight();

    float PcElapsed = this->m_FadingTimer.GetElapsedPc();

    // Avoid looping the effect by keeping the elapsed time between minimum and maximum
    if (PcElapsed > 1.0f)
    {
        PcElapsed = 1.0f;
    }
    if (PcElapsed < 0.0f)
    {
        PcElapsed = 0.0f;
    }

    unsigned long Transparency = 255.00f * PcElapsed;

    if (this->m_FadingType == FaderColor::FADEIN && !this->isFadingDone() )
    {
        // It is pointless to try to render the fading-in effect after it is finished, as it is just fully transparent...
        p_Window.DrawSquare(0, 0, ScreenW, ScreenH, 0, 0, 0, 255 - Transparency);
    }
    else if (this->m_FadingType == FaderColor::FADEOUT)
    {
        // Screen stays of the fading color after it is done
        p_Window.DrawSquare(0, 0, ScreenW, ScreenH, 0, 0, 0, Transparency);
    }
}
