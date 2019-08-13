#ifndef NOISEGENERATOR_HPP_INCLUDED
#define NOISEGENERATOR_HPP_INCLUDED

#include "Constants.hpp"


class NoiseGenerator
{
private:
    float* seed = nullptr;
    int outputSize = MAP_BLOCK_WIDTH;
public:
    float* perlinNoise = nullptr;

    NoiseGenerator();
    ~NoiseGenerator();
    void generatePerlinNoise(int nOctaves);
};

#endif // NOISEGENERATOR_HPP_INCLUDED
