#include "SPEngine/spsdl/video/SPAsciiBMP.h"

SPAsciiBMP::SPAsciiBMP() : m_BMPFont(), m_WPerChar(0), m_HPerChar(0), m_CharPerLine(0)
{

    this->m_CharPerLine = 0;
}

SPAsciiBMP::SPAsciiBMP(std::string p_FontPath, int p_WPerChar, int p_HPerChar) : m_BMPFont(p_FontPath), m_WPerChar(p_WPerChar), m_HPerChar(p_HPerChar)
{
    LoadFont(p_FontPath,p_WPerChar,p_HPerChar);
}

SPAsciiBMP::~SPAsciiBMP()
{

}

void SPAsciiBMP::LoadFont(std::string p_FontPath, int p_WPerChar, int p_HPerChar)
{
    this->m_BMPFont.LoadSurface(p_FontPath);
    m_WPerChar = p_WPerChar;
    m_HPerChar = p_HPerChar;
    this->m_CharPerLine = m_BMPFont.getSurface()->w / m_WPerChar;
}

void SPAsciiBMP::RenderTextOnScreen(SPDisplayWindow& p_Window, int p_X, int p_Y, std::string p_Text)
{
    // Envoyer une exception si aucune image valide n'a �t� charg�e
    if (this->m_CharPerLine == 0)
    {
        SPException e("SPAsciiBMP::RenderTextOnScreen - EXCEPTION : BMP Font not loaded. Impossible to render text.");
        throw e;
    }

    int CharXCoord = 0;
    int CharYCoord = 0;
    long DrawX = 0;
    long DrawY = 0;

    for (unsigned int i = 0; i < p_Text.size(); i++)
    {
        char tmpChar = p_Text.at(i);
        DrawX = p_X + (i * m_WPerChar);
        DrawY = p_Y;
        // R�cup�rer les coordonn�es du caract�re souhait�
        this->FindCharPos(tmpChar, CharXCoord, CharYCoord);
        this->m_BMPFont.SetLocalRect(CharXCoord, CharYCoord, CharXCoord + m_WPerChar, CharYCoord + m_HPerChar);
        // Dessiner le caract�re souhait�
        this->m_BMPFont.RenderToWindow(p_Window, DrawX, DrawY);
        // R�initialise le "viseur" du caract�re
        this->m_BMPFont.ResetLocalRect();

    }
}

void SPAsciiBMP::RenderTextOnTexture(SPTexture& p_Window, int p_X, int p_Y, std::string p_Text) // A inclure plus tard ?
{

}

bool SPAsciiBMP::FindCharPos(char p_Char, int& p_X, int& p_Y)
{
    // initialiser les entiers
    p_X = 0;
    p_Y = 0;

    if (p_Char >= 32 && p_Char < 96)
    {
        // Chercher le caract�re directement
    }
    else if (p_Char >= 96 && p_Char < 123)
    {
        // Convertir les minuscules en majuscules puis chercher le caract�re r�sultant
        p_Char = p_Char - 32;
    }
    else
    {
        // Pour les caract�res hors de la police, on remplace par un espace
        p_Char = 32;
    }

    p_Char -= 32;  // Le tileset d�marre 32 caract�res en arri�re !

    p_X = p_Char * m_WPerChar;

    while (p_X > (this->m_CharPerLine * m_WPerChar) )
    {
        p_Y += m_HPerChar;
        p_X -= (m_CharPerLine * m_WPerChar);
    }

    return true;
}
