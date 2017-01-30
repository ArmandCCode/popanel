#ifndef MANUALTIMER_H
#define MANUALTIMER_H

#include <iostream>
#include <cmath>

class ManualTimer
{
    public:
        ManualTimer();
        virtual ~ManualTimer();

        void UpdateTimer(unsigned long p_TimeElapsedMs);

        // redémarrage du chronomètre
        void Restart();
        void Restart(unsigned long p_TimeOfStart); // redémarrer le chronomètre à cette valeur

        // temps écoulé
        unsigned long GetElapsed() { return m_Elapsed; }
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

        // Indicateur "une certaine période de temps est passée durant la dernière mise à jour"
        // Permet de définir des évènements chaque seconde par exemple
        void SetPeriod(unsigned long p_Ms);
        unsigned long GetPeriod() { return m_Period; }
        bool HasPeriodPassed();

    protected:
    private:
        // membres
        // Mode chronomètre
        unsigned long m_Elapsed;

        // Mode minuteur
        unsigned long m_Length;

        // Indicateur "une certaine période de temps est passée durant la dernière mise à jour"
        // Permet de définir des évènements chaque seconde par exemple
        // PS : le fonctionnement actuel ne supporte pas de changement en plein milieu (n'a pas vocation à l'être !)
        unsigned long m_Period;
        unsigned long m_StartOfPeriod;
        bool m_PeriodPassedThisFrame;
};

#endif // MANUALTIMER_H
