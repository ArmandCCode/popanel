#ifndef SPSOUNDSMGER_H
#define SPSOUNDSMGER_H

#include <SDL/SDL_mixer.h>

#include <vector>
#include "SPEngine/spsdl/audio/SPAudioEngine.h"

class SPSoundsMger
{
    public:
        SPSoundsMger(SPAudioEngine& p_SoundEngineRef);
        virtual ~SPSoundsMger();

        int LoadSample(std::string p_SamplePath); // Renvoie l'ID dans la liste
        int LoadMusic(std::string p_MusicPath);

        Mix_Chunk* GetSample(int p_SampleID);
        Mix_Music* GetMusic(int p_MusicID);

        void UnloadAllSamples(); // Arr�ter la lecture dans SPAudioEngine avant de d�charger !
        void UnloadAllMusic(); // Arr�ter la lecture dans SPAudioEngine avant de d�charger !

    protected:
    private:
        SPAudioEngine& m_SoundEngineRef; // Permet de faire taire les canaux avant d�chargement !

        std::vector<Mix_Chunk*> m_SampleList;
        std::vector<Mix_Music*> m_MusicList;


};

#endif // SPSOUNDSMGER_H
