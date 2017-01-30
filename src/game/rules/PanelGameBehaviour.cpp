#include "PanelGameBehaviour.h"

PanelGameBehaviour::PanelGameBehaviour()
{


}

PanelGameBehaviour::~PanelGameBehaviour()
{

}

void PanelGameBehaviour::InitPlayer( Player& p_Player, MatchSettings& p_MatchSettings )
{
    // Charger les blocs visibles
    std::vector<int> tmpField = p_MatchSettings.GetBlocksFromSeed(p_Player.GetIDinBlockList(), 5*C_Field::BLOCKSPERLINE);
    p_Player.GetField().LoadField(tmpField);
    p_Player.SetIDinBlockList(p_Player.GetIDinBlockList() + 5*C_Field::BLOCKSPERLINE);
    // R�cup�rer les 6 prochains blocs
    p_Player.GetField().SetNewNextLine(p_MatchSettings.GetBlocksFromSeed(p_Player.GetIDinBlockList(), C_Field::BLOCKSPERLINE));
    p_Player.SetIDinBlockList(p_Player.GetIDinBlockList() + C_Field::BLOCKSPERLINE);
    // Positionner le curseur � la coordonn�e 3:3
    p_Player.GetCursor().Move(2, 3);
}

// Controlling the game from outside
// Cursor moving
bool PanelGameBehaviour::RequestCursorMoveUp(Player& p_Player)
{
    return p_Player.GetCursor().GoUp();
}

bool PanelGameBehaviour::RequestCursorMoveDown(Player& p_Player)
{
    return p_Player.GetCursor().GoDown();
}

bool PanelGameBehaviour::RequestCursorMoveLeft(Player& p_Player)
{
    return p_Player.GetCursor().GoLeft();
}

bool PanelGameBehaviour::RequestCursorMoveRight(Player& p_Player)
{
    return p_Player.GetCursor().GoRight();
}

// Tile swapping
bool PanelGameBehaviour::RequestSwapping(Player& p_Player, GameEvents& p_GameEvents)
{
    bool bBlock1Move = false;
    bool bBlock2Move = false;

    int X1 = p_Player.GetCursor().GetX();
    int Y1 = p_Player.GetCursor().GetY();
    int X2 = p_Player.GetCursor().GetX() + 1;
    int Y2 = p_Player.GetCursor().GetY();

    // Si au moins un des deux blocs en dessous du curseur est en mesure d'�tre d�plac�
    if ( p_Player.GetField().IsSwappingAllowed(X1, Y1, bBlock1Move, X2, Y2, bBlock2Move) )
    {
        if (bBlock1Move)
        {
            p_Player.GetField().GetBlock(X1, Y1).SetXDest(X2) ;
            p_Player.GetField().GetBlock(X1, Y1).SetYDest(Y2) ;
            p_Player.GetField().GetBlock(X1, Y1).SetStatus(Block::STATUS_SWITCHING, C_BlockTimers::GetSwitchTime( p_Player.GetDifficulty() ) );
        }

        if (bBlock2Move)
        {
            p_Player.GetField().GetBlock(X2, Y2).SetXDest(X1) ;
            p_Player.GetField().GetBlock(X2, Y2).SetYDest(Y1) ;
            p_Player.GetField().GetBlock(X2, Y2).SetStatus(Block::STATUS_SWITCHING, C_BlockTimers::GetSwitchTime( p_Player.GetDifficulty() ) );
        }

        p_GameEvents.AddEvent(GameEvents::EVT_PANELMOVE);

        return true;
    }

    return false;
}

bool PanelGameBehaviour::RequestFieldRise(Player& p_Player)
{
    // Transmettre la demande � l'objet joueur
    p_Player.SetFastRiseRequest(true);
    return true;

}

bool PanelGameBehaviour::RequestFieldRiseStop(Player& p_Player)
{
    // Le joueur a arr�t� d'appuyer sur le bouton, on demande l'arr�t de l'ascension rapide � la prochaine ligne
    p_Player.SetFastRiseRequest(false);
    return true;
}

