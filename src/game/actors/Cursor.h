#ifndef CURSOR_H
#define CURSOR_H


class Cursor
{
    public:
        Cursor(unsigned int m_XBoundary, unsigned int p_YBoundary);
        virtual ~Cursor();
        int GetX() { return m_X; }
        void SetX(int val) { m_X = val; }
        int GetY() { return m_Y; }
        void SetY(int val) { m_Y = val; }

        // absolute positionning
        bool Move(unsigned int p_X, unsigned int p_Y);

        // relative positionning
        bool GoUp();
        bool GoDown();
        bool GoLeft();
        bool GoRight();

    protected:
    private:
        int m_X;
        int m_Y;

        unsigned int m_XBoundary;
        unsigned int m_YBoundary;

        // A améliorer plus tard
        static const unsigned int m_CursorWidth = 2;
};

#endif // CURSOR_H
