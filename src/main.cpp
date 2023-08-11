// Copyright 2023 `iraca` authors.

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "Simulation.h"
// #include "Simulation.cpp"

// std::vector<std::vector<float>> generalData(std::string fileName) {
//   std::ifstream data(fileName);
//   std::string line;
//   std::vector<std::vector<std::string>> readData;
//   while (std::getline(data, line)) {
//     std::stringstream lineStream(line);
//     std::string cell;
//     std::vector<std::string> observation;
//     while (std::getline(lineStream, cell, ',')) {
//       observation.push_back(cell);
//     }
//     readData.push_back(observation);
//   }
//   readData.erase(readData.begin());
//   std::vector<std::vector<float>> readDataOutput = {};
//   int nreadData = readData.size();
//   for (int i = 0; i < nreadData; i++) {
//     std::vector<float> temp_vec = {};
//     int nreadDataI = readData[i].size();
//     for (int j = 0; j < nreadDataI; j++) {
//       float temp = std::stof(readData[i][j]);
//       temp_vec.push_back(temp);
//     }
//     readDataOutput.push_back(temp_vec);
//   }
//   return readDataOutput;
// }

//' @title run simulation
//' @param nDays Number of days to simulate.
//' @param demographicData Data frame containing the Demographic data for the
//' synthetic population.
//' @param temperatureData Data frame with the temperature historic.
//' @param movementData Data frame with the movement flow between territories.
//' @param incubationPeriod Incubation period of the disease.
//' @param infectionDuration Infection duration of the disease.
//' @param initInfectedHumans Percentage of initially infected humans.
//' @param initInfectedMosquitoes Percentage of initially infected mosquitoes.
//' @export
// [[Rcpp::export]]
Rcpp::DataFrame simulate(int nDays,
                         std::vector<std::vector<float>> demographicData,
                         std::vector<std::vector<float>> temperatureData,
                         std::vector<std::vector<float>> movementData,
                         int incubationPeriod, int infectionDuration,
                         float initInfectedHumans,
                         float initInfectedMosquitoes) {
  Simulation simulation(nDays, demographicData, temperatureData, movementData,
                        incubationPeriod, infectionDuration, initInfectedHumans,
                        initInfectedMosquitoes);
  simulation.simulate();
  return 0;
}

int main() { return 0; }
