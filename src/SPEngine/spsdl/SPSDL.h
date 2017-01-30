#ifndef SPSDL_H
#define SPSDL_H

#include <string>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

#include "SPEngine/SPException.h"
#include "video/SPDisplayWindow.h"
#include "audio/SPAudioEngine.h"

class SPSDL
{
    public:
        SPSDL();
        virtual ~SPSDL();

        // Démarrage de tous les modules de la SDL
        void StartEngine();
        void StopEngine();

        // Création d'une fenêtre
        void CreateWindow(int p_Width, int p_Height, int p_BitPerPixel, bool p_Fullscreen = false, std::string p_WinName = "");
        void DestroyWindow();

        // Création d'un buffer audio
        void CreateAudioOutput(int p_SampleFreq, int p_SampleSize);
        void DestroyAudioOutput();

        SPDisplayWindow& window() { return m_Window; }
        SPAudioEngine& audio() { return m_AudioEngine; }
    protected:
    private:
        SPDisplayWindow m_Window;
        SPAudioEngine m_AudioEngine;
};

#endif // SPSDL_H
