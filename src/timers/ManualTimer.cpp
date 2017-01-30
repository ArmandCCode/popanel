#include "ManualTimer.h"

ManualTimer::ManualTimer() : m_Elapsed(0), m_Length(0), m_Period(0), m_StartOfPeriod(0), m_PeriodPassedThisFrame(false)
{

}

ManualTimer::~ManualTimer()
{

}

void ManualTimer::UpdateTimer(unsigned long p_TimeElapsedMs)
{
    this->m_Elapsed += p_TimeElapsedMs;

    if ( this->m_Elapsed > (this->m_StartOfPeriod + this->m_Period) )
    {
        this->m_StartOfPeriod += this->m_Period;
        this->m_PeriodPassedThisFrame = true;
    }
    else
    {
        this->m_PeriodPassedThisFrame = false;
    }
}

// redémarrage du chronomètre
void ManualTimer::Restart()
{
    this->Restart(0);
}

void ManualTimer::Restart(unsigned long p_TimeOfStart) // redémarrer le chronomètre à cette valeur
{
    this->m_Elapsed = p_TimeOfStart;
}

bool ManualTimer::IsTimerEnd()
{
    if (this->m_Elapsed != 0)
    {
        return (this->m_Elapsed >= this->m_Length);
    }
    else
    {
        return false;
    }

}

unsigned long ManualTimer::GetOvertime() // elapsed - length = temps qui dépasse de la longueur du minuteur demandée
{
    if (this->m_Elapsed > this->m_Length)
        return (this->m_Elapsed - this->m_Length);
    else
        return 0;
}

float ManualTimer::GetElapsedPc()
{
    // Si on a indiqué une limite, on fait le calcul
    if ( this->m_Length > 0 )
    {
        return (float)this->m_Elapsed / (float)this->m_Length;
    }
    else
    {
        // Sinon on renvoie 100%
        return (float)1.0;
    }
}

unsigned long ManualTimer::GetRemaining()
{
    // Renvoie 0 si le temps dépasse la limite
    if ( this->m_Elapsed <= this->m_Length )
    {
        return this->m_Length - this->m_Elapsed;
    }
    else
    {
        return 0;
    }
}

float ManualTimer::GetRemainingPc() // Renvoie 0% si le chronomètre à dépassé la limite
{
    // Si on a indiqué une limite, on fait le calcul
    if ( this->m_Length > 0 )
    {
        if ( this->GetRemaining() > 0 )
        {
            return (float)this->GetRemaining() / (float)this->m_Length;
        }
        else
        {
            return (float)0.0;
        }
    }
    else
    {
        // Sinon on renvoie 0%
        return (float)0.0;
    }
}

void ManualTimer::GetElapsedInHMSMs(int& p_Hours, int& p_Minutes, int& p_Seconds, int& p_MS)
{
    p_Hours = this->m_Elapsed / 3600000 ;
    p_Minutes = (this->m_Elapsed - (p_Hours * 3600000)) / 60000;
    p_Seconds = (this->m_Elapsed - (p_Hours * 3600000) - (p_Minutes * 60000)) / 1000;
    p_MS = this->m_Elapsed - (p_Hours * 3600000) - (p_Minutes * 60000) - (p_Seconds * 1000);

    // std::cout << p_Hours << ":" << p_Minutes << ":" << p_Seconds << " et " << p_MS << " ms." << std::endl;
}

void ManualTimer::GetRemainingInHMSMs(int& p_Hours, int& p_Minutes, int& p_Seconds, int& p_MS)
{
    p_Hours = this->GetRemaining() / 3600000 ;
    p_Minutes = (this->GetRemaining() - (p_Hours * 3600000)) / 60000;
    p_Seconds = (this->GetRemaining() - (p_Hours * 3600000) - (p_Minutes * 60000)) / 1000;
    p_MS = this->GetRemaining() - (p_Hours * 3600000) - (p_Minutes * 60000) - (p_Seconds * 1000);

    // std::cout << p_Hours << ":" << p_Minutes << ":" << p_Seconds << " et " << p_MS << " ms." << std::endl;
}

void ManualTimer::GetRemainingInHMS(int& p_Hours, int& p_Minutes, int& p_Seconds) // renvoie des secondes "pleines", adapté pour un décompte
{
    int p_MS = 0;

    this->GetRemainingInHMSMs(p_Hours, p_Minutes, p_Seconds, p_MS);

    if (p_MS > 0)
    {
        // Ajouter une seconde à l'affichage
        p_Seconds++;

        // Si on dépasse la minute
        if (p_Seconds == 60)
        {
            p_Seconds = 0;
            p_Minutes++;

            // Si on dépasse l'heure
            if (p_Minutes == 60)
            {
                p_Minutes = 0;
                p_Hours++;
            }
        }
    }
}

void ManualTimer::SetPeriod(unsigned long p_Ms)
{
    m_Period = p_Ms;
    m_StartOfPeriod = this->GetElapsed();
}

bool ManualTimer::HasPeriodPassed()
{
    return this->m_PeriodPassedThisFrame;
}

