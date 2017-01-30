#ifndef SPASCIIBMP_H
#define SPASCIIBMP_H

#include <string>
#include "SPEngine/spsdl/video/SPTexture.h"
#include "SPEngine/spsdl/video/SPDisplayWindow.h"

class SPAsciiBMP
{
    public:
        SPAsciiBMP();
        SPAsciiBMP(std::string p_FontPath, int p_WPerChar, int p_HPerChar);
        virtual ~SPAsciiBMP();
        SPTexture GetBMPFont() { return m_BMPFont; }
        //void SetBMPFont(SPTexture val) { m_BMPFont = val; }

        void LoadFont(std::string p_FontPath, int p_WPerChar, int p_HPerChar);

        void RenderTextOnScreen(SPDisplayWindow& p_Window, int p_X, int p_Y, std::string p_Text);
        void RenderTextOnTexture(SPTexture& p_Window, int p_X, int p_Y, std::string p_Text); // A inclure plus tard ?

    protected:
    private:
        SPTexture m_BMPFont;
        int m_WPerChar;
        int m_HPerChar;

        // faciliter la recherche du bon caractère
        int m_CharPerLine;

        int GetWPerChar() { return m_WPerChar; }
        void SetWPerChar(int val) { m_WPerChar = val; }
        int GetHPerChar() { return m_HPerChar; }
        void SetHPerChar(int val) { m_HPerChar = val; }

        bool FindCharPos(char p_Char, int& p_X, int& p_Y);

};

#endif // SPASCIIBMP_H
