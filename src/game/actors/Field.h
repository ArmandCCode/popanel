#ifndef FIELD_H
#define FIELD_H

#include <vector>
#include "Block.h"
#include "Cursor.h"
#include "SPEngine/SPException.h"
#include "SPEngine/sputil/SPNumHelper.h"
#include "game/consts/PanelGameConsts.h"

class Field
{
    public:
        Field();
        virtual ~Field();

        // Accès aux blocs
        bool isBlockAt(int p_X, int p_Y);
        Block& GetBlock(int p_X, int p_Y);
        Block& GetBlock(int p_ID);
        int GetBlockIDAt(int p_X, int p_Y);
        // Suppression d'un bloc
        bool DeleteBlock(unsigned int p_ID);

        int GetNbBlocks() { return m_FieldBlocks.size(); } // A utiliser conjointement avec GetBlock(BlockID)

        // Field generation
        void EmptyField();
        // void CreateNewRandomField(int p_NbColors, int p_WantedHeight = 6);
        void LoadField(std::vector<int> p_Blocks);

        // Block swapping
        bool IsSwappingAllowed(int p_X1, int p_Y1, bool& p_Block1Move, int p_X2, int p_Y2, bool& p_Block2Move);

        // Timers update
        void UpdateTimers(unsigned long p_TimePassedMs);

        // Vérification des interactions des blocs
        bool IsABlockInteracting();
        bool IsABlockGoingToDest(int p_X, int p_Y);
        bool IsABlockPopping();

        // New Line insertion
        void SetNewNextLine(std::vector<int> p_NextLineBlocks);
        void InsertNextLineInField();

        ManualTimer& GetNewLineTimer() { return m_NewLineTimer; }
        Block& GetNewLineBlock(int p_xCoord);

        // Temps de résistance de la pile avant Game over
        ManualTimer& GetFieldResistanceTimer() { return m_FieldResistanceTimer; }

        void EnableFastRise(bool p_Fast); // Activer immédiatement, mais désactiver à la prochaine ligne
        void StopFastRise(); // Désactiver immédiatement
        bool IsFastRiseEnabled() { return m_FastRise; }
        void ResetNewLineTimer();

        // Vitesse de montée de la pile
        void SetSpeed(int p_Speed);
        int GetSpeed();
        void RiseSpeed(int p_Increments = 1);

        ManualTimer& GetStopTimer() { return m_StopTimer; }
        void AddStopTime(int p_StopSeconds);
        void ResetStopTime(); // En cas d'annulation causée par un appui sur "SHIFT"
        bool IsStopTimeInEffect();

    protected:
    private:

        std::vector<Block> m_FieldBlocks;
        std::vector<Block> m_NextLine;

        // BlockSwapping (internal)
        bool IsMoveAllowed(int p_X1, int p_Y1, int p_X2, int p_Y2);

        // Timer de pile (montée des blocs)
        ManualTimer m_NewLineTimer;
        int m_NewLinesCounter;

        // Temps de résistance de la pile avant game over
        ManualTimer m_FieldResistanceTimer;

        // Vitesse de montée de la pile
        bool m_FastRise;
        int m_Speed;

        // "Stop!"
        ManualTimer m_StopTimer;
};

#endif // FIELD_H
