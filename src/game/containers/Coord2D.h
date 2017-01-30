#ifndef Coord2D_H
#define Coord2D_H


class Coord2D
{
    public:
        Coord2D();
        Coord2D(int p_X, int p_Y);
        virtual ~Coord2D();
        int GetX() { return m_X; }
        void SetX(int val) { m_X = val; }
        int GetY() { return m_Y; }
        void SetY(int val) { m_Y = val; }
        // bool operator==(Coord2D& p_OtherCoord);
    protected:
    private:
        int m_X;
        int m_Y;
};

#endif // Coord2D_H
