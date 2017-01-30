#include "Cursor.h"

Cursor::Cursor(unsigned int p_XBoundary, unsigned int p_YBoundary) : m_X(0), m_Y(0), m_XBoundary(p_XBoundary), m_YBoundary(p_YBoundary)
{

}

Cursor::~Cursor()
{

}

// absolute positionning
bool Cursor::Move(unsigned int p_X, unsigned int p_Y)
{
    if (p_X >= 0 && p_Y >= 0 && p_X < m_XBoundary - (m_CursorWidth - 1) && p_Y < m_YBoundary)
    {
        // The cursor (width included) is inside the boundaries
        this->SetX(p_X);
        this->SetY(p_Y);
        return true;
    }
    else
    {
        return false;
    }
}

// relative positionning
bool Cursor::GoUp()
{
    return this->Move(this->GetX(), this->GetY() + 1);
}

bool Cursor::GoDown()
{
    return this->Move(this->GetX(), this->GetY() - 1);
}

bool Cursor::GoLeft()
{
    return this->Move(this->GetX() - 1, this->GetY());
}

bool Cursor::GoRight()
{
    return this->Move(this->GetX() + 1, this->GetY());
}