void PanelGameBehaviour::UpdateGameLogic(Player& p_Player, unsigned long p_TimePassedMs, GameEvents& p_GameEvents, MatchSettings& p_MatchSettings)
{
    // pour v�rifier si le joueur a fait Game Over
    this->UpdatePlayerStatus(p_Player);

    if ( p_Player.GetStatus() == Player::PLAYER_STAT_PLAYING )
    {
        // Mettre � jour le timer de la partie
        p_Player.GetGameTimer().UpdateTimer(p_TimePassedMs);

        // Je pense que c'est un bon moment pour recalculer le timing de la pile en fonction du souhait du joueur...
        switch (p_Player.GetFastRiseRequest())
        {
            case true:
                p_Player.GetField().EnableFastRise(true);
                // Forcer l'ascension arr�te le stop
                if ( p_Player.GetField().IsStopTimeInEffect() )
                {
                    p_Player.GetField().ResetStopTime();
                }
            break;
            case false:
                p_Player.GetField().EnableFastRise(false);
                // Si la pile est d�j� arrt�e par un bloc qui disparait, on peut annuler l'ascension rapide
                if ( p_Player.GetField().IsABlockPopping() )
                {
                    p_Player.GetField().StopFastRise();
                }
            break;
        }
        // Mettre � jour les timers de la pile et des blocs
        p_Player.GetField().UpdateTimers(p_TimePassedMs);

        // Mettre � jour les timers des blocs qui poppent
        for (unsigned int i = 0 ; i < p_Player.GetDisappearingBlocks().size(); i++)
        {
            p_Player.GetDisappearingBlocks()[i].GetPopTimer().UpdateTimer(p_TimePassedMs);
        }

        // Si les blocs bougent de quelque sorte, ne mettre � jour que les blocs
        if (p_Player.GetField().IsABlockInteracting() )
        {
            this->UpdateBlocks(p_Player, p_GameEvents);
        }
        else
        {
            this->UpdateField(p_Player, p_MatchSettings);
        }

        this->CheckMatches(p_Player, p_GameEvents);

    }
    else if (p_Player.GetStatus() == Player::PLAYER_STAT_ENDTIMER )
    {
        p_GameEvents.AddEvent(GameEvents::EVT_COUNTEND);
    }
    else if (p_Player.GetStatus() == Player::PLAYER_STAT_DEFEAT )
    {
        p_GameEvents.AddEvent(GameEvents::EVT_DEFEAT);
    }

}

