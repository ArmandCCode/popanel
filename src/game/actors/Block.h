#ifndef BLOCK_H
#define BLOCK_H

#include "timers/ManualTimer.h"

class Block
{
    public:
        Block(int p_x, int p_y, int p_Color);
        virtual ~Block();

        static const int COLOR_NO = 0;
        static const int COLOR_1 = 1;
        static const int COLOR_2 = 2;
        static const int COLOR_3 = 3;
        static const int COLOR_4 = 4;
        static const int COLOR_5 = 5;
        static const int COLOR_6 = 6;

        static const int STATUS_FIXED = 0;
        static const int STATUS_SWITCHING = 1;
        static const int STATUS_SUSPEND = 2;
        static const int STATUS_FALLING = 3;
        static const int STATUS_POPPING = 4;

        // Coordonnées actuelles
        int GetX() { return m_X; }
        void SetX(int val) { m_X = val; }
        int GetY() { return m_Y; }
        void SetY(int val) { m_Y = val; }

        // Coordonnées de destination
        int GetXDest() { return m_XDest; }
        void SetXDest(int val) { m_XDest = val; }
        int GetYDest() { return m_YDest; }
        void SetYDest(int val) { m_YDest = val; }

        // Couleur du bloc
        int GetColor() { return m_Color; }
        void SetColor(int val) { m_Color = val; }

        // Statut et temps avant changement
        void SetStatus(int p_Status, int p_Time);
        int GetStatus() { return m_BlockStatus; }
        ManualTimer& GetTimer() { return m_Timer; }

        // Déplacement vers la destination
        void MoveToDest();

        // ID de multiplicateur de chaîne
        int GetChainID() { return m_ChainID; }
        void SetChainID(int val) { m_ChainID = val; }
        void ResetChainID() { m_ChainID = -1; } // Facilite la lecture du code

    protected:
    private:
        // Coordonnées actuelles
        int m_X;
        int m_Y;
        // Coordonnées de destination
        int m_XDest;
        int m_YDest;

        // Couleur du bloc
        int m_Color;

        // Statut et temps avant changement
        int m_BlockStatus;
        ManualTimer m_Timer;

        // ID du multiplicateur de chaîne
        int m_ChainID;
};

#endif // BLOCK_H
