#include "SPEngine/spsdl/audio/SPAudioEngine.h"

SPAudioEngine::SPAudioEngine() : m_OutputIsOpened(false)
{

}

SPAudioEngine::~SPAudioEngine()
{

}

void SPAudioEngine::CreateAudioOutput(int p_SampleFreq, int p_SampleSize)
{
    // Est-ce que SDL_mixer peut ouvrir une sortie son ?
    if( Mix_OpenAudio(p_SampleFreq, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, p_SampleSize) == - 1)
    {
        std::string sMess("SDL_mixer : Unable to initialize audio output. ");
        sMess = sMess + SDL_GetError();
        SPException e( sMess );
        throw e;
    }

    m_OutputIsOpened = true;
}

bool SPAudioEngine::isAudioOutputOpen()
{
    return m_OutputIsOpened;
}

void SPAudioEngine::DestroyAudioOutput()
{
    Mix_CloseAudio();
    m_OutputIsOpened = false;
}

void SPAudioEngine::PlaySample(Mix_Chunk* p_Sample, int p_ChannelNum)
{
    Mix_PlayChannel(p_ChannelNum, p_Sample, 0);
}

void SPAudioEngine::StopAllSamples() // Arrêter la lecture des samples pour pouvoir les décharger sans risque !
{
    Mix_HaltChannel(-1);
}

void SPAudioEngine::SetMusic(Mix_Music* p_Music, bool p_bAlwaysLoop) // pour enlever le pointeur, passer NULL
{
    if (this->m_CurrentMusic != NULL)
    {
        this->StopCurrentMusic();
    }
    this->m_CurrentMusic = p_Music;
    this->m_bLoopSong = p_bAlwaysLoop;
}

void SPAudioEngine::PlayCurrentMusic() // Ne démarre la musique que si le pointeur n'est pas NULL
{
    if (this->m_CurrentMusic != NULL)
    {
        if (this->m_bLoopSong)
        {
            Mix_PlayMusic(this->m_CurrentMusic, -1);
        }
        else
        {
            Mix_PlayMusic(this->m_CurrentMusic, 0);
        }
    }

}

void SPAudioEngine::PauseCurrentMusic() // Idem
{
    if (this->m_CurrentMusic != NULL)
    {
        Mix_PauseMusic();
    }
}

void SPAudioEngine::ResumeCurrentMusic()
{
    if (this->m_CurrentMusic != NULL)
    {
        Mix_ResumeMusic();
    }
}

void SPAudioEngine::StopCurrentMusic()
{
    if (this->m_CurrentMusic != NULL)
    {
        Mix_HaltMusic();
    }
}

bool SPAudioEngine::IsMusicPlaying()
{
    return ( Mix_PlayingMusic() == 1 );
}
