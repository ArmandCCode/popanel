#ifndef MATCHSETTINGS_H
#define MATCHSETTINGS_H

#include <vector>
#include "SPEngine/sputil/SPNumHelper.h"
#include "game/actors/Player.h"

// Cet objet sert à paramétrer un match dans le menu
class MatchSettings
{
    public:
        MatchSettings();
        virtual ~MatchSettings();

        // Ces membres servent à initialiser un jeu, ils ne devront pas être utilisés plus tard
        // Pour l'instant, tous les joueurs auront les mêmes paramètres de départ
        unsigned int GetNumPlayers() { return m_NumPlayers; }
        void SetNumPlayers(unsigned int val) { m_NumPlayers = val; }
        unsigned int GetInitialSpeed() { return m_InitialSpeed; }
        void SetInitialSpeed(unsigned int val) { m_InitialSpeed = val; }
        unsigned int GetDifficulty() { return m_Difficulty; }
        void SetDifficulty(unsigned int val) { m_Difficulty = val; }

        // Durée souhaitée du jeu. 0 = illimitée
        unsigned long GetGameDuration() { return m_GameDuration; }
        void SetGameDuration(unsigned long val) { m_GameDuration = val; }

        // Créer une liste de blocs aléatoires
        void CreateNewBlockSet();
        std::vector<int> GetBlocksFromSeed(unsigned long p_start, unsigned long p_amount);

    protected:
    private:
        unsigned int m_NumPlayers;
        unsigned int m_InitialSpeed;
        unsigned int m_Difficulty;
        unsigned long m_GameDuration;

        // graine aléatoire pour les blocs
        std::vector<int> m_BlockList;
};

#endif // MATCHSETTINGS_H
