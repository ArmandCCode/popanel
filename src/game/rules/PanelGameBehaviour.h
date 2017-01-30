#ifndef PANELGAMEBEHAVIOUR_H
#define PANELGAMEBEHAVIOUR_H

#include <vector>
#include "game/actors/Player.h"
#include "gui/GameEvents.h"
#include "game/containers/MatchSettings.h"
#include <cmath>

class PanelGameBehaviour
{
    public:
        PanelGameBehaviour();
        virtual ~PanelGameBehaviour();

        // Player initialization
        void InitPlayer(Player& p_Player, MatchSettings& p_MatchSettings);

        // Controlling the game from outside
        // Cursor moving
        bool RequestCursorMoveUp(Player& p_Player);
        bool RequestCursorMoveDown(Player& p_Player);
        bool RequestCursorMoveLeft(Player& p_Player);
        bool RequestCursorMoveRight(Player& p_Player);

        // Tile swapping
        bool RequestSwapping(Player& p_Player, GameEvents& p_GameEvents);

        // Field raising
        bool RequestFieldRise(Player& p_Player);
        bool RequestFieldRiseStop(Player& p_Player);

        // Game logic update
        void UpdateGameLogic(Player& p_Player, unsigned long p_TimePassedMs, GameEvents& p_GameEvents, MatchSettings& p_MatchSettings);

    protected:
    private:

        void UpdateBlocks(Player& p_Player, GameEvents& p_GameEvents);
        void CheckMatches(Player& p_Player, GameEvents& p_GameEvents);
        bool IsABlockMatchableAt(Player& p_Player, int p_X, int p_Y, int p_SearchedColour);

        void UpdateField(Player& p_Player, MatchSettings& p_MatchSettings);
        void UpdatePlayerStatus(Player& p_Player);

        // Accesseurs rapides
        int GetNumColors(Player& p_Player);
        int GetBlockSwitchTime(Player& p_Player);
        int GetBlockGoingToFallTime(Player& p_Player);
        int GetBlockFallTime(Player& p_Player);
        int GetBlockPopTime(Player& p_Player);



};

#endif // PANELGAMEBEHAVIOUR_H
