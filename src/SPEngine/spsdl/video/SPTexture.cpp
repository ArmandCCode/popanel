#include "SPEngine/spsdl/video/SPTexture.h"

SPTexture::SPTexture()
{
    this->m_Surface = NULL;
    // set initial coordinates at 0, 0
    m_Rect.x = 0;
    m_Rect.y = 0;
}

SPTexture::SPTexture(std::string p_FileName)
{

    // load filename
    this->m_Surface = NULL;
    this->LoadSurface(p_FileName);
    // set initial coordinates at 0, 0
    m_Rect.x = 0;
    m_Rect.y = 0;
    this->ResetLocalRect();
}

SPTexture::~SPTexture()
{

    this->freeSurface();
}

void SPTexture::CreateEmptySurface()
{
    this->freeSurface();

    // Create new SDL_Surface with the correct BPP
    this->m_Surface = SDL_CreateRGBSurface(SDL_SWSURFACE, 16, 16, SDL_GetVideoSurface()->format->BitsPerPixel, 0, 0, 0, 0);
    this->ResetLocalRect();
}

void SPTexture::CreateEmptySurface(long p_xSize, long p_ySize)
{
    this->freeSurface();

    // Create new SDL_Surface with the correct BPP
    this->m_Surface = SDL_CreateRGBSurface(SDL_SWSURFACE, p_xSize, p_ySize, SDL_GetVideoSurface()->format->BitsPerPixel, 0, 0, 0, 0);
    this->ResetLocalRect();
}

void SPTexture::LoadSurface(std::string p_FileName)
{
    this->freeSurface();

    // Load SDL_Surface
    // old // this->setSurface(IMG_Load(p_FileName.c_str()));
    SDL_Surface* tmpConv = NULL;
    tmpConv = IMG_Load(p_FileName.c_str());

    if ( tmpConv == NULL )
    {
        std::string sMess("SPTexture::LoadSurface() : Unable to load specified filename \"" + p_FileName + "\".\n");
        throw SPException(sMess);
    }

    // Avant de convertir, il faut tester que la surface a été chargée
    this->m_Surface = SDL_DisplayFormatAlpha(tmpConv);

    if ( !this->isSurfaceLoaded() )
    {
        std::string sMess("SPTexture::LoadSurface() : Unable to convert surface \"" + p_FileName + "\".\n");
        throw SPException(sMess);
    }

    SDL_FreeSurface(tmpConv);

    this->ResetLocalRect();
}


void SPTexture::setTransparentColor(long p_red, long p_green, long p_blue)
{
    if (!this->isSurfaceLoaded())
    {
        throw SPException("SPTexture::setTransparentColor() : Surface non chargée");
    }

    SDL_SetColorKey(this->getSurface(), SDL_SRCCOLORKEY, SDL_MapRGB(this->getSurface()->format, p_red, p_green, p_blue));
}

bool SPTexture::isSurfaceLoaded()
{
    // if m_Surface pointer == NULL, nothing is loaded
    if (this->m_Surface == NULL)
        return false;

    return true;
}

void SPTexture::freeSurface()
{
    if (this->isSurfaceLoaded())
    {
        SDL_FreeSurface(this->getSurface());
    }
}

void SPTexture::SetLocalRect(long p_XStart, long p_YStart, long p_XEnd, long p_YEnd)
{
    if (p_XStart < 0)
        p_XStart = 0;
    if (p_YStart < 0)
        p_YStart = 0;
    if (p_XEnd > this->m_Surface->w || p_XEnd < 0)
        p_XEnd = this->m_Surface->w;
    if (p_YEnd > this->m_Surface->h || p_YEnd < 0)
        p_YEnd = this->m_Surface->h;

    this->m_LocalRect.x = p_XStart;
    this->m_LocalRect.y = p_YStart;
    this->m_LocalRect.w = p_XEnd - p_XStart;
    this->m_LocalRect.h = p_YEnd - p_YStart;
}

void SPTexture::ResetLocalRect() // Remet la taille maximale du Rectangle de rendu
{
    if ( this->isSurfaceLoaded() )
    {
        this->m_LocalRect.x = 0;
        this->m_LocalRect.y = 0;
        this->m_LocalRect.w = this->m_Surface->w;
        this->m_LocalRect.h = this->m_Surface->h;
    }
    else
    {
        this->m_LocalRect.x = 0;
        this->m_LocalRect.y = 0;
        this->m_LocalRect.w = 0;
        this->m_LocalRect.h = 0;
    }
}

void SPTexture::RenderToWindow(SPDisplayWindow& p_window, long p_X, long p_Y, int p_Alignment)
{
    switch (p_Alignment)
    {
        case RENDER_FROMCENTER:
            m_Rect.x = p_X - (this->getSurface()->w / 2);
            m_Rect.y = p_Y - (this->getSurface()->h / 2);
        break;
        case RENDER_FROMTOPRIGHT:
            m_Rect.x = p_X - this->getSurface()->w;
            m_Rect.y = p_Y;
        break;
        default:
            m_Rect.x = p_X;
            m_Rect.y = p_Y;
        break;
    }

    int iResult = 0;

    iResult =  SDL_BlitSurface(this->getSurface(), &m_LocalRect, p_window.getDrawBuffer(), &m_Rect);

    if (iResult != 0)
    {
        std::cout << SDL_GetError() << std::endl;
    }
}
