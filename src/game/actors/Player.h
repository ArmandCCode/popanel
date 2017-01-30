#ifndef Player_H
#define Player_H

#include "Cursor.h"
#include "Field.h"
#include "PopBlockSet.h"
#include "timers/ManualTimer.h"
#include "game/consts/PanelGameConsts.h"

class Player
{
    public:
        Player();
        virtual ~Player();

        Cursor& GetCursor() { return m_Cursor; }
        void SetCursor(Cursor val) { m_Cursor = val; }

        Field& GetField() { return m_Field; }
        void SetField(Field val) { m_Field = val; }

        unsigned long GetIDinBlockList() { return m_IDinBlockList; }
        void SetIDinBlockList(unsigned long val) { m_IDinBlockList = val; }

        unsigned int GetDifficulty() { return m_Difficulty; }
        void SetDifficulty(unsigned int val) { m_Difficulty = val; }

        ManualTimer& GetGameTimer() { return m_GameTimer; }

        // Game State management
        int GetStatus() { return m_GameStatus; }
        void SetStatus(unsigned int val) { m_GameStatus = val; }
        static const int PLAYER_STAT_PLAYING = 0;
        static const int PLAYER_STAT_DEFEAT = 1;
        static const int PLAYER_STAT_VICTORY = 2;
        static const int PLAYER_STAT_ENDTIMER = 3;

        // Score
        long GetScore() { return m_Score; }
        void AwardPoints(long p_PointsAwarded);

        std::vector<PopBlockSet>& GetDisappearingBlocks() { return m_DisappearingBlocks; }
        std::vector<int>& GetChainList() { return m_ChainList; } // l'accesseur est l'ID, la valeur dans le tableau est le multiplicateur !
        std::vector<int>& GetStoppedBlocksID() { return m_StoppedBlocksID; } // Les blocs qui sont devenus fixes cette frame pour ce joueur

        // Indique si le joueur souhaite que la pile monte en accéléré
        bool m_FastRiseRequested;
        void SetFastRiseRequest(bool p_value) { m_FastRiseRequested = p_value; }
        bool GetFastRiseRequest() { return m_FastRiseRequested; }

        // Panic detection
        bool isInPanicMode();

    protected:
    private:
        Cursor m_Cursor;
        Field m_Field;

        unsigned long m_IDinBlockList;

        // Temps de jeu
        ManualTimer m_GameTimer;

        // Propriétés du jeu du joueur
        unsigned int m_Difficulty;
        unsigned int m_GameStatus;

        long m_Score;

        std::vector<PopBlockSet> m_DisappearingBlocks;
        std::vector<int> m_ChainList; // l'accesseur est l'ID, la valeur dans le tableau est le multiplicateur !
        std::vector<int> m_StoppedBlocksID; // Les blocs qui sont devenus fixes cette frame pour ce joueur
};

#endif // Player_H