void PanelGameBehaviour::UpdateBlocks(Player& p_Player, GameEvents& p_GameEvents)
{

    int oldX = 0, oldY = 0;
    std::vector<Coord2D> BlocksCoordToRemove;

    Field& PlayerField = p_Player.GetField();

    for (int i = 0; i < PlayerField.GetNbBlocks(); i++ )
    {
        if (PlayerField.GetBlock(i).GetStatus() != Block::STATUS_FIXED)
        {
            // Tous les timers �coul�s
            while (PlayerField.GetBlock(i).GetTimer().IsTimerEnd())
            {
                switch ( PlayerField.GetBlock(i).GetStatus() )
                {
                case Block::STATUS_SWITCHING:
                    oldX = PlayerField.GetBlock(i).GetX();
                    oldY = PlayerField.GetBlock(i).GetY();

                    // D�placer le bloc � sa destination
                    PlayerField.GetBlock(i).MoveToDest();

                    // Regarder si les coordonn�es d'origine du bloc d�plac� sont/vont �tre occup�es par un autre bloc
                    if ( PlayerField.isBlockAt( oldX, oldY + 1 ) )
                    {
                        if ( PlayerField.GetBlock( oldX, oldY + 1 ).GetStatus() == Block::STATUS_FIXED )
                        {
                            if ( !PlayerField.IsABlockGoingToDest(oldX, oldY) && !PlayerField.isBlockAt(oldX, oldY) )
                            {
                                PlayerField.GetBlock( oldX, oldY + 1 ).SetStatus(Block::STATUS_SUSPEND, C_BlockTimers::GetSuspendTime( p_Player.GetDifficulty() ) );
                            }
                        }
                    }
                    // S'il y a un trou � la place des anciennes coordonn�es, faire tomber le bloc au dessus

                    // Regarder si le bloc d�plac� est maintenant au dessus d'un trou
                    if (PlayerField.GetBlock(i).GetY() > 0)
                    {
                        if ( !PlayerField.isBlockAt(PlayerField.GetBlock(i).GetX(), PlayerField.GetBlock(i).GetY() - 1 ) )
                        {
                            PlayerField.GetBlock(i).SetStatus( Block::STATUS_SUSPEND, C_BlockTimers::GetSuspendTime( p_Player.GetDifficulty() ) );
                        }
                        else
                        {
                            PlayerField.GetBlock(i).SetStatus( Block::STATUS_FIXED, 0 );
                            p_Player.GetStoppedBlocksID().push_back(i);
                        }
                    }
                    else
                    {
                        // Le bloc est � la coordonn�e du sol absolu
                        PlayerField.GetBlock(i).SetStatus( Block::STATUS_FIXED, 0 );
                        p_Player.GetStoppedBlocksID().push_back(i);
                    }

                    break;

                case Block::STATUS_SUSPEND:
                    PlayerField.GetBlock(i).SetStatus( Block::STATUS_FALLING, C_BlockTimers::GetFallTime( p_Player.GetDifficulty() ) );
                    break;
                case Block::STATUS_FALLING:
                    {
                        bool bToucheSol = false;

                        // Cas impossible : une chute qui d�marre � la coordonn�e 0 : on corrige en fixant le bloc, toutefois, on n'est jamais trop prudent...
                        if (PlayerField.GetBlock(i).GetY() > 0)
                        {
                            // Faire tomber le bloc
                            PlayerField.GetBlock(i).SetYDest( PlayerField.GetBlock(i).GetY() - 1 );
                            PlayerField.GetBlock(i).MoveToDest();

                            // Faire suivre tous les blocs coll�s au dessus
                            int y = PlayerField.GetBlock(i).GetY() + 2;
                            bool bContinueStackFall = true;
                            while ( bContinueStackFall )
                            {
                                // Il faudrait arr�ter la chute du reste de la pile lorsqu'on trouve...
                                if ( PlayerField.isBlockAt( PlayerField.GetBlock(i).GetX(), y) )
                                {
                                    switch ( PlayerField.GetBlock(PlayerField.GetBlock(i).GetX(), y).GetStatus() )
                                    {
                                    case Block::STATUS_FALLING: // Un bloc qui se pr�pare � tomber (avec un autre timing, peu probable mais, prudence...)
                                    case Block::STATUS_SUSPEND:
                                    case Block::STATUS_POPPING: // Un bloc qui disparait
                                    case Block::STATUS_SWITCHING: // Un bloc qui se d�place
                                        bContinueStackFall = false;
                                        break;
                                    default:
                                        break;
                                    }

                                }
                                else
                                {
                                    // Un espace vide, qu'un bloc s'y d�place ou non !
                                    bContinueStackFall = false;
                                }

                                // Si le bloc du dessus peut �tre inclus � la pile, on le d�place
                                if ( bContinueStackFall )
                                {
                                    PlayerField.GetBlock(PlayerField.GetBlock(i).GetX(), y).SetYDest( y - 1 );
                                    PlayerField.GetBlock(PlayerField.GetBlock(i).GetX(), y).MoveToDest();
                                    // Ajouter le bloc � la liste des blocs d�plac�s
                                    // m_StoppedBlocksID.push_back( PlayerField.GetBlockIDAt(PlayerField.GetBlock(i).GetX(), y - 1) );
                                    y++;
                                }

                            }

                            // Si le bloc est tomb� � une coordonn�e Y sup�rieure � 0
                            if (PlayerField.GetBlock(i).GetY() > 0)
                            {
                                // Si un bloc se situe ou alors va en dessous de la nouvelle coordonn�e, on consid�re que le bloc a fini sa chute
                                if ( PlayerField.isBlockAt(PlayerField.GetBlock(i).GetX(),  PlayerField.GetBlock(i).GetY() - 1) || PlayerField.IsABlockGoingToDest(PlayerField.GetBlock(i).GetX() , PlayerField.GetBlock(i).GetY() - 1) )
                                {
                                    bToucheSol = true;
                                }
                                else
                                {
                                    // Sinon, il continue de tomber
                                    bToucheSol = false;
                                }
                            }
                            else
                            {
                                // Si on tombe sur le sol, le bloc a termin� sa chute
                                bToucheSol = true;
                            }

                        }
                        else
                        {
                            bToucheSol = true;
                        }

                        // Si le bloc qui tombe a touch� le sol...
                        if (bToucheSol)
                        {

                            PlayerField.GetBlock(i).SetStatus(Block::STATUS_FIXED, 0 );
                            p_Player.GetStoppedBlocksID().push_back(i);
                            p_GameEvents.AddEvent( GameEvents::EVT_PANELBUMP );

                            // Les blocs situ�s au dessus de celui-ci touchent le sol aussi
                            int y = PlayerField.GetBlock(i).GetY() + 1;
                            bool bContinueStackFall = true;

                            while ( bContinueStackFall )
                            {
                                // Il faudrait arr�ter la chute du reste de la pile lorsqu'on trouve...
                                if ( PlayerField.isBlockAt( PlayerField.GetBlock(i).GetX(), y) )
                                {
                                    switch ( PlayerField.GetBlock(PlayerField.GetBlock(i).GetX(), y).GetStatus() )
                                    {
                                    case Block::STATUS_FALLING: // Un bloc qui se pr�pare � tomber (avec un autre timing, peu probable mais, prudence...)
                                    case Block::STATUS_SUSPEND:
                                    case Block::STATUS_POPPING: // Un bloc qui disparait
                                    case Block::STATUS_SWITCHING: // Un bloc qui se d�place
                                        bContinueStackFall = false;
                                        break;
                                    default:
                                        break;
                                    }

                                }
                                else
                                {
                                    // Un espace vide, qu'un bloc s'y d�place ou non !
                                    bContinueStackFall = false;
                                }

                                // Si le bloc du dessus peut �tre inclus � la pile, on le d�place
                                if ( bContinueStackFall )
                                {
                                    // Ajouter le bloc � la liste des blocs d�plac�s et devenus fixes
                                    p_Player.GetStoppedBlocksID().push_back( PlayerField.GetBlockIDAt(PlayerField.GetBlock(i).GetX(), y) );
                                    y++;
                                }
                            }

                        }
                        else
                        {
                            PlayerField.GetBlock(i).SetStatus( Block::STATUS_FALLING, C_BlockTimers::GetFallTime( p_Player.GetDifficulty() ));
                        }
                    }
                    break; // Block::STATUS_FALLING ou GOINGTOFALL
                default:
                    break;

                }
            }
        }

    }



    // Occupons-nous des listes de blocs qui "poppent"
    for (unsigned int i = 0; i < p_Player.GetDisappearingBlocks().size(); i++ )
    {
        if ( p_Player.GetDisappearingBlocks()[i].GetPopTimer().IsTimerEnd() )
        {
            for (unsigned int j = 0 ; j < p_Player.GetDisappearingBlocks()[i].GetListPopBlocksCoord().size() ; j++ )
            {
                Coord2D tmpID = p_Player.GetDisappearingBlocks()[i].GetListPopBlocksCoord()[j];
                BlocksCoordToRemove.push_back( p_Player.GetDisappearingBlocks()[i].GetListPopBlocksCoord()[j] );
            }

            p_Player.GetDisappearingBlocks().erase(p_Player.GetDisappearingBlocks().begin() + i); // Supprimer le combo maintenant qu'il a fini de dispara�tre !
        }
    }

    // Supprimer les blocs marqu�s pour suppression...
    // Du plus �loign� dans au moins �loign�, pour les sortir proprement du vector sans casser le parcours par index !
    for (int i = BlocksCoordToRemove.size() - 1; i >= 0; i--)
    {
        Coord2D tmpCoord = BlocksCoordToRemove[i];
        int tmpChainMult = PlayerField.GetBlock( PlayerField.GetBlockIDAt( tmpCoord.GetX(), tmpCoord.GetY() ) ).GetChainID() ;

        int tmpIDBlock = PlayerField.GetBlockIDAt( tmpCoord.GetX(), tmpCoord.GetY() );
        PlayerField.DeleteBlock( tmpIDBlock );
        p_GameEvents.AddEvent(GameEvents::EVT_PANELPOP);

        // S'il y a un bloc au dessus, le faire tomber
        if ( PlayerField.isBlockAt(tmpCoord.GetX() , tmpCoord.GetY() + 1) )
        {
            // Cha�nes : appliquer un multiplicateur aux blocs disponibles au dessus des blocs qui disparait
            int y = tmpCoord.GetY() + 1;
            bool bContinueStackMult = true;
            while ( bContinueStackMult )
            {
                // Il faudrait arr�ter la chute du reste de la pile lorsqu'on trouve...
                if ( PlayerField.isBlockAt( tmpCoord.GetX(), y) )
                {
                    switch ( PlayerField.GetBlock(tmpCoord.GetX(), y).GetStatus() )
                    {
                    case Block::STATUS_FALLING: // Un bloc qui se pr�pare � tomber
                    case Block::STATUS_SUSPEND:
                    case Block::STATUS_POPPING: // Un bloc qui disparait
                    case Block::STATUS_SWITCHING: // Un bloc qui se d�place
                        bContinueStackMult = false;
                        break;
                    default:
                        break;
                    }

                }
                else
                {
                    // Un espace vide, qu'un bloc s'y d�place ou non !
                    bContinueStackMult = false;
                }

                // Si le bloc du dessus peut �tre inclus � la pile, on lui applique le multiplicateur
                if ( bContinueStackMult )
                {
                    PlayerField.GetBlock(tmpCoord.GetX(), y).SetChainID(tmpChainMult);
                    y++;
                }

            }

            // Pour tomber, le bloc au dessus doit �tre disponible
            if ( PlayerField.GetBlock(tmpCoord.GetX() , tmpCoord.GetY() + 1).GetStatus() == Block::STATUS_FIXED )
            {
                PlayerField.GetBlock(tmpCoord.GetX() , tmpCoord.GetY() + 1).SetStatus(Block::STATUS_SUSPEND, C_BlockTimers::GetSuspendTime( p_Player.GetDifficulty() ) );
            }

        }

    }

}

