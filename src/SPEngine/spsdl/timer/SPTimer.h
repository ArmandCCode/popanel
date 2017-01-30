#ifndef SPTIMER_H
#define SPTIMER_H

#include "SPEngine/spsdl/SPSDL.h"

class SPTimer
{
    public:
        SPTimer();
        virtual ~SPTimer();

        // redémarrage du chronomètre
        void Restart();
        void Restart(unsigned long p_TimeOfStart); // redémarrer le chronomètre à cette valeur

        // temps écoulé
        unsigned long GetElapsed();
        float GetElapsedPc(); // Renvoie 100% si pas de longueur assignée
        unsigned long GetRemaining();
        float GetRemainingPc(); // Renvoie 0% si le chronomètre à dépassé la limite

        // Minuteur
        void SetLength(unsigned long p_Ms) { m_Length = p_Ms; }
        unsigned long GetLength() { return m_Length; }
        bool IsTimerEnd();
        unsigned long GetOvertime(); // elapsed - length = temps qui dépasse de la longueur du minuteur demandée

        // Conversion en heures, minutes, secondes
        void GetElapsedInHMSMs(int& p_Hours, int& p_Minutes, int& p_Seconds, int& p_MS);
        void GetRemainingInHMSMs(int& p_Hours, int& p_Minutes, int& p_Seconds, int& p_MS);
        void GetRemainingInHMS(int& p_Hours, int& p_Minutes, int& p_Seconds); // renvoie des secondes "pleines", adapté pour un décompte

        // Mise en pause du chronomètre
        void PauseTimer();
        void ResumeTimer();
        bool isTimerPaused() { return m_isPaused; }

    protected:
    private:
        // membres
        // Mode chronomètre
        unsigned long m_StartOfTimer;
        unsigned long m_Elapsed;

        // Mode minuteur
        unsigned long m_Length;

        // Mise en pause du chronomètre
        bool m_isPaused;
        unsigned long m_TimeOfPause;
};

#endif // SPTIMER_H
