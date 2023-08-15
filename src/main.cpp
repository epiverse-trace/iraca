// Copyright 2023 `iraca` authors.

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "Simulation.h"
// #include "Simulation.cpp"

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
Rcpp::DataFrame simulate(int nDays, Rcpp::DataFrame demographicData,
                         Rcpp::DataFrame temperatureData,
                         Rcpp::DataFrame movementData, int incubationPeriod,
                         int infectionDuration, double initInfectedHumans,
                         double initInfectedMosquitoes) {
  std::vector<std::vector<double>> _demographicData =
      Rcpp::as<std::vector<std::vector<double>>>(demographicData);
  std::vector<std::vector<double>> _temperatureData =
      Rcpp::as<std::vector<std::vector<double>>>(temperatureData);
  std::vector<std::vector<double>> _movementData =
      Rcpp::as<std::vector<std::vector<double>>>(movementData);

  Simulation simulation(nDays, _demographicData, _temperatureData,
                        _movementData, incubationPeriod, infectionDuration,
                        initInfectedHumans, initInfectedMosquitoes);
  Rcpp::Rcout << "Simulation object created" << std::endl;
  simulation.simulate();
  return 0;
}

int main() { return 0; }
