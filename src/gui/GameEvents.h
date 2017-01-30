#ifndef GameEvents_H
#define GameEvents_H

#include <queue>
#include "SPEngine/SPException.h"
// Cette classe permet de r�pertorier les �v�nements importants ayant eu lieu dans chaque mise � jour du jeu
// afin de modifier les graphismes et les sons en cons�quence (changement de musique, dessin d'un graphisme particulier)

class GameEvents
{
    public:
        GameEvents();
        virtual ~GameEvents();

        enum {EVT_COUNTDOWN, EVT_COUNTEND, EVT_CURSORMOVE, EVT_PANELMOVE, EVT_PANELBUMP, EVT_PANELPOP, EVT_COMBOCHAIN, EVT_DEFEAT};

        void AddEvent(int p_IDGameEvent);

        bool isQueueEmpty();
        int& GetCurrentEvent();
        void StepToNextEvent();

    protected:
    private:
        std::queue<int> m_EventList;
};

#endif // GameEvents_H
