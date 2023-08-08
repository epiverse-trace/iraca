// Copyright 2023 `iraca` authors.

#include <cstdlib>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include "Simulation.h"
// #include "Simulation.cpp"

// Read data file of Ibague's sectores
std::vector<std::vector<float>> generalData(std::string fileName)
{
    std::ifstream data(fileName);
    std::string line;
    std::vector<std::vector<std::string>> moveData;
    while (std::getline(data, line))
    {
        std::stringstream lineStream(line);
        std::string cell;
        std::vector<std::string> observation;
        while (std::getline(lineStream, cell, ','))
        {
            observation.push_back(cell);
        }
        moveData.push_back(observation);
    }
    moveData.erase(moveData.begin());
    std::vector<std::vector<float>> moveDataOutput = {};
    for (int i = 0; i < int(moveData.size()); i++)
    {
        std::vector<float> temp_vec = {};
        for (int j = 0; j < int(moveData[i].size()); j++)
        {
            float temp = std::stof(moveData[i][j]);
            temp_vec.push_back(temp);
        }
        moveDataOutput.push_back(temp_vec);
    }
    return moveDataOutput;
};

//' @title run simulation
//' @export
// [[Rcpp::export]]
int main()
{
    int nDays = 10;
    std::vector<std::vector<float>> ibague_data = generalData("C:/Users/Julia/Downloads/temp_data/sectores_ibague.csv");
    std::vector<std::vector<float>> temperature = generalData("C:/Users/Julia/Downloads/temp_data/temperature_ibague.csv");
    std::vector<std::vector<float>> flow = generalData("C:/Users/Julia/Downloads/temp_data/flow_sectores_ibague.csv");

    Simulation simulation(nDays, ibague_data, temperature, flow);
    simulation.simulate(nDays);
    return 0;
};