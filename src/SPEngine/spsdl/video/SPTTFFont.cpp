#include "SPTTFFont.h"

SPTTFFont::SPTTFFont() : m_Font(NULL)
{

}

SPTTFFont::~SPTTFFont()
{

    this->UnloadFont();
}

void SPTTFFont::LoadFont(std::string p_sPath, unsigned long p_Size)
{
    if (m_Font != NULL)
    {
        this->UnloadFont();
    }

    // Sauvegarder la hauteur demandée pour plus tard
    m_RequestedHeight = p_Size;

    m_Font = TTF_OpenFont( p_sPath.c_str() , m_RequestedHeight );

    if (m_Font == NULL)
    {
        SPException e("SPTTFFont::LoadFont() : Unable to open font file at " + p_sPath );
        throw e;
    }
}

void SPTTFFont::UnloadFont()
{
    if (m_Font != NULL)
    {
        TTF_CloseFont(m_Font);
    }
}

unsigned long SPTTFFont::GetFontLineSkip()
{
    if (m_Font != NULL)
    {
        return TTF_FontLineSkip(m_Font);
    }
    return 0;
}
