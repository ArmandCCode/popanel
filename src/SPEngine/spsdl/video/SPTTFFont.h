#ifndef SPTTFFONT_H
#define SPTTFFONT_H

#include <SDL/SDL_ttf.h>

#include <string>
#include "SPEngine/SPException.h"

class SPTTFFont
{
    public:
        SPTTFFont();
        virtual ~SPTTFFont();
        void LoadFont(std::string p_sPath, unsigned long p_Size);
        void UnloadFont();
        TTF_Font* GetFontPointer() { return m_Font; }
        unsigned long GetFontLineSkip(); // fonction SDL_ttf, permet de récupérer la hauteurs de lignes idéale pour la police chargée
        unsigned long GetRequestedHeight() { return m_RequestedHeight; }
    protected:
    private:
        TTF_Font* m_Font;
        unsigned long m_RequestedHeight; // Purement informatif, pour aider au rendu
};

#endif // SPTTFFONT_H
