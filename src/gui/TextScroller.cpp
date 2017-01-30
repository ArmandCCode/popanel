#include "TextScroller.h"

TextScroller::TextScroller() : m_TextPosition(0), m_LastDrawTime(0), m_ScrollerSpeed(0), m_ScrollerWidth(0), m_ScrollerHeight(0)
{

}

TextScroller::~TextScroller()
{

}

void TextScroller::SetString(std::string p_Text, SPTTFFont& p_RenderFont, unsigned long p_red, unsigned long p_green, unsigned long p_blue)
{
    this->m_TextToScroll.SetColor(p_red, p_green, p_blue);
    this->m_TextToScroll.RasterizeString(p_Text, p_RenderFont, true);
}

void TextScroller::SetScrollerSpeed(unsigned int p_Speed) // Stocke la vitesse
{
    this->m_ScrollerSpeed = p_Speed;
}

void TextScroller::RestartScroll()
{
    // Permet de redémarrer le défilement pour s'adapter à un changement d'écran
    this->m_UITimer.Restart();
    this->m_TextPosition = 0.f;
}

// Dessine le cadre et le texte défilant, puis calculé
void TextScroller::RenderToWindow(SPDisplayWindow& p_window, long p_X, long p_Y)
{
    // Calculer la nouvelle position de dessin
    unsigned long tmpNewTime = m_UITimer.GetElapsed();
    // Réinitialiser le timer pour decaler de la bonne valeur à chaque frame
    m_UITimer.Restart();

    // On calcule d'abord le temps en MS puis on divise par 1000.
    float tmpPixelsParcourus = ( (float)m_ScrollerSpeed * ((float)tmpNewTime / 1000.0000f) );
    m_TextPosition += tmpPixelsParcourus;

    // Si le texte dépasse la largeur maximale, on décale de "largeur texte + loargeur boite"
    unsigned long maxWidth = this->m_TextToScroll.getW() + this->m_ScrollerWidth;
    if (this->m_TextPosition > maxWidth )
    {
        this->m_TextPosition -= maxWidth;
    }

    // Dessin de la boîte
    unsigned long DrawH = this->m_TextToScroll.getH();
    unsigned long DrawW = this->m_TextToScroll.getW();
    long DrawX = p_X;
    long DrawY = p_Y;

    p_window.DrawSquareWithOuterBorder(DrawX, DrawY, this->m_ScrollerWidth, DrawH, 0, 0, 0, 128, 2, 255, 255, 255, 255);

    // Dessin du texte qui défile
    if (m_TextPosition < this->m_ScrollerWidth)
    {
        DrawW = this->m_TextPosition;
        DrawX = p_X + this->m_ScrollerWidth - this->m_TextPosition;
        this->m_TextToScroll.SetLocalRect(0, -1, DrawW, -1);
        this->m_TextToScroll.RenderToWindow(p_window, DrawX, DrawY);
    }
    else if (m_TextPosition >= this->m_ScrollerWidth && m_TextPosition < this->m_TextToScroll.getW() + this->m_ScrollerWidth)
    {
        DrawW = this->m_ScrollerWidth;
        DrawX = p_X;
        this->m_TextToScroll.SetLocalRect(m_TextPosition - DrawW, -1, m_TextPosition, -1);
        this->m_TextToScroll.RenderToWindow(p_window, DrawX, DrawY);
    }
}
