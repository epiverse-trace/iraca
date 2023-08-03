
#include <cstdlib>
#include <iostream>
#include <vector>
#include <Rcpp.h>

int weightedRandom(std::vector<float> probabilities)
{
    int output = 0;
    std::vector<int> transformedP;
    int accumulated = 0;
    for (float i : probabilities)
    {
        int transformed = int(i * 1000) + accumulated;
        transformedP.push_back(transformed);
        accumulated = transformed;
    }
    int randomInteger = Rcpp::sample(1000, 1)[0];
    // int randomInteger = rand() % 100;
    for (int i = 0; i < int(probabilities.size()); i++)
    {
        if (randomInteger < transformedP[i])
        {
            output = i;
            break;
        }
    }
    return output;
};