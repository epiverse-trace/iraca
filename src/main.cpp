// Copyright 2023 `iraca` authors.

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "Simulation.h"
// #include "Simulation.cpp"

std::vector<std::vector<float>> generalData(std::string fileName) {
  std::ifstream data(fileName);
  std::string line;
  std::vector<std::vector<std::string>> readData;
  while (std::getline(data, line)) {
    std::stringstream lineStream(line);
    std::string cell;
    std::vector<std::string> observation;
    while (std::getline(lineStream, cell, ',')) {
      observation.push_back(cell);
    }
    readData.push_back(observation);
  }
  readData.erase(readData.begin());
  std::vector<std::vector<float>> readDataOutput = {};
  int nreadData = readData.size();
  for (int i = 0; i < nreadData; i++) {
    std::vector<float> temp_vec = {};
    int nreadDataI = readData[i].size();
    for (int j = 0; j < nreadDataI; j++) {
      float temp = std::stof(readData[i][j]);
      temp_vec.push_back(temp);
    }
    readDataOutput.push_back(temp_vec);
  }
  return readDataOutput;
}

//' @title run simulation
//' @export
// [[Rcpp::export]]
int main() {
  int nDays = 10;
  std::vector<std::vector<float>> ibague_data =
      generalData("C:/Users/Julia/Downloads/temp_data/sectores_ibague.csv");
  std::vector<std::vector<float>> temperature =
      generalData("C:/Users/Julia/Downloads/temp_data/temperature_ibague.csv");
  std::vector<std::vector<float>> flow = generalData(
      "C:/Users/Julia/Downloads/temp_data/flow_sectores_ibague.csv");

  Simulation simulation(nDays, ibague_data, temperature, flow);
  simulation.simulate(nDays);
  return 0;
}
