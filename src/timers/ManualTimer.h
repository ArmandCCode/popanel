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

        // red�marrage du chronom�tre
        void Restart();
        void Restart(unsigned long p_TimeOfStart); // red�marrer le chronom�tre � cette valeur

        // temps �coul�
        unsigned long GetElapsed() { return m_Elapsed; }
        float GetElapsedPc(); // Renvoie 100% si pas de longueur assign�e
        unsigned long GetRemaining();
        float GetRemainingPc(); // Renvoie 0% si le chronom�tre � d�pass� la limite

        // Minuteur
        void SetLength(unsigned long p_Ms) { m_Length = p_Ms; }
        unsigned long GetLength() { return m_Length; }
        bool IsTimerEnd();
        unsigned long GetOvertime(); // elapsed - length = temps qui d�passe de la longueur du minuteur demand�e

        // Conversion en heures, minutes, secondes
        void GetElapsedInHMSMs(int& p_Hours, int& p_Minutes, int& p_Seconds, int& p_MS);
        void GetRemainingInHMSMs(int& p_Hours, int& p_Minutes, int& p_Seconds, int& p_MS);
        void GetRemainingInHMS(int& p_Hours, int& p_Minutes, int& p_Seconds); // renvoie des secondes "pleines", adapt� pour un d�compte

        // Indicateur "une certaine p�riode de temps est pass�e durant la derni�re mise � jour"
        // Permet de d�finir des �v�nements chaque seconde par exemple
        void SetPeriod(unsigned long p_Ms);
        unsigned long GetPeriod() { return m_Period; }
        bool HasPeriodPassed();

    protected:
    private:
        // membres
        // Mode chronom�tre
        unsigned long m_Elapsed;

        // Mode minuteur
        unsigned long m_Length;

        // Indicateur "une certaine p�riode de temps est pass�e durant la derni�re mise � jour"
        // Permet de d�finir des �v�nements chaque seconde par exemple
        // PS : le fonctionnement actuel ne supporte pas de changement en plein milieu (n'a pas vocation � l'�tre !)
        unsigned long m_Period;
        unsigned long m_StartOfPeriod;
        bool m_PeriodPassedThisFrame;
};

#endif // MANUALTIMER_H
