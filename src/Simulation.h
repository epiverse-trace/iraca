// Copyright 2023 `iraca` authors.
#ifndef SRC_SIMULATION_H_
#define SRC_SIMULATION_H_

#include <Rcpp.h>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "Territory.h"
// #include "Territory.cpp"

class Simulation {
 public:
  int getDays() { return days; }
  std::map<int, Territory> getTerritories() { return territories; }
  std::vector<std::vector<float>> getTerritoriesData() {
    return territoriesData;
  }
  std::vector<std::vector<float>> getTemperatureData() {
    return temperatureData;
  }
  std::vector<std::vector<float>> getMovementData() { return movementData; }

  Simulation(int, const std::vector<std::vector<float>>&,
             const std::vector<std::vector<float>>&,
             const std::vector<std::vector<float>>&);

  ~Simulation();

  void initialize();
  Rcpp::DataFrame simulate(int);
  // void simulate(int);
  void moveHumans();

 private:
  int days;
  std::map<int, Territory> territories;
  std::vector<std::vector<float>> territoriesData;
  std::vector<std::vector<float>> temperatureData;
  std::vector<std::vector<float>> movementData;
  std::list<Human> transitHumans;
};

#endif  // SRC_SIMULATION_H_
