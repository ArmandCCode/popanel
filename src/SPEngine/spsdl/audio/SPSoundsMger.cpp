#include "SPEngine/spsdl/audio/SPSoundsMger.h"

SPSoundsMger::SPSoundsMger(SPAudioEngine& p_SoundEngineRef) : m_SoundEngineRef(p_SoundEngineRef)
{

}

SPSoundsMger::~SPSoundsMger()
{

    this->UnloadAllSamples();
    this->UnloadAllMusic();
}

int SPSoundsMger::LoadSample(std::string p_SamplePath) // Renvoie l'ID dans la liste
{
    Mix_Chunk* tmpChunk;
    tmpChunk = Mix_LoadWAV( p_SamplePath.c_str() );

    this->m_SampleList.push_back(tmpChunk);

    return (this->m_SampleList.size() - 1);
}

int SPSoundsMger::LoadMusic(std::string p_MusicPath)
{
    Mix_Music* tmpMusic;
    tmpMusic = Mix_LoadMUS( p_MusicPath.c_str() );

    this->m_MusicList.push_back(tmpMusic);

    return (this->m_MusicList.size() - 1);
}

Mix_Chunk* SPSoundsMger::GetSample(int p_SampleID)
{
    return this->m_SampleList[p_SampleID];
}

Mix_Music* SPSoundsMger::GetMusic(int p_MusicID)
{
    return this->m_MusicList[p_MusicID];
}

void SPSoundsMger::UnloadAllSamples()
{
    // Arrêter la lecture dans SPAudioEngine avant de décharger !
    this->m_SoundEngineRef.StopAllSamples();
    // D'abord décharger les samples
    for (unsigned int i = 0; i < m_SampleList.size(); i++)
    {
        Mix_FreeChunk( m_SampleList[i] );
    }
    // Puis supprimer les pointeurs
    this->m_SampleList.clear();
}

void SPSoundsMger::UnloadAllMusic() // Arrêter la lecture dans SPAudioEngine avant de décharger !
{
    // Arrêter la lecture dans SPAudioEngine avant de décharger !
    this->m_SoundEngineRef.StopCurrentMusic();
    this->m_SoundEngineRef.SetMusic(NULL);
    // D'abord décharger les musiques
    for (unsigned int i = 0; i < m_MusicList.size(); i++)
    {
        Mix_FreeMusic( m_MusicList[i] );
    }
    // Puis supprimer les pointeurs
    this->m_MusicList.clear();
}
