#include "Block.h"

Block::Block(int p_x, int p_y, int p_Color) : m_X(p_x), m_Y(p_y), m_XDest(p_x), m_YDest(p_y), m_Color(p_Color), m_BlockStatus(Block::STATUS_FIXED), m_ChainID(-1)
{

}

Block::~Block()
{

}

void Block::MoveToDest()
{
    this->SetX(this->GetXDest());
    this->SetY(this->GetYDest());
}

void Block::SetStatus(int p_Status, int p_Time)
{
    // Changement statut
    m_BlockStatus = p_Status;

    // Temps de validité
    switch (p_Status)
    {
    case Block::STATUS_FIXED:
        m_Timer.Restart();
        m_Timer.SetLength(0);
        break;
    case Block::STATUS_SWITCHING:
        m_Timer.Restart( m_Timer.GetOvertime() ); // Récupérer le temps "qui dépasse" de l'ancienne limite
        m_Timer.SetLength( p_Time );
        break;
    case Block::STATUS_SUSPEND:
        m_Timer.Restart( m_Timer.GetOvertime() ); // Récupérer le temps "qui dépasse" de l'ancienne limite
        m_Timer.SetLength( p_Time );
        break;
    case Block::STATUS_FALLING:
        m_Timer.Restart( m_Timer.GetOvertime() ); // Récupérer le temps "qui dépasse" de l'ancienne limite
        m_Timer.SetLength( p_Time );
        break;
    case Block::STATUS_POPPING:
        m_Timer.Restart(0);
        m_Timer.SetLength(0);
        break;
    }

}
