#ifndef DISPLAYWINDOW_H
#define DISPLAYWINDOW_H

#ifdef GP2X
    #define DISP_FLAGS SDL_SWSURFACE|SDL_HWACCEL|SDL_FULLSCREEN
#else
    #define DISP_FLAGS SDL_HWSURFACE|SDL_HWACCEL|SDL_DOUBLEBUF
#endif

#include <iostream>
#include <string>

#include <SDL/SDL.h>

#include "SPFrameLimiter.h"
#include "SPEngine/sputil/SPConvertTools.h"

class SPDisplayWindow
{
    public:
        // constructors, destructors
        SPDisplayWindow();
        virtual ~SPDisplayWindow();

        // Window initialization
        void CreateWindow(int p_Width, int p_Height, int p_BitPerPixel, bool p_Fullscreen = false);
        bool isWindowCreated();
        void SetWindowName(std::string p_WinName);
        void DestroyWindow();
        int GetHeight() { return m_WindowSurface->h; }
        int GetWidth() { return m_WindowSurface->w; }

        // Drawing
        // functions
        void SetBGColor(int p_red, int p_green, int p_blue);
        void EmptyFrame(); // Remplit l'image de la couleur d'arrière plan paramétrée dans m_BGColor

        //void DrawTexture(SPTexture& p_TexToDraw);
        SDL_Surface* getDrawBuffer() { return m_DrawBuffer; }

        void DrawSquare(int p_X, int p_Y, int p_W, int p_H, int p_red, int p_green, int p_blue, int p_alpha);
        void DrawSquareWithOuterBorder(int p_X, int p_Y, int p_W, int p_H, int p_red, int p_green, int p_blue, int p_alpha, int p_BorderW, int p_BorderRed, int p_BorderGreen, int p_BorderBlue, int p_BorderAlpha);
        void DrawSquareWithInnerBorder(int p_X, int p_Y, int p_W, int p_H, int p_red, int p_green, int p_blue, int p_alpha, int p_BorderW, int p_BorderRed, int p_BorderGreen, int p_BorderBlue, int p_BorderAlpha);
        void DisplayFrame(int p_BGBlend = SDL_ALPHA_OPAQUE); // 0 = couleur d'arrière plan, 255 = opaque

        SPFrameLimiter& FPSLimiter() { return m_FrameLimit; }

    protected:
    private:
        // Window initialization
        std::string m_WindowName;

        // Drawing
        // members
        SDL_Surface* m_WindowSurface; // Je ne peux pas la faire passer pour une SPTexture standard, elle a des propriétés particulières
        Uint32 m_BGColor; // Couleur d'effacement de la fenêtre // Note : Utiliser SDL_Color demanderait un appel à MapRGB sur chaque Frame
        SDL_Surface* m_DrawBuffer; // accès interne : sert de mémoire tampon au dessin final
        SDL_Surface* m_BucketBuffer; // accès interne : sert à effectuer des dessins de carrés, avec des couleurs, et en transparence

        // internal functions
        void setDrawBuffer(int p_Width, int p_Height);
        void setBucketBuffer(int p_Width, int p_Height);
        SDL_Surface* getBucketBuffer() { return m_BucketBuffer; }
        SDL_Surface* getWindow() { return m_WindowSurface; }

        // Frame limiter
        SPFrameLimiter m_FrameLimit;
};

#endif // DISPLAYWINDOW_H
