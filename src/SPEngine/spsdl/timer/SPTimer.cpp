#include "SPTimer.h"

SPTimer::SPTimer()
{
    this->m_StartOfTimer = SDL_GetTicks();
    this->m_isPaused = false;
}

SPTimer::~SPTimer()
{

}

// redémarrage du chronomètre
void SPTimer::Restart()
{
    this->Restart(0);
}

void SPTimer::Restart(unsigned long p_TimeOfStart) // redémarrer le chronomètre à cette valeur
{
    this->m_StartOfTimer = SDL_GetTicks() - p_TimeOfStart;
}

unsigned long SPTimer::GetElapsed()
{
    // Si le chrono est en pause, on envoie un résultat différent
    if ( this->isTimerPaused() )
    {
        return m_TimeOfPause - m_StartOfTimer;
    }
    else
    {
        return SDL_GetTicks() - m_StartOfTimer;
    }
}

bool SPTimer::IsTimerEnd()
{
    if (this->GetElapsed() != 0)
    {
        return (this->GetElapsed() >= this->m_Length);
    }
    else
    {
        return false;
    }

}

unsigned long SPTimer::GetOvertime() // elapsed - length = temps qui dépasse de la longueur du minuteur demandée
{
    if (this->GetElapsed() > this->m_Length)
        return (this->GetElapsed() - this->m_Length);
    else
        return 0;
}

float SPTimer::GetElapsedPc()
{
    // Si on a indiqué une limite, on fait le calcul
    if ( this->m_Length > 0 )
    {
        return (float)this->GetElapsed() / (float)this->m_Length;
    }
    else
    {
        // Sinon on renvoie 100%
        return (float)1.0;
    }
}

unsigned long SPTimer::GetRemaining()
{
    // Renvoie 0 si le temps dépasse la limite
    if ( this->GetElapsed() <= this->m_Length )
    {
        return this->m_Length - this->GetElapsed();
    }
    else
    {
        return 0;
    }
}

float SPTimer::GetRemainingPc() // Renvoie 0% si le chronomètre à dépassé la limite
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

void SPTimer::GetElapsedInHMSMs(int& p_Hours, int& p_Minutes, int& p_Seconds, int& p_MS)
{
    p_Hours = this->GetElapsed() / 3600000 ;
    p_Minutes = (this->GetElapsed() - (p_Hours * 3600000)) / 60000;
    p_Seconds = (this->GetElapsed() - (p_Hours * 3600000) - (p_Minutes * 60000)) / 1000;
    p_MS = this->GetElapsed() - (p_Hours * 3600000) - (p_Minutes * 60000) - (p_Seconds * 1000);

    // std::cout << p_Hours << ":" << p_Minutes << ":" << p_Seconds << " et " << p_MS << " ms." << std::endl;
}

void SPTimer::GetRemainingInHMSMs(int& p_Hours, int& p_Minutes, int& p_Seconds, int& p_MS)
{
    p_Hours = this->GetRemaining() / 3600000 ;
    p_Minutes = (this->GetRemaining() - (p_Hours * 3600000)) / 60000;
    p_Seconds = (this->GetRemaining() - (p_Hours * 3600000) - (p_Minutes * 60000)) / 1000;
    p_MS = this->GetRemaining() - (p_Hours * 3600000) - (p_Minutes * 60000) - (p_Seconds * 1000);

    // std::cout << p_Hours << ":" << p_Minutes << ":" << p_Seconds << " et " << p_MS << " ms." << std::endl;
}

void SPTimer::GetRemainingInHMS(int& p_Hours, int& p_Minutes, int& p_Seconds) // renvoie des secondes "pleines", adapté pour un décompte
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

void SPTimer::PauseTimer()
{
    // Ne pas mettre en pause deux fois de suite
    if ( !this->isTimerPaused() )
    {
        // indiquer le moment ou la pause a été mise
        this->m_TimeOfPause = SDL_GetTicks();
        this->m_isPaused = true;
    }

}

void SPTimer::ResumeTimer()
{
    // Ne pas rétablir deux fois de suite
    if ( this->isTimerPaused() )
    {
        // Décaler le point de départ du chronomètre pour ratrapper le temps passé en pause
        this->m_StartOfTimer += SDL_GetTicks() - this->m_TimeOfPause;
        this->m_isPaused = false;
    }
}
