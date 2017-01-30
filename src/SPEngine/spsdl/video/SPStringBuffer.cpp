#include "SPStringBuffer.h"

SPStringBuffer::SPStringBuffer()
{
    this->SetColor(0, 0, 0);
}

SPStringBuffer::~SPStringBuffer()
{

}

void SPStringBuffer::SetColor(unsigned long p_red, unsigned long p_green, unsigned long p_blue)
{
    this->m_TextColor.r = p_red;
    this->m_TextColor.g = p_green;
    this->m_TextColor.b = p_blue;
}

void SPStringBuffer::RasterizeString(std::string p_sText, SPTTFFont& p_Font, bool p_Force)
{
    // Ne faire le rendu que si nécessaire :
    if (this->GetLastRasterizedText() != p_sText || p_Force)
    {
            // Supprimer le texte actuellement rendu
        if ( this->isSurfaceLoaded() )
        {
            this->freeSurface();
        }

        SDL_Surface* saucepan = TTF_RenderUTF8_Blended(p_Font.GetFontPointer(), p_sText.c_str() , m_TextColor);

        this->setSurface( saucepan );

        // Si aucun rendu n'a été effectué alors qu'un texte valide existe : exception
        if ( !this->isSurfaceLoaded() && (p_sText != "") )
        {
            std::string sMess("SPStringBuffer::RasterizeText() : Unable to render string \"" + p_sText + "\".\n");
            throw SPException(sMess);
        }

        // Retenir le dernier texte rendu
        this->m_LastRasterizedText = p_sText;
    }


}

