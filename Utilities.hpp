#ifndef UTILITIES_HPP_INCLUDED
#define UTILITIES_HPP_INCLUDED

#include <random>

inline int randomInt(int min, int max)
{
    return ( min + ( std::rand() % ( max - min + 1 ) ) );
}

#endif // UTILITIES_HPP_INCLUDED
