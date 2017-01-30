#include "MatchSettings.h"

MatchSettings::MatchSettings() : m_NumPlayers(1), m_InitialSpeed(0), m_Difficulty(0), m_GameDuration(0)
{

}

MatchSettings::~MatchSettings()
{

}

// Créer une liste de blocs aléatoires
void MatchSettings::CreateNewBlockSet()
{
    // Supprimer la liste existante
    this->m_BlockList.clear();

    bool blockOK = false;
    unsigned int maxColors = C_Difficulty::GetMaxColorByDifficulty( this->GetDifficulty() );;
    int tmpColor = 0;

    // génération de 384 blocs
    for (int i = 0; i < C_Field::MAXBLOCKSONSEED; i++)
    {
        blockOK = false;

        // Répéter tant que le tirage au sort n'est pas bon
        while (!blockOK)
        {
            // sélectionner un nombre au pif
            tmpColor = NumHelper::RandomInt(1, maxColors);

            // on part du principe que la couleur est bonne
            blockOK = true;

            // premier bloc, on ne regarde pas le bloc à gauche... il y en a pas :)
            if (i > 0)
            {
                // Si le bloc à gauche est identique, on en tire un autre
                if (m_BlockList[i - 1] == tmpColor)
                    blockOK = false;
            }
            // Première ligne, on ignore les vérifs sur le dessus
            if (i >= C_Field::BLOCKSPERLINE)
            {
                // le bloc de la ligne du dessus est identique
                if ( m_BlockList[i - C_Field::BLOCKSPERLINE] == tmpColor )
                    blockOK = false;
            }


        }

        m_BlockList.push_back(tmpColor);

    }


}

std::vector<int> MatchSettings::GetBlocksFromSeed(unsigned long p_start, unsigned long p_amount)
{
    std::vector<int> tmpBlocks;

    // l'algo doit pouvoir boucler si on arrive à la fin du vector !
    unsigned long index = p_start;
    while (tmpBlocks.size() < p_amount)
    {
        // L'index va au delà de la capacité ?
        if ( index >= C_Field::MAXBLOCKSONSEED )
        {
            // Et paf, modulo, pour revenir au début !
            index %= C_Field::MAXBLOCKSONSEED;
        }

        tmpBlocks.push_back(m_BlockList[index]);

        index++;
    }

    return tmpBlocks;
}
