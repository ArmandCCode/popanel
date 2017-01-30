#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <string>
#include "SPEngine/SPException.h"
#include "SPEngine/spsdl/video/SPDisplayWindow.h"

class SPTexture
{
    public:
        // constructors, destructors
        SPTexture();
        SPTexture(std::string p_FileName);
        virtual ~SPTexture();

        // get/set
        SDL_Surface* getSurface() { return m_Surface; } // fonction amie de SPDisplayWindow ?
        SDL_Rect getRect() { return m_Rect; } // fonction amie de SPDisplayWindow ?
        SDL_Rect getLocalRect() { return m_LocalRect; } // fonction amie de SPDisplayWindow ?
        long getH() { return m_Surface->h; }
        long getW() { return m_Surface->w; }

        // Functions
        void CreateEmptySurface();
        void CreateEmptySurface(long p_xSize, long pYsize);
        void LoadSurface(std::string p_FileName);
        void SetLocalRect(long p_XStart, long p_YStart, long p_XEnd, long p_YEnd);
        void ResetLocalRect(); // Remet la taille maximale du Rectangle de rendu
        void setTransparentColor(long p_red, long p_green, long p_blue); // Pour indiquer la couleur à ignorer pour les images sans transparence (SDL Color Key)

        static const int RENDER_FROMTOPLEFT = 0;
        static const int RENDER_FROMCENTER = 1;
        static const int RENDER_FROMTOPRIGHT = 2;

        void RenderToWindow(SPDisplayWindow& p_window, long p_X, long p_Y, int p_Alignment = RENDER_FROMTOPLEFT);


    protected:
        // Valide pour les héritiers
        bool isSurfaceLoaded();
        void freeSurface();

        // internal functions
        void setSurface(SDL_Surface* val) { m_Surface = val; ResetLocalRect(); }
        void setRect(SDL_Rect val) { m_Rect = val; }
    private:
        // members
        SDL_Surface* m_Surface;
        SDL_Rect m_Rect;
        SDL_Rect m_LocalRect;

};

#endif // TEXTURE_H
