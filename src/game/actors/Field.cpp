#include "Field.h"

Field::Field() : m_NewLinesCounter(0), m_FastRise(false), m_Speed(0)
{

}

Field::~Field()
{

}

void Field::EmptyField()
{
    this->m_FieldBlocks.clear();
}

void Field::LoadField(std::vector<int> p_Blocks)
{
    this->EmptyField();

    int iNbOfLines = p_Blocks.size() / C_Field::BLOCKSPERLINE;
    bool bIncompleteLine = ((p_Blocks.size() % C_Field::BLOCKSPERLINE) > 0);

    if (bIncompleteLine)
    {
        SPException e("Exception : Structure de tableau personnalisée incorrecte.");
        throw e;
    }
    else
    {
        int iTopBorder = iNbOfLines - 1;
        for (int y = iTopBorder ; y >= 0; y-- )
        {
            for (int x = 0; x < C_Field::BLOCKSPERLINE; x++)
            {
                int idBlock = (y * C_Field::BLOCKSPERLINE) + x;
                Block tmpBlock(x, iTopBorder - y, p_Blocks[idBlock] );

                if (tmpBlock.GetColor() != Block::COLOR_NO)
                {
                    this->m_FieldBlocks.push_back(tmpBlock);
                }
            }
        }
    }
}

bool Field::isBlockAt(int p_X, int p_Y)
{
    for (unsigned int i = 0; i < this->m_FieldBlocks.size() ; i++ )
    {
        if (this->m_FieldBlocks[i].GetX() == p_X && this->m_FieldBlocks[i].GetY() == p_Y)
        {
            return true;
        }
    }

    return false;
}

Block& Field::GetBlock(int p_X, int p_Y)
{

    for (unsigned int i = 0; i < this->m_FieldBlocks.size() ; i++ )
    {
        if (this->m_FieldBlocks[i].GetX() == p_X && this->m_FieldBlocks[i].GetY() == p_Y)
        {
            return this->m_FieldBlocks[i];
        }
    }

    // Si on est arrivé là, lever une exception !
    SPException e("EXCEPTION : No block at this coordinate. Check its existence before trying to get it.");
    throw e;
}

Block& Field::GetBlock(int p_ID)
{
    return this->m_FieldBlocks[p_ID];
}

int Field::GetBlockIDAt(int p_X, int p_Y)
{
    for (unsigned int i = 0; i < this->m_FieldBlocks.size() ; i++ )
    {
        if (this->m_FieldBlocks[i].GetX() == p_X && this->m_FieldBlocks[i].GetY() == p_Y)
        {
            return i;
        }
    }

    // Si on est arrivé là, lever une exception !
    SPException e("EXCEPTION : No block at this coordinate. Check its existence before trying to get its ID.");
    throw e;
}


bool Field::DeleteBlock(unsigned int p_ID)
{
    // Vérifier si l'ID est pas hors limite !
    if (p_ID >= m_FieldBlocks.size() || p_ID < 0 )
    {
        SPException e("Field::DeleteBlock - ID is out of range");
        throw e;
    }
    else
    {
        this->m_FieldBlocks.erase(m_FieldBlocks.begin() + p_ID);
        return true;
    }

}

bool Field::IsSwappingAllowed(int p_X1, int p_Y1, bool& p_Block1Move, int p_X2, int p_Y2, bool& p_Block2Move)
{
    // Left block
    if ( this->IsMoveAllowed(p_X1, p_Y1, p_X2, p_Y2) )
    {
        p_Block1Move = true;
    }

    // Right block
    if ( this->IsMoveAllowed(p_X2, p_Y2, p_X1, p_Y1) )
    {
        p_Block2Move = true;
    }

    return (p_Block1Move || p_Block2Move);
}

