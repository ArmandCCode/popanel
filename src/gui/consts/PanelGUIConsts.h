#ifndef PANELGUICONST_H
#define PANELGUICONST_H

namespace C_TextureSizes
{
    static const int PANELW = 32;
    static const int PANELH = 32;
};

namespace C_FontProps
{
    static const int XSMALLFONTHEIGHT = 16;
    static const int SMALLFONTHEIGHT = 24;
    static const int MEDIUMFONTHEIGHT = 32;
    static const int BIGFONTHEIGHT = 64;

    static const int TITLEFONTR = 255;
    static const int TITLEFONTG = 255;
    static const int TITLEFONTB = 255;

    static const int VALUEFONTR = 255;
    static const int VALUEFONTG = 255;
    static const int VALUEFONTB = 255;

    static const int MENUELTFONTR = 255;
    static const int MENUELTFONTG = 255;
    static const int MENUELTFONTB = 255;

};

namespace C_FadingTimers
{
    static const int FADEIN = 500;
    static const int FADEOUT = 1000;
};

namespace C_GameStateEventTimers
{
    static const unsigned long INTRO = 1000;
    static const unsigned long FINISHED = 1200;
    static const unsigned long RESULTSROLL = 2000;
    static const unsigned long OUTRO = 1000;
};

#endif // PANELGUICONST_H
