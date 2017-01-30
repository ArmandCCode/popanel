#ifndef POPBLOCKSET_H
#define POPBLOCKSET_H

#include <vector>
#include "timers/ManualTimer.h"
#include "game/containers/Coord2D.h"
#include "SPEngine/SPException.h"


class PopBlockSet
{
    public:
        PopBlockSet();
        virtual ~PopBlockSet();
        ManualTimer& GetPopTimer() { return m_PopTimer; }

        std::vector<Coord2D>& GetListPopBlocksCoord() { return m_ListPopBlocksCoord; }
        int GetNbBlockInSet() { return m_ListPopBlocksCoord.size(); }

        void AddBlockCoord(Coord2D p_BlockCoord); // Si le timer a d�pass� 0, lever une exception on n'ajoute pas un bloc dans un combo qui poppe d�j� ! + Ne pas ajouter � la liste un bloc qui existe d�j� dans la liste !
        bool isBlockCoordInSet(Coord2D p_BlockCoord);
    protected:
    private:
        ManualTimer m_PopTimer;
        std::vector<Coord2D> m_ListPopBlocksCoord;
};

#endif // POPBLOCKSET_H
