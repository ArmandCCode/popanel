#ifndef PANELTEXLOADER_H
#define PANELTEXLOADER_H

#include "SPEngine/spsdl/video/SPTextureMger.h"

namespace LogoTex
{
    inline void LoadLogoTex(SPTextureMger& p_TexMger)
    {

    }
};

namespace TitleTex
{
    static const int TX_LOGO = 0;
    static const int TX_BACKGROUNDSCROLL = 1;

    inline void LoadTitleTex(SPTextureMger& p_TexMger)
    {
        p_TexMger.AttachAndLoadTexture(TX_LOGO, "./gfx/title/title.png");
        p_TexMger.AttachAndLoadTexture(TX_BACKGROUNDSCROLL, "./gfx/common/bgscroll.png");
    }
};

namespace GameTex
{
    static const int TX_PANEL1 = 0;
    static const int TX_PANEL2 = 1;
    static const int TX_PANEL3 = 2;
    static const int TX_PANEL4 = 3;
    static const int TX_PANEL5 = 4;
    static const int TX_PANEL6 = 5;
    static const int TX_CURSORFIT = 6;
    static const int TX_CURSORLOOSE = 7;

    static const int TX_COMBO4 = 8;
    static const int TX_COMBO5 = 9;
    static const int TX_COMBO6 = 10;
    static const int TX_COMBO7 = 11;
    static const int TX_COMBO8 = 12;
    static const int TX_COMBO9 = 13;
    static const int TX_COMBO10 = 14;
    static const int TX_COMBO11 = 15;
    static const int TX_COMBO12 = 16;
    static const int TX_COMBOMORE = 17;

    static const int TX_CHAIN2 = 18;
    static const int TX_CHAIN3 = 19;
    static const int TX_CHAIN4 = 20;
    static const int TX_CHAIN5 = 21;
    static const int TX_CHAIN6 = 22;
    static const int TX_CHAIN7 = 23;
    static const int TX_CHAIN8 = 24;
    static const int TX_CHAIN9 = 25;
    static const int TX_CHAIN10 = 26;
    static const int TX_CHAIN11 = 27;
    static const int TX_CHAIN12 = 28;
    static const int TX_CHAINMORE = 29;

    // Background gfx
    static const int TX_BACKGROUNDSCROLL = 30;
    static const int TX_FOREGROUND1P = 31;
    static const int TX_FOREGROUND2P = 32;


    inline void LoadGameTex(SPTextureMger& p_TexMger)
    {
        // Block gfx
        p_TexMger.AttachAndLoadTexture(TX_PANEL1, "./gfx/game/panels/panel1.png");
        p_TexMger.AttachAndLoadTexture(TX_PANEL2, "./gfx/game/panels/panel2.png");
        p_TexMger.AttachAndLoadTexture(TX_PANEL3, "./gfx/game/panels/panel3.png");
        p_TexMger.AttachAndLoadTexture(TX_PANEL4, "./gfx/game/panels/panel4.png");
        p_TexMger.AttachAndLoadTexture(TX_PANEL5, "./gfx/game/panels/panel5.png");
        p_TexMger.AttachAndLoadTexture(TX_PANEL6, "./gfx/game/panels/panel6.png");

        // Cursor gfx
        p_TexMger.AttachAndLoadTexture(TX_CURSORFIT, "./gfx/game/cursorfit.png");
        p_TexMger.AttachAndLoadTexture(TX_CURSORLOOSE, "./gfx/game/cursorloose.png");

        // Combo gfx
        p_TexMger.AttachAndLoadTexture(TX_COMBO4, "./gfx/game/combo/combo4.png");
        p_TexMger.AttachAndLoadTexture(TX_COMBO5, "./gfx/game/combo/combo5.png");
        p_TexMger.AttachAndLoadTexture(TX_COMBO6, "./gfx/game/combo/combo6.png");
        p_TexMger.AttachAndLoadTexture(TX_COMBO7, "./gfx/game/combo/combo7.png");
        p_TexMger.AttachAndLoadTexture(TX_COMBO8, "./gfx/game/combo/combo8.png");
        p_TexMger.AttachAndLoadTexture(TX_COMBO9, "./gfx/game/combo/combo9.png");
        p_TexMger.AttachAndLoadTexture(TX_COMBO10, "./gfx/game/combo/combo10.png");
        p_TexMger.AttachAndLoadTexture(TX_COMBO11, "./gfx/game/combo/combo11.png");
        p_TexMger.AttachAndLoadTexture(TX_COMBO12, "./gfx/game/combo/combo12.png");
        p_TexMger.AttachAndLoadTexture(TX_COMBOMORE, "./gfx/game/combo/combomore.png");

        // Chain gfx
        p_TexMger.AttachAndLoadTexture(TX_CHAIN2, "./gfx/game/chain/chain2.png");
        p_TexMger.AttachAndLoadTexture(TX_CHAIN3, "./gfx/game/chain/chain3.png");
        p_TexMger.AttachAndLoadTexture(TX_CHAIN4, "./gfx/game/chain/chain4.png");
        p_TexMger.AttachAndLoadTexture(TX_CHAIN5, "./gfx/game/chain/chain5.png");
        p_TexMger.AttachAndLoadTexture(TX_CHAIN6, "./gfx/game/chain/chain6.png");
        p_TexMger.AttachAndLoadTexture(TX_CHAIN7, "./gfx/game/chain/chain7.png");
        p_TexMger.AttachAndLoadTexture(TX_CHAIN8, "./gfx/game/chain/chain8.png");
        p_TexMger.AttachAndLoadTexture(TX_CHAIN9, "./gfx/game/chain/chain9.png");
        p_TexMger.AttachAndLoadTexture(TX_CHAIN10, "./gfx/game/chain/chain10.png");
        p_TexMger.AttachAndLoadTexture(TX_CHAIN11, "./gfx/game/chain/chain11.png");
        p_TexMger.AttachAndLoadTexture(TX_CHAIN12, "./gfx/game/chain/chain12.png");
        p_TexMger.AttachAndLoadTexture(TX_CHAINMORE, "./gfx/game/chain/chainmore.png");

        p_TexMger.AttachAndLoadTexture(TX_BACKGROUNDSCROLL, "./gfx/common/bgscroll.png");
        p_TexMger.AttachAndLoadTexture(TX_FOREGROUND1P, "./gfx/game/foreground1P.png");
        p_TexMger.AttachAndLoadTexture(TX_FOREGROUND2P, "./gfx/game/foreground2P.png");

    }
};

#endif // PANELTEXLOADER_H
