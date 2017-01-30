#ifndef PANELSNDLOADER_H
#define PANELSNDLOADER_H

#include "SPEngine/spsdl/audio/SPSoundsMger.h"

namespace MainSnd
{
    static const int SPL_VALIDATE = 0;

    inline void LoadMainSnd(SPSoundsMger& p_SndMger)
    {
        p_SndMger.LoadSample("./snd/validate.wav"); // Son 0
    }
};

namespace LogoSnd
{
    static const int SPL_VALIDATE = 0;

    inline void LoadLogoSnd(SPSoundsMger& p_SndMger)
    {
        p_SndMger.LoadSample("./snd/validate.wav"); // Son 0
    }
};

namespace TitleSnd
{
    static const int SPL_VALIDATE = 0;
    static const int SPL_CURSORMOVE = 1;

    static const int MUS_MENU = 0;

    inline void LoadTitleSnd(SPSoundsMger& p_SndMger)
    {
        p_SndMger.LoadSample("./snd/validate.wav"); // Son 0
        p_SndMger.LoadSample("./snd/cursormove.wav"); // Son 1

        p_SndMger.LoadMusic("./snd/menu.ogg"); // Musique 0
    }
};

namespace GameSnd
{
    static const int SPL_VALIDATE = 0;
    static const int SPL_COUNTDOWN = 1;
    static const int SPL_COUNTEND = 2;
    static const int SPL_CURSORMOVE = 3;
    static const int SPL_PANELMOVE = 4;
    static const int SPL_PANELBUMP = 5;
    static const int SPL_PANELPOP = 6;
    static const int SPL_COMBOCHAIN = 7;
    static const int SPL_STOP = 8;
    static const int SPL_GAMEOVERLOSE = 9;

    static const int MUS_MAIN = 0;
    static const int MUS_PANIC = 1;
    static const int MUS_VICTORY = 2;
    static const int MUS_DEFEAT = 3;

    inline void LoadGameSnd(SPSoundsMger& p_SndMger)
    {
        // Samples
        p_SndMger.LoadSample("./snd/validate.wav"); // Son 0
        p_SndMger.LoadSample("./snd/countdown.wav"); // Son 1
        p_SndMger.LoadSample("./snd/countend.wav"); // Son 2
        p_SndMger.LoadSample("./snd/cursormove.wav"); // Son 3
        p_SndMger.LoadSample("./snd/panelmove.wav"); // Son 4
        p_SndMger.LoadSample("./snd/panelbump.wav"); // Son 5
        p_SndMger.LoadSample("./snd/panelpop.wav"); // Son 6
        p_SndMger.LoadSample("./snd/combochain.wav"); // Son 7
        p_SndMger.LoadSample("./snd/stop.wav"); // Son 8
        p_SndMger.LoadSample("./snd/gameoverlose.wav"); // Son 9

        // Music
        p_SndMger.LoadMusic("./snd/main.ogg"); // Musique 0
        p_SndMger.LoadMusic("./snd/panic.ogg"); // Musique 1
        p_SndMger.LoadMusic("./snd/menu.ogg"); // Musique 2
        p_SndMger.LoadMusic("./snd/menu.ogg"); // Musique 3

    }
};

#endif // PANELSNDLOADER_H
