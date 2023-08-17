// Copyright 2023 `iraca` authors.
#ifndef SRC_SIMULATION_H_
#define SRC_SIMULATION_H_

// Simulation Class

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

class Simulation {
 public:
  int getDays() { return days; }
  std::map<int, Territory> getTerritories() { return territories; }
  std::vector<std::vector<double>> getTerritoriesData() {
    return territoriesData;
  }
  std::vector<std::vector<double>> getTemperatureData() {
    return temperatureData;
  }
  std::vector<std::vector<double>> getMovementData() { return movementData; }
  int getIncubationPeriod() { return incubationPeriod; }
  int fetInfectionDuration() { return infectionDuration; }
  double getInitInfectedHumans() { return initInfectedHumans; }
  double getInitInfectedMosquitoes() { return initInfectedMosquitoes; }

  Simulation(int, const std::vector<std::vector<double>>&,
             const std::vector<std::vector<double>>&,
             const std::vector<std::vector<double>>&, int, int, double, double);

  ~Simulation();

  void initialize();
  Rcpp::DataFrame simulate();
  void moveHumans();

 private:
  int days;
  std::vector<std::vector<double>> territoriesData;
  std::vector<std::vector<double>> temperatureData;
  std::vector<std::vector<double>> movementData;
  std::map<int, Territory> territories;
  std::list<Human> transitHumans;
  int incubationPeriod;
  int infectionDuration;
  double initInfectedHumans;
  double initInfectedMosquitoes;
};

#endif  // SRC_SIMULATION_H_
