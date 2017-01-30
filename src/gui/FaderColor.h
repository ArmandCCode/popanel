#ifndef FADERCOLOR_H
#define FADERCOLOR_H

#include "SPEngine/spsdl/video/SPDisplayWindow.h"
#include "SPEngine/spsdl/timer/SPTimer.h"

class FaderColor
{
    public:
        FaderColor();
        virtual ~FaderColor();

        // Transition settings
        void SetColor(unsigned long p_red, unsigned long p_green, unsigned long p_blue);
        void SetFadingLength(unsigned long p_length);
        void SetFadingType(int p_FadingType) { m_FadingType = p_FadingType; }
        int GetFadingType() { return m_FadingType; }

        static const int FADEIN = 0;
        static const int FADEOUT = 1;

        // Fading controls
        void StartFadingNow();
        bool isFadingDone();

        // Screen rendering
        void RenderToWindow(SPDisplayWindow& p_Window);
    protected:
    private:
        SPTimer m_FadingTimer;
        unsigned long m_FadingR;
        unsigned long m_FadingG;
        unsigned long m_FadingB;
        int m_FadingType;
};

#endif // FADERCOLOR_H
