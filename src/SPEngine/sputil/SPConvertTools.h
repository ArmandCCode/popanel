#ifndef CONVERTTOOLS_H
#define CONVERTTOOLS_H

#include <string>
#include <sstream>

// Merci à cMinor pour l'astuce
// http://stackoverflow.com/a/20861692
namespace SPConv
{
    template < typename T > std::string NumToStr( const T& p_Num )
    {
        std::ostringstream stm ;
        stm << p_Num ;
        return stm.str() ;
    }
}

#endif
