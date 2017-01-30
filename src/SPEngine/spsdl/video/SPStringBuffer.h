#ifndef SPSTRINGBUFFER_H
#define SPSTRINGBUFFER_H

#include <string>
#include "SPTexture.h"
#include "SPTTFFont.h"

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

class SPStringBuffer : public SPTexture
{
    public:
        SPStringBuffer();
        virtual ~SPStringBuffer();
        void SetColor(unsigned long p_red, unsigned long p_green, unsigned long p_blue);
        void RasterizeString(std::string p_sText, SPTTFFont& p_Font, bool p_Force = false);
        std::string& GetLastRasterizedText() { return m_LastRasterizedText; } // Permet de ne faire le rendu que lorsque nécessaire
    protected:
    private:
        SDL_Color m_TextColor;
        std::string m_LastRasterizedText;

};

#endif // SPSTRINGBUFFER_H
