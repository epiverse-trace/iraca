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
  int getIncubationPeriod() { return incubationPeriod; }
  int fetInfectionDuration() { return infectionDuration; }
  float getInitInfectedHumans() { return initInfectedHumans; }
  float getInitInfectedMosquitoes() { return initInfectedMosquitoes; }

  Simulation(int, const std::vector<std::vector<float>>&,
             const std::vector<std::vector<float>>&,
             const std::vector<std::vector<float>>&, int, int, float, float);

  ~Simulation();

  void initialize();
  Rcpp::DataFrame simulate();
  // void simulate(int);
  void moveHumans();

 private:
  int days;
  std::vector<std::vector<float>> territoriesData;
  std::vector<std::vector<float>> temperatureData;
  std::vector<std::vector<float>> movementData;
  std::map<int, Territory> territories;
  std::list<Human> transitHumans;
  int incubationPeriod;
  int infectionDuration;
  float initInfectedHumans;
  float initInfectedMosquitoes;
};

#endif  // SRC_SIMULATION_H_
