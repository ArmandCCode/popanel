#ifndef TEXTSCROLLER_H
#define TEXTSCROLLER_H

#include <string>
#include "SPEngine/spsdl/timer/SPTimer.h"
#include "SPEngine/spsdl/video/SPStringBuffer.h"

class TextScroller
{
    public:
        TextScroller();
        virtual ~TextScroller();

        void SetString(std::string p_Text, SPTTFFont& p_RenderFont, unsigned long p_red, unsigned long p_green, unsigned long p_blue);

        void SetScrollerWidth(unsigned long p_width) { m_ScrollerWidth = p_width; }
        void SetScrollerSpeed(unsigned int p_Speed); // Stocke la vitesse

        void RestartScroll(); // Permet de redémarrer le défilement pour s'adapter à un changement d'écran

        // Dessine le cadre et le texte défilant, puis calculé
        void RenderToWindow(SPDisplayWindow& p_window, long p_X, long p_Y);
    protected:
    private:
        SPStringBuffer m_TextToScroll;
        float m_TextPosition; // convertie en long lors du rendu
        unsigned long m_LastDrawTime; // Stocke m_UITimer.GetElapsed() au moment de dessiner le contrôle"

        unsigned long m_ScrollerSpeed;
        unsigned long m_ScrollerWidth;
        unsigned long m_ScrollerHeight;

        SPTimer m_UITimer;
};

#endif // TEXTSCROLLER_H