bool Field::IsMoveAllowed(int p_X1, int p_Y1, int p_X2, int p_Y2)
{
    // To request a move, a block needs to exist at the first coordinate
    if ( this->isBlockAt(p_X1, p_Y1) )
    {
        // Checking rules who disallow moving the block (X1, Y1) to (X2, Y2)
        // There's a block at the second coordinate, and...
        if ( this->isBlockAt(p_X2, p_Y2) )
        {
            switch (this->GetBlock(p_X2, p_Y2).GetStatus())
            {
            case Block::STATUS_FIXED:
                // It's not moving. No reason to disallow. Move is allowed.
                break;
            case Block::STATUS_SWITCHING:
                // It is moving, but...
                if (this->GetBlock(p_X2, p_Y2).GetXDest() == p_X1 && this->GetBlock(p_X2, p_Y2).GetYDest() == p_Y1 )
                {
                    // It is moving to (X1, Y1). Move is allowed.
                }
                else
                {
                    // It is moving elsewhere. Move is not allowed.
                    return false;
                }
                break;
            default:
                return false;
            }

        }
        else if ( this->IsABlockGoingToDest(p_X2, p_Y2) )
        {
            // Another block is trying to move to (X2, Y2). Move is not allowed.
            return false;
        }
        else if ( this->IsABlockGoingToDest(p_X2, p_Y2 + 1) )
        {
            // A block is falling on top of (X2, Y2). Move is not allowed, as it would be easily exploitable...
            return false;
        }
        else if ( this->isBlockAt(p_X2, p_Y2 + 1) )
        {
            // A block is falling at (X2, Y2). Move is not allowed.
            if ( this->GetBlock(p_X2, p_Y2 + 1).GetStatus() == Block::STATUS_FALLING || this->GetBlock(p_X2, p_Y2 + 1).GetStatus() == Block::STATUS_SUSPEND )
            {
                return false;
            }
        }

        // Block at (X1, Y1) is above the first line ? Check falling conditions.
        if ( p_Y1 > 0 )
        {
            /*
            Block in (X1, Y1) cant move if the block under it is moving to another coordinate,
            as it is going to leave an empty space where the block in (X1, Y1) is going to fall.
            // */
            if ( this->isBlockAt(p_X1, p_Y1 - 1) )
            {
                if ( this->GetBlock(p_X1, p_Y1 - 1).GetStatus() == Block::STATUS_SWITCHING && !this->IsABlockGoingToDest(p_X1, p_Y1 - 1) )
                {
                    return false;
                }
            }
        }


        // Otherwise, moving the block in (X1, Y1) requires that it isn't already moving...
        switch (this->GetBlock(p_X1, p_Y1).GetStatus() )
        {
        case Block::STATUS_FIXED:
            return true;
            break;
        default:
            break;
        }
    }

    return false;
}


void Field::UpdateTimers(unsigned long p_TimePassedMs)
{
    if ( this->IsStopTimeInEffect() )
    {
        this->GetStopTimer().UpdateTimer(p_TimePassedMs);
        if ( this->GetStopTimer().IsTimerEnd() )
        {
            this->ResetStopTime();
        }
    }

    // Si aucun bloc n'est en cours d'interaction, mettre à jour les timers de pile
    if ( IsABlockInteracting() )
    {
        // Mettre à jour les timers des blocs
        for (unsigned int i = 0; i < m_FieldBlocks.size(); i++ )
        {
            // Si le statut est un statut actif
            switch (this->m_FieldBlocks[i].GetStatus())
            {
            case Block::STATUS_FIXED:
            case Block::STATUS_POPPING:
                // Le bloc ne se déplace pas sur la grille : on le laisse tranquille
                break;
            default:
                this->m_FieldBlocks[i].GetTimer().UpdateTimer(p_TimePassedMs);
                break;
            }
        }
    }
    else if ( !this->IsStopTimeInEffect() )
    {
        // Si un bloc à la coordonnée Y 11 empêche la pile de monter
        bool isBlockAtTop = false;
        for (int x = 0; x < C_Field::BLOCKSPERLINE; x++)
        {
            if (this->isBlockAt(x, C_Field::LINESPERFIELD - 1))
            {
                isBlockAtTop = true;
            }
        }

        if (isBlockAtTop)
        {
            // Augmenter le timer de résistance de la pile
            this->GetFieldResistanceTimer().UpdateTimer(p_TimePassedMs);
        }
        else
        {
            // Si la voie est libre, on remet le timer de résistance à 0
            this->GetFieldResistanceTimer().Restart();
            // Mettre à jour les timers de la pile
            this->GetNewLineTimer().UpdateTimer(p_TimePassedMs);
        }

    }

}


bool Field::IsABlockInteracting()
{
    for (unsigned int i = 0; i < this->m_FieldBlocks.size() ; i++ )
    {
        if (this->m_FieldBlocks[i].GetStatus() != Block::STATUS_FIXED )
        {
            return true;
        }
    }

    return false;
}

bool Field::IsABlockGoingToDest(int p_X, int p_Y)
{
    for (unsigned int i = 0; i < this->m_FieldBlocks.size() ; i++ )
    {
        if (this->m_FieldBlocks[i].GetXDest() == p_X && this->m_FieldBlocks[i].GetYDest() == p_Y && this->m_FieldBlocks[i].GetStatus() == Block::STATUS_SWITCHING )
        {
            return true;
        }
    }

    return false;
}

