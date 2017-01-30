#ifndef LogoState_H
#define LogoState_H

#include "SPEngine/spsdl/SPGameState.h"
#include "StateRequestObject.h"
#include "SPEngine/spsdl/video/SPTextureMger.h"
#include "SPEngine/spsdl/audio/SPSoundsMger.h"
#include "gui/consts/PanelTexLoader.h"
#include "gui/consts/PanelSndLoader.h"
#include "gui/consts/PanelGUIConsts.h"
#include "SPEngine/spsdl/video/SPAsciiBMP.h"
#include "gui/FaderColor.h"
#include "SPEngine/spsdl/video/SPTTFFont.h"
#include "SPEngine/spsdl/video/SPStringBuffer.h"
#include "gui/KeyboardControls.h"

class LogoState : public SPGameState
{
    public:
        LogoState(SPSDL& p_SDLRef, StateRequestObject& p_StateRequest, KeyboardControls& p_ControlsSettings);
        virtual ~LogoState();
        bool Play(long p_Delta);
        bool Frame();
    protected:
    private:
        // State request info
        StateRequestObject& m_stateRequest;

        // Game variables
        KeyboardControls& m_Controls;

        // Texture list
        SPTextureMger m_texList;
        bool LoadResources();
        bool UnloadResources();

        // Credits et lien vers mon site
        SPTTFFont m_LogoFont;
        SPStringBuffer m_GameNameText;
        SPStringBuffer m_DevelopedByText;
        SPStringBuffer m_HyperlinkText;

        // Sounds list
        SPSoundsMger m_sndList;
        SPAsciiBMP m_TextRenderer;

        // Chrono interface
        SPTimer m_TimerUI;

        // Fondu
        FaderColor m_Fader;
};

#endif // LogoState_H