void PanelGameBehaviour::CheckMatches(Player& p_Player, GameEvents& p_GameEvents)
{
    std::vector<int> ListMatchedBlockIDs;

    Field& PlayerField = p_Player.GetField();
    std::vector<int>& PlayerChains = p_Player.GetChainList();

    int tmpIDMaxMulti = -1;

    if(p_Player.GetStoppedBlocksID().size() > 0)
    {
        // Pour chaque bloc de la liste
        for (unsigned int i = 0; i < p_Player.GetStoppedBlocksID().size(); i++)
        {

            // Nombre de blocs de m�me couleur trouv�s dans les 4 directions
            int iVertUp = 0, iVertDown = 0, iHoriLeft = 0, iHoriRight = 0;

            // Coordonn�es du bloc de r�f�rence auquel commence le test
            int RefBlockID = p_Player.GetStoppedBlocksID()[i];
            int RefBlockX = p_Player.GetField().GetBlock(RefBlockID).GetX();
            int RefBlockY = p_Player.GetField().GetBlock(RefBlockID).GetY();
            int RefBlockColour = p_Player.GetField().GetBlock(RefBlockID).GetColor();

            // Matching vertical
            // Variables pour parcourir le tableau
            bool bContinue = true;
            int tmpX = RefBlockX;
            int tmpY = RefBlockY;
            // Vers le haut
            while (bContinue)
            {
                tmpY++;
                if ( this->IsABlockMatchableAt(p_Player, tmpX, tmpY, RefBlockColour) )
                {
                    iVertUp++;
                }
                else
                {
                    bContinue = false;
                }
            }

            // Vers le bas
            // R�initialiser les variables pour parcourir le tableau
            bContinue = true;
            tmpX = RefBlockX;
            tmpY = RefBlockY;
            // Vers le haut
            while (bContinue)
            {
                tmpY--;
                if ( this->IsABlockMatchableAt(p_Player, tmpX, tmpY, RefBlockColour) )
                {
                    iVertDown++;
                }
                else
                {
                    bContinue = false;
                }
            }

            // Total des blocs trouv�s >= 3 ?
            int iVertTotal = iVertUp + iVertDown + 1;
            if (iVertTotal >= 3)
            {
                // Ajout des ID de blocs concern�s � la liste des blocs "attach�s"
                for (int y = (RefBlockY - iVertDown); y < (RefBlockY + iVertUp + 1); y++ )
                {
                    bool isAlreadyThere = false;

                    // On v�rifie que le bloc ne soit pas d�j� dans la liste pour �viter les doublons
                    for (unsigned int id = 0; id < ListMatchedBlockIDs.size(); id++)
                    {
                        if (ListMatchedBlockIDs[id] == p_Player.GetField().GetBlockIDAt(RefBlockX, y) )
                        {
                            isAlreadyThere = true;
                        }
                    }

                    // Si le bloc n'est pas d�j� pr�sent, on l'ajoute
                    if ( !isAlreadyThere )
                    {
                        ListMatchedBlockIDs.push_back(p_Player.GetField().GetBlockIDAt(RefBlockX, y));

                        // Regarder si le bloc attach� a un multiplicateur
                        int tmpChainID = p_Player.GetField().GetBlock(RefBlockX, y).GetChainID();
                        if ( tmpChainID != -1  )
                        {
                            // Si oui, regarder s'il est plus grand que les autres d�tect�s jusqu'ici
                            // Inutile de tester si c'est le m�me
                            if ( tmpChainID != tmpIDMaxMulti )
                            {
                                if ( PlayerChains[tmpChainID] > PlayerChains[tmpIDMaxMulti] )
                                {
                                    // Si le multiplicateur est plus grand, on le propage � ce combo
                                    tmpIDMaxMulti = tmpChainID;
                                }
                            }
                        }
                    }

                }

            }

            // Matching horizontal
            // Vers la gauche
            // R�initialiser les variables pour parcourir le tableau
            bContinue = true;
            tmpX = RefBlockX;
            tmpY = RefBlockY;
            // Vers le haut
            while (bContinue)
            {
                tmpX--;
                if ( this->IsABlockMatchableAt(p_Player, tmpX, tmpY, RefBlockColour) )
                {
                    iHoriLeft++;
                }
                else
                {
                    bContinue = false;
                }
            }

            // Vers la droite
            // R�initialiser les variables pour parcourir le tableau
            bContinue = true;
            tmpX = RefBlockX;
            tmpY = RefBlockY;
            // Vers le haut
            while (bContinue)
            {
                tmpX++;
                if ( this->IsABlockMatchableAt(p_Player, tmpX, tmpY, RefBlockColour) )
                {
                    iHoriRight++;
                }
                else
                {
                    bContinue = false;
                }
            }

            // Total des blocs trouv�s >= 3 ?
            int iHoriTotal = iHoriLeft + iHoriRight + 1;
            if (iHoriTotal >= 3)
            {
                // Ajout des ID de blocs concern�s � la liste des blocs "attach�s"
                for (int x = (RefBlockX - iHoriLeft); x < (RefBlockX + iHoriRight + 1); x++ )
                {
                    bool isAlreadyThere = false;

                    // On v�rifie que le bloc ne soit pas d�j� dans la liste pour �viter les doublons
                    for (unsigned int id = 0; id < ListMatchedBlockIDs.size(); id++)
                    {
                        if (ListMatchedBlockIDs[id] == p_Player.GetField().GetBlockIDAt(x, RefBlockY) )
                        {
                            isAlreadyThere = true;
                        }
                    }

                    // Si le bloc n'est pas d�j� pr�sent, on l'ajoute
                    if ( !isAlreadyThere )
                    {
                        ListMatchedBlockIDs.push_back( p_Player.GetField().GetBlockIDAt(x, RefBlockY) );

                        // Regarder si le bloc attach� a un multiplicateur
                        int tmpChainID = p_Player.GetField().GetBlock(x, RefBlockY).GetChainID();
                        if ( tmpChainID != -1  )
                        {
                            // Si oui, regarder s'il est plus grand que les autres d�tect�s jusqu'ici !
                            // Inutile de tester si c'est le m�me
                            if ( tmpChainID != tmpIDMaxMulti )
                            {
                                if ( PlayerChains[tmpChainID] > PlayerChains[tmpIDMaxMulti] )
                                {
                                    // Si le multiplicateur est plus grand, on le propage � ce combo
                                    tmpIDMaxMulti = tmpChainID;
                                }
                            }
                        }
                    }

                }
            }
        }

        // Tous les blocks "match�s" doivent �tre modifi�s pour disparaitre
        if ( ListMatchedBlockIDs.size() > 0 )
        {
            PopBlockSet tmpPop;

            // Si aucun multiplicateur de cha�ne n'a �t� trouv�, il faut en cr�er un
            if (tmpIDMaxMulti == -1)
            {
                tmpIDMaxMulti = PlayerChains.size();
                PlayerChains.push_back(1);
            }
            else
            {
                // Si on en a trouv� un, augmenter sa valeur de 1
                PlayerChains[tmpIDMaxMulti] += 1;
            }


            for (unsigned int i = 0; i < ListMatchedBlockIDs.size(); i++ )
            {
                Coord2D tmpCoord( PlayerField.GetBlock( ListMatchedBlockIDs[i] ).GetX(), PlayerField.GetBlock( ListMatchedBlockIDs[i] ).GetY() );
                tmpPop.AddBlockCoord( tmpCoord );

                // le temps de disparition est plus rapide en mode moyen et difficile
                tmpPop.GetPopTimer().SetLength(C_BlockTimers::TIMERPOP_GROUPBASE + ( C_BlockTimers::GetPopTime( p_Player.GetDifficulty() ) * tmpPop.GetNbBlockInSet() ) );

                PlayerField.GetBlock( ListMatchedBlockIDs[i] ).SetStatus(Block::STATUS_POPPING, 0 );
                PlayerField.GetBlock( ListMatchedBlockIDs[i] ).SetChainID(tmpIDMaxMulti); // On applique l'ID de cha�ne maximal

            }

            // On r�compense l'assemblage d'un bon petit score !
            int tmpValeurScore = (10 * tmpPop.GetNbBlockInSet() );
            if (tmpPop.GetNbBlockInSet() > 3)
            {
                tmpValeurScore += (10 * (tmpPop.GetNbBlockInSet() - 3) );
            }
            if (tmpIDMaxMulti >= 0)
            {
                tmpValeurScore = tmpValeurScore * PlayerChains[tmpIDMaxMulti];
            }
            p_Player.AwardPoints( tmpValeurScore );

            p_Player.GetDisappearingBlocks().push_back(tmpPop);
        }

    }

    // Temps d'arr�t pour les combos
    if ( ListMatchedBlockIDs.size() >= 3)
    {
        p_Player.GetField().StopFastRise(); // Un assemblage arr�te la mont�e rapide
    }
    if ( ListMatchedBlockIDs.size() > 3 )
    {
        p_GameEvents.AddEvent( GameEvents::EVT_COMBOCHAIN );
        p_Player.GetField().AddStopTime(3);
    }
    // Temps d'arr�t pour les cha�nes (si existant)
    if (tmpIDMaxMulti != -1)
    {
        if ( PlayerChains[tmpIDMaxMulti] > 1 )
        {
            p_GameEvents.AddEvent( GameEvents::EVT_COMBOCHAIN );
            PlayerField.AddStopTime(4);
        }
    }

    // Tous les blocs "arr�t�s" qui n'ont pas �t� "match�s" voient leur multiplicateur remis � 0
    for (unsigned int i = 0; i < p_Player.GetStoppedBlocksID().size(); i++)
    {
        // Rechercher ce bloc parmi les blocs "match�s"
        bool bTrouve = false;
        for (unsigned int j = 0; j < ListMatchedBlockIDs.size(); j++ )
        {
            if (p_Player.GetStoppedBlocksID()[i] == ListMatchedBlockIDs[j])
            {
                 bTrouve = true;
            }
        }
        // S'il n'a pas �t� trouv�, alors on remet son multiplicateur � 0
        if (!bTrouve)
        {
            PlayerField.GetBlock( p_Player.GetStoppedBlocksID()[i] ).ResetChainID();
        }

    }

    // La liste a �t� utilis�e, on peut la vider
    ListMatchedBlockIDs.clear();
    p_Player.GetStoppedBlocksID().clear();
}


