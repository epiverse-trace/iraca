// Copyright 2023 `iraca` authors.

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "Simulation.h"

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
//' @param geometries Geometries as text.
//' @return Dataframe with susceptible, infected an recovered individuals each
//' day
//' @keywords internal
// [[Rcpp::export]]
Rcpp::DataFrame internal_simulation_cpp(
    int nDays, Rcpp::DataFrame demographicData, Rcpp::DataFrame temperatureData,
    Rcpp::DataFrame movementData, int incubationPeriod, int infectionDuration,
    double initInfectedHumans, double initInfectedMosquitoes,
    Rcpp::StringVector geometries) {
  std::vector<std::vector<double>> _demographicData =
      Rcpp::as<std::vector<std::vector<double>>>(demographicData);
  std::vector<std::vector<double>> _temperatureData =
      Rcpp::as<std::vector<std::vector<double>>>(temperatureData);
  std::vector<std::vector<double>> _movementData =
      Rcpp::as<std::vector<std::vector<double>>>(movementData);
  std::vector<std::string> _geometries(geometries.size());
  const int n_geometries = geometries.size();
  for (int i = 0; i < n_geometries; i++) {
    _geometries[i] = (geometries(i));
  }
  std::cout << "here" << std::endl;
  Simulation simulation(nDays, _demographicData, _temperatureData,
                        _movementData, incubationPeriod, infectionDuration,
                        initInfectedHumans, initInfectedMosquitoes,
                        _geometries);
  Rcpp::DataFrame output = simulation.simulate();
  return output;
}

