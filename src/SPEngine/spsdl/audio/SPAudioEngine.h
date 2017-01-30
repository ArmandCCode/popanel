#ifndef SPAUDIOENGINE_H
#define SPAUDIOENGINE_H

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include <string>
#include <vector>
#include "SPEngine/SPException.h"

class SPAudioEngine
{
    public:
        SPAudioEngine();
        virtual ~SPAudioEngine();

        // Création et destruction de la sortie sonore
        void CreateAudioOutput(int p_SampleFreq, int p_SampleSize);
        bool isAudioOutputOpen();
        void DestroyAudioOutput();

        // Lecture de sons
        void PlaySample(Mix_Chunk* p_Sample, int p_iChannelNum = -1);
        void StopAllSamples(); // Arrêter la lecture des samples pour pouvoir les décharger !

        void SetMusic(Mix_Music* p_Music, bool p_bAlwaysLoop = true); // pour enlever le pointeur, passer NULL
        void PlayCurrentMusic(); // Ne démarre la musique que si le pointeur n'est pas NULL
        void PauseCurrentMusic(); // Idem
        void ResumeCurrentMusic(); // Après un appel de PauseCurrentMusic
        void StopCurrentMusic();
        bool IsMusicPlaying();

    protected:
    private:
        bool m_OutputIsOpened;
        // Il faut retenir la musique qui est en train d'être jouée, pour pouvoir l'arrêter si on en joue une autre
        Mix_Music* m_CurrentMusic;
        bool m_bLoopSong;

};

#endif // SPAUDIOENGINE_H
