#ifndef NOISEGENERATOR_HPP_INCLUDED
#define NOISEGENERATOR_HPP_INCLUDED

#include "Constants.hpp"
#include <vector>


class NoiseGenerator
{
private:
    int outputSize = MAP_BLOCK_WIDTH;
    std::vector<float> seed;
public:
    std::vector<float> perlinNoise;

    NoiseGenerator();
    void generatePerlinNoise(int nOctaves);
};

#endif // NOISEGENERATOR_HPP_INCLUDED