bool PanelGameBehaviour::IsABlockMatchableAt(Player& p_Player, int p_X, int p_Y, int p_SearchedColour)
{
    if ( !p_Player.GetField().isBlockAt(p_X, p_Y) )
    {
        // Un bloc n'existe pas � cette coordonn�e
        return false;
    }
    else if ( p_Player.GetField().GetBlock(p_X, p_Y).GetColor() != p_SearchedColour )
    {
        // Un bloc ne correspond pas � la bonne couleur
        return false;
    }
    else if ( p_Player.GetField().GetBlock(p_X, p_Y).GetStatus() != Block::STATUS_FIXED )
    {
        // Le bloc n'est pas disponible � l'assemblage
        return false;
    }
    else
    {
        return true;
    }
}

void PanelGameBehaviour::UpdateField(Player& p_Player, MatchSettings& p_MatchSettings)
{
    // Si le timer de r�sistance est actif, on ne bouge pas la pile
    if ( p_Player.GetField().GetFieldResistanceTimer().GetElapsed() == 0)
    {
        if ( p_Player.GetField().GetNewLineTimer().IsTimerEnd() )
        {
            p_Player.GetField().InsertNextLineInField();

            std::vector<int> tmpBlockNextLine;
            tmpBlockNextLine = p_MatchSettings.GetBlocksFromSeed(p_Player.GetIDinBlockList(), C_Field::BLOCKSPERLINE);
            p_Player.SetIDinBlockList(p_Player.GetIDinBlockList() + C_Field::BLOCKSPERLINE);
            p_Player.GetField().SetNewNextLine(tmpBlockNextLine);
            p_Player.GetField().ResetNewLineTimer();

            for (int x = 0; x < C_Field::BLOCKSPERLINE; x++)
            {
                p_Player.GetStoppedBlocksID().push_back( p_Player.GetField().GetBlockIDAt(x, 0) );
            }

            // D�placer le curseur pour suivre le bloc sur lequel il est positionn�, sauf si sa coordonn�e Y risque de passer au del� de 12
            if ( p_Player.GetCursor().GetY() < 11 )
            {
                p_Player.GetCursor().SetY( p_Player.GetCursor().GetY() + 1 );
            }

        }
    }
}

void PanelGameBehaviour::UpdatePlayerStatus(Player& p_Player)
{
    if (p_Player.GetField().GetFieldResistanceTimer().IsTimerEnd())
    {
        p_Player.SetStatus(Player::PLAYER_STAT_DEFEAT);
    }
    else if (p_Player.GetGameTimer().IsTimerEnd())
    {
        p_Player.SetStatus(Player::PLAYER_STAT_ENDTIMER);
    }
}


int PanelGameBehaviour::GetNumColors(Player& p_Player)
{
    return C_Difficulty::GetMaxColorByDifficulty( p_Player.GetDifficulty() );

}
