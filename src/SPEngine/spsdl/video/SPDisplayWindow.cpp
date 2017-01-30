#include "SPEngine/spsdl/video/SPDisplayWindow.h"

SPDisplayWindow::SPDisplayWindow() : m_BGColor(0)
{

    this->m_WindowSurface = NULL;
    this->m_DrawBuffer = NULL;
    this->m_BucketBuffer = NULL;
}

SPDisplayWindow::~SPDisplayWindow()
{

    if (this->isWindowCreated())
    {
        this->DestroyWindow();
    }
}

void SPDisplayWindow::CreateWindow(int p_Width, int p_Height, int p_BitPerPixel, bool p_Fullscreen)
{
    int tmpDispFlags = DISP_FLAGS;
    if (p_Fullscreen)
    {
        tmpDispFlags = tmpDispFlags | SDL_FULLSCREEN;
    }

    this->m_WindowSurface = SDL_SetVideoMode(p_Width, p_Height, p_BitPerPixel,
                                           tmpDispFlags);
    if ( !isWindowCreated() )
    {
        std::cout << "Unable to set resolution : " << SDL_GetError() << std::endl;
        return;
    }

    this->setDrawBuffer(p_Width, p_Height);
    this->setBucketBuffer(p_Width, p_Height);
}

bool SPDisplayWindow::isWindowCreated()
{
    if (this->m_WindowSurface != NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void SPDisplayWindow::DestroyWindow()
{
    // Free bucket buffer
    SDL_FreeSurface(this->m_BucketBuffer);
    this->m_BucketBuffer = NULL;
    // Free drawing buffer
    SDL_FreeSurface(this->m_DrawBuffer);
    this->m_DrawBuffer = NULL;
    // Free drawing window
    SDL_FreeSurface(this->m_WindowSurface);
    this->m_WindowSurface = NULL;
}

void SPDisplayWindow::SetWindowName(std::string p_WinName)
{
    this->m_WindowName = p_WinName;
    SDL_WM_SetCaption(this->m_WindowName.c_str() , NULL);
}

void SPDisplayWindow::setDrawBuffer(int p_Width, int p_Height)
{
    // Pour l'instant, on ne gère pas les changements de résolution à la volée, mais mieux vaut prévenir que guérir !
    if (this->m_DrawBuffer != NULL)
        SDL_FreeSurface(this->m_DrawBuffer);

    // Cette image est fréquemment modifiée, il vaut mieux la stocker en RAM du CPU
    this->m_DrawBuffer = SDL_CreateRGBSurface(SDL_SWSURFACE, p_Width, p_Height, SDL_GetVideoSurface()->format->BitsPerPixel, 0, 0, 0, 0);

}

void SPDisplayWindow::setBucketBuffer(int p_Width, int p_Height)
{
    // Pour l'instant, on ne gère pas les changements de résolution à la volée, mais mieux vaut prévenir que guérir !
    if (this->m_BucketBuffer != NULL)
        SDL_FreeSurface(this->m_BucketBuffer);

    // Cette image est fréquemment modifiée, il vaut mieux la stocker en RAM du CPU
    this->m_BucketBuffer = SDL_CreateRGBSurface(SDL_SWSURFACE, p_Width, p_Height, SDL_GetVideoSurface()->format->BitsPerPixel, 0, 0, 0, 0);

}

void SPDisplayWindow::SetBGColor(int p_red, int p_green, int p_blue)
{
    this->m_BGColor = SDL_MapRGB(this->getWindow()->format, p_red, p_green, p_blue);
}

void SPDisplayWindow::EmptyFrame()
{
    SDL_FillRect(this->getDrawBuffer(), 0, this->m_BGColor );
    SDL_FillRect(this->m_WindowSurface, 0, this->m_BGColor );
}

void SPDisplayWindow::DisplayFrame(int p_BGBlend)
{
    // Gérer la transparence peut prendre du temps, on ne le fait que si demandé
    if (p_BGBlend != SDL_ALPHA_OPAQUE)
    {
        SDL_SetAlpha(this->getDrawBuffer(), SDL_SRCALPHA, p_BGBlend);
    }

    SDL_BlitSurface(m_DrawBuffer, 0, m_WindowSurface, 0);
    SDL_Flip(this->m_WindowSurface);

    if (p_BGBlend != SDL_ALPHA_OPAQUE)
    {
        SDL_SetAlpha(this->getDrawBuffer(), SDL_SRCALPHA, SDL_ALPHA_OPAQUE);
    }

    // Frame limiter
    this->FPSLimiter().FrameDone();
    this->FPSLimiter().RestUntilNextFrame();
}

void SPDisplayWindow::DrawSquare(int p_X, int p_Y, int p_W, int p_H, int p_red, int p_green, int p_blue, int p_alpha)
{
    // on corrige les paramètres : pas de carré plus gros que la fenêtre
    if ( p_W > this->GetWidth() )
    {
        p_W = this->GetWidth();
    }
    if ( p_H > this->GetHeight() )
    {
        p_H = this->GetHeight();
    }
    // Pas de correction des autres paramètres, SDL gère ces cas ci en bouclant sur les valeurs par pas de 256

    // rectangle de dessin du remplissage
    SDL_Rect tmpLocalRect;
    tmpLocalRect.x = 0;
    tmpLocalRect.y  = 0;
    tmpLocalRect.w = p_W;
    tmpLocalRect.h = p_H;


    // Rectangle de dessin sur le buffer de l'écran
    SDL_Rect tmpDrawRect;
    tmpDrawRect.x = p_X;
    tmpDrawRect.y  = p_Y;
    tmpDrawRect.w = p_W;
    tmpDrawRect.h = p_H;

    SDL_FillRect(this->getBucketBuffer(), &tmpLocalRect, SDL_MapRGB(this->getWindow()->format, p_red, p_green, p_blue) );

    if (p_alpha != SDL_ALPHA_OPAQUE)
    {
        SDL_SetAlpha(this->getBucketBuffer(), SDL_SRCALPHA, p_alpha);
    }

    SDL_BlitSurface(this->getBucketBuffer(), &tmpLocalRect, this->getDrawBuffer(), &tmpDrawRect);

    if (p_alpha != SDL_ALPHA_OPAQUE)
    {
        SDL_SetAlpha(this->getBucketBuffer(), SDL_SRCALPHA, SDL_ALPHA_OPAQUE);
    }

}

void SPDisplayWindow::DrawSquareWithOuterBorder(int p_X, int p_Y, int p_W, int p_H, int p_red, int p_green, int p_blue, int p_alpha, int p_BorderW, int p_BorderRed, int p_BorderGreen, int p_BorderBlue, int p_BorderAlpha)
{
    // Dessin du cadre des options
    this->DrawSquare(p_X, p_Y, p_W, p_H, p_red, p_green, p_blue, p_alpha);
    // Bordures
    // haut
    this->DrawSquare(p_X - p_BorderW,
                     p_Y - p_BorderW,
                     p_W + (2*p_BorderW),
                     p_BorderW,
                     p_BorderRed, p_BorderGreen, p_BorderBlue, p_BorderAlpha);
    // gauche
    this->DrawSquare(p_X - p_BorderW,
                     p_Y,
                     p_BorderW,
                     p_H ,
                     p_BorderRed, p_BorderGreen, p_BorderBlue, p_BorderAlpha);
    // bas
    this->DrawSquare(p_X - p_BorderW,
                    (p_Y + p_H),
                     p_W + (2*p_BorderW),
                     p_BorderW,
                     p_BorderRed, p_BorderGreen, p_BorderBlue, p_BorderAlpha);
    // droite
    this->DrawSquare(p_X + p_W,
                     p_Y,
                     p_BorderW,
                     p_H,
                     p_BorderRed, p_BorderGreen, p_BorderBlue, p_BorderAlpha);

}


void SPDisplayWindow::DrawSquareWithInnerBorder(int p_X, int p_Y, int p_W, int p_H, int p_red, int p_green, int p_blue, int p_alpha, int p_BorderW, int p_BorderRed, int p_BorderGreen, int p_BorderBlue, int p_BorderAlpha)
{
    // Dessin du cadre des options
    this->DrawSquare(p_X, p_Y, p_W, p_H, p_red, p_green, p_blue, p_alpha);
    // Bordures
    // haut
    this->DrawSquare(p_X,
                     p_Y,
                     p_W,
                     p_BorderW,
                     p_BorderRed, p_BorderGreen, p_BorderBlue, p_BorderAlpha);
    // gauche
    this->DrawSquare(p_X,
                     p_Y + p_BorderW,
                     p_BorderW,
                     p_H - (2*p_BorderW),
                     p_BorderRed, p_BorderGreen, p_BorderBlue, p_BorderAlpha);
    // bas
    this->DrawSquare(p_X,
                    (p_Y + p_H - p_BorderW),
                     p_W,
                     p_BorderW,
                     p_BorderRed, p_BorderGreen, p_BorderBlue, p_BorderAlpha);
    // droite
    this->DrawSquare(p_X + p_W - p_BorderW,
                     p_Y + p_BorderW,
                     p_BorderW,
                     p_H - (2*p_BorderW),
                     p_BorderRed, p_BorderGreen, p_BorderBlue, p_BorderAlpha);

}

