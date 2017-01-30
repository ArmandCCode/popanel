#include "PopBlockSet.h"

PopBlockSet::PopBlockSet()
{

}

PopBlockSet::~PopBlockSet()
{

}

void PopBlockSet::AddBlockCoord(Coord2D p_BlockCoord)
{
    // Si le timer a dépassé 0, lever une exception. On n'ajoute pas un bloc dans un combo qui poppe déjà !
    if (this->GetPopTimer().GetElapsed() > 0)
    {
        SPException e("PopBlockSet::AddBlockCoord() - Coding problem : DO NOT add a block to an already disappearing set !");
        throw e;
    }
    // Ne pas ajouter à la liste un bloc qui existe déjà dans la liste !
    else if ( !isBlockCoordInSet(p_BlockCoord) )
    {
        this->m_ListPopBlocksCoord.push_back(p_BlockCoord);
    }
}

bool PopBlockSet::isBlockCoordInSet(Coord2D p_BlockCoord)
{
    for (unsigned int i = 0; i < this->m_ListPopBlocksCoord.size() ; i++ )
    {
        if ( this->m_ListPopBlocksCoord[i].GetX() == p_BlockCoord.GetX() && this->m_ListPopBlocksCoord[i].GetY() == p_BlockCoord.GetY() )
        {
            return true;
        }
    }

    return false;
}
