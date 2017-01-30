#include "MessageBox.h"

MessageBox::MessageBox() : m_MessageBoxWidth(0), m_MessageBoxTextCenter(false)
{

}

MessageBox::~MessageBox()
{

}

void MessageBox::SetLineString(unsigned long p_LineNumber, std::string p_Text, SPTTFFont& p_RenderFont, unsigned long p_red, unsigned long p_green, unsigned long p_blue)
{
    if (p_LineNumber < m_MessageText.size())
    {
        this->m_MessageText[p_LineNumber].SetColor(p_red, p_green, p_blue);
        this->m_MessageText[p_LineNumber].RasterizeString(p_Text, p_RenderFont, true);
    }
}

void MessageBox::RenderToWindow(SPDisplayWindow& p_window, long p_X, long p_Y, bool p_BoxCenter)
{
    unsigned long DrawH = 0; // Will be calculated
    unsigned long DrawW = this->m_MessageBoxWidth;
    long DrawX = p_X;
    long DrawY = p_Y;

    // Calculate total height of the messagebox
    for (int i = 0; i < m_MessageText.size(); i++)
    {
        DrawH += this->m_MessageText[i].getH();
    }

    if (this->isAutoWidth())
    {
        // Adapt to the largest line in the message box
        DrawW = 0;
        for (int i = 0; i < m_MessageText.size(); i++)
        {
            if (this->m_MessageText[i].getW() > DrawW)
            {
                DrawW = this->m_MessageText[i].getW();
            }
        }

    }

    // Drawing the box still happens from a top-left perspective : convert the center coords to top-left coords
    if (p_BoxCenter)
    {
        DrawX = DrawX - floor((float)DrawW / 2.f);
        DrawY = DrawY - floor((float)DrawH / 2.f);
    }

    p_window.DrawSquareWithOuterBorder(DrawX, DrawY, DrawW, DrawH, 0, 0, 0, 128, 2, 255, 255, 255, 255);

    // Drawing all the text
    for (unsigned long i = 0; i < m_MessageText.size(); i++)
    {
        // Trying to draw an empty surface will crash the game
        if (m_MessageText[i].getH() > 0 || m_MessageText[i].getW() > 0 )
        {
            if (m_MessageBoxTextCenter)
            {
                // Drawing coordinates are usually top-left : convert to center for the text drawing
                long textDrawX = DrawX + floor((float)DrawW / 2.f);
                long textDrawY = DrawY + floor((float)this->m_MessageText[i].getH() / 2.f);
                this->m_MessageText[i].RenderToWindow(p_window, textDrawX, textDrawY, SPStringBuffer::RENDER_FROMCENTER);
            }
            else
            {
                this->m_MessageText[i].RenderToWindow(p_window, DrawX, DrawY);
            }

            // Move the top-left origin down to the bottom of the line of text
            DrawY += this->m_MessageText[i].getH();
        }
    }


}

bool MessageBox::isAutoWidth()
{
    // When m_MessageBoxWidth is 0, we calculate the width to fill all the text.
    if (this->m_MessageBoxWidth > 0)
        return false;
    else
        return true;
}

void MessageBox::SetLinesInMessage(unsigned long p_LinesNumber)
{

    if (p_LinesNumber < m_MessageText.size())
    {
        // Delete ALL the text... That exceeds the number of lines we asked for
        for (int i = m_MessageText.size() ; i > p_LinesNumber; i--)
        {
            m_MessageText.pop_back();
        }
    }
    else if (p_LinesNumber > m_MessageText.size())
    {
        // Add new String buffers until we have as many lines of text as desired
        for (int i = m_MessageText.size() ; i < p_LinesNumber; i++)
        {
            m_MessageText.push_back(SPStringBuffer());
        }
    }



}