bool Field::IsABlockPopping()
{
    for (unsigned int i = 0; i < this->m_FieldBlocks.size() ; i++ )
    {
        if (this->m_FieldBlocks[i].GetStatus() == Block::STATUS_POPPING )
        {
            return true;
        }
    }

    return false;
}

void Field::SetNewNextLine(std::vector<int> p_NextLineBlocks)
{
    this->m_NextLine.clear();

    for (int x = 0; x < C_Field::BLOCKSPERLINE; x++)
    {
        Block tmpBlock( x, 0, p_NextLineBlocks[x] );
        this->m_NextLine.push_back(tmpBlock);
    }
}

void Field::InsertNextLineInField()
{
    // Décaler l'intégralité des blocs d'un bloc vers le haut
    for (unsigned int i = 0; i < this->m_FieldBlocks.size(); i++)
    {
        this->GetBlock(i).SetY(GetBlock(i).GetY() + 1);
        this->GetBlock(i).SetYDest(GetBlock(i).GetYDest() + 1);
    }

    // Insérer les blocs de la nouvelle ligne
    for (unsigned int x = 0; x < this->m_NextLine.size(); x++)
    {
        this->m_FieldBlocks.push_back( GetNewLineBlock(x) );
    }

    // Ajouter 1 au nombre de lignes
    this->m_NewLinesCounter += 1;
    // Si ça fait 5, on augmente la vitesse de 1
    if (this->m_NewLinesCounter >= 5)
    {
        this->m_NewLinesCounter = 0;
        this->RiseSpeed();
    }

}

Block& Field::GetNewLineBlock(int p_xCoord)
{
    return this->m_NextLine[p_xCoord];
}

void Field::SetSpeed(int p_Speed)
{
    // Niveau maximal : 99
    if (p_Speed > 0 && p_Speed < 100)
    {
        this->m_Speed = p_Speed;
    }
}

int Field::GetSpeed()
{
    return this->m_Speed;
}

void Field::RiseSpeed(int p_Increments )
{
    this->SetSpeed( this->GetSpeed() + p_Increments );
}

void Field::EnableFastRise(bool p_Fast)
{
    this->m_FastRise = p_Fast;

    // Forcer la montée rapide
    if (p_Fast)
    {
        float tmpPcElapsed = this->GetNewLineTimer().GetElapsedPc();
        this->GetNewLineTimer().SetLength( C_Field::TIMERLINERISE_FAST );
        this->GetNewLineTimer().Restart( ceil((float)C_Field::TIMERLINERISE_FAST * tmpPcElapsed) );
    }
}

void Field::StopFastRise()
{
    // Arrêter la montée rapide immédiatement
    this->m_FastRise = false;

    float tmpPcElapsed = this->GetNewLineTimer().GetElapsedPc();
    int tmpTimePerLine = C_Field::GetLineRiseTime(this->GetSpeed());
    this->GetNewLineTimer().SetLength(tmpTimePerLine);
    this->GetNewLineTimer().Restart( ceil((float)tmpTimePerLine * tmpPcElapsed) );
}

void Field::ResetNewLineTimer()
{
    // Nouvelle formule :
    int tmpTimePerLine = 0;

    // When rising speed is sped up, the timer before the new line has always the same value
    if ( this->IsFastRiseEnabled() )
    {
        tmpTimePerLine = C_Field::TIMERLINERISE_FAST;
    }
    else
    {
        tmpTimePerLine = C_Field::GetLineRiseTime(this->GetSpeed());
    }

    this->GetNewLineTimer().Restart( this->GetNewLineTimer().GetOvertime() );
    this->GetNewLineTimer().SetLength(tmpTimePerLine);
}

void Field::AddStopTime(int p_StopSeconds)
{
    // On limite le Stop à 16 secondes (c'est pas mal déjà)
    if (this->GetStopTimer().GetRemaining() + (p_StopSeconds * 1000) < 16000 )
    {
        this->GetStopTimer().SetLength( this->GetStopTimer().GetLength() + (p_StopSeconds * 1000) );
    }
}

void Field::ResetStopTime() // En cas d'annulation causée par un appui sur "SHIFT"
{
    this->GetStopTimer().Restart();
    this->GetStopTimer().SetLength(0);
}

bool Field::IsStopTimeInEffect()
{
    return (this->GetStopTimer().GetLength() > 0);
}
