#include "NoiseGenerator.hpp"
#include <cstdlib>
#include <ctime>

#include <iostream>

NoiseGenerator::NoiseGenerator()
    : seed(outputSize), perlinNoise(outputSize)
{
    // seed generator
    srand(time(NULL));
    for (int i=0; i<outputSize; i++)
    {
        seed[i] = rand() / static_cast<float>(RAND_MAX);
    }
}

void NoiseGenerator::generatePerlinNoise(int nOctaves)
{
    for (int i=0; i<outputSize; i++)
    {
        float noise = 0.0f;
        float scale = 1.0f;
        float scaleAccumulator = 0.0f;

        for (int o=0; o<nOctaves; o++)
        {
            int nPitch = outputSize >> o;
            int sample1 = (i / nPitch) * nPitch;
            int sample2 = (sample1 + nPitch) % outputSize;

            float fBlend = static_cast<float>(i - sample1) / static_cast<float>(nPitch);
            float fSample = (1.0f - fBlend) * seed[sample1] + fBlend * seed[sample2];
            noise += fSample * scale;
            scaleAccumulator += scale;
            scale = scale / 2.0f;
        }

        perlinNoise[i] = noise / scaleAccumulator;
    }

    double smallest = perlinNoise[0] ;
    for ( int i=1;  i < outputSize;  i++ )
        if ( perlinNoise[i] < smallest )
             smallest = perlinNoise[i] ;
    std::cout << smallest << '\n' ;

    double biggest = perlinNoise[0] ;
    for ( int i=1;  i < outputSize;  i++ )
        if ( perlinNoise[i] > biggest )
             biggest = perlinNoise[i] ;
    std::cout << biggest << '\n' ;

}
