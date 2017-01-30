#ifndef NUMHELPER_H
#define NUMHELPER_H

#include <cstdlib>
#include <vector>

namespace NumHelper
{

    inline unsigned long RandomInt( unsigned long p_min, unsigned long p_max )
    {
        // Retourne un entier positif aléatoire entre les deux valeurs indiquées en paramètre
        return (rand() % (p_max) + p_min);
    }

    inline void RandomizeSeed(unsigned long p_NewSeed)
    {
        srand(p_NewSeed);
    }

}

#endif
