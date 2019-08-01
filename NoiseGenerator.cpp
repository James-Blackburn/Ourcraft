#include "NoiseGenerator.hpp"
#include <cstdlib>

NoiseGenerator::NoiseGenerator()
{
    seed = new float[outputSize];
    for (int i=0; i<outputSize; i++)
    {
        seed[i] = rand() / (float)RAND_MAX;
    }
    perlinNoise = new float[outputSize];
}

NoiseGenerator::~NoiseGenerator()
{
    delete[] seed;
    delete[] perlinNoise;
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
}
