#include "Player.h"

Player::Player() : m_Cursor(C_Field::BLOCKSPERLINE, C_Field::LINESPERFIELD), m_Difficulty(1), m_GameStatus(0), m_Score(0)
{

    int tmpLength = 0;
    switch (this->m_Difficulty)
    {
    case 1:
        tmpLength = 2250;
        break;
    case 2:
        tmpLength = 1500;
        break;
    default:
        tmpLength = 3000;
        break;
    }

    this->GetField().ResetNewLineTimer();
    this->GetField().GetFieldResistanceTimer().SetLength(tmpLength);
    this->GetField().GetFieldResistanceTimer().Restart();
    this->GetField().ResetStopTime();

}

Player::~Player()
{

}

void Player::AwardPoints(long p_PointsAwarded)
{
    this->m_Score += p_PointsAwarded;
}

bool Player::isInPanicMode()
{
    // Just check if there is at least one block on the 3 top lines
    for (int y = (C_Field::LINESPERFIELD - 1); y >= (C_Field::LINESPERFIELD - 3); y--)
    {
        for (int x = 0; x < C_Field::BLOCKSPERLINE; x++)
        {
            if (this->GetField().isBlockAt(x, y))
            {
                return true;
            }
        }
    }

    return false;
}
