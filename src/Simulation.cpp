// Copyright 2023 `iraca` authors.

#include "Simulation.h"

Simulation::Simulation(int _days,
                       const std::vector<std::vector<double>> &_territoriesData,
                       const std::vector<std::vector<double>> &_temperatureData,
                       const std::vector<std::vector<double>> &_movementData,
                       int _incubationPeriod, int _infectionDuration,
                       double _initInfectedHumans,
                       double _initInfectedMosquitoes)
    : territoriesData(_territoriesData),
      temperatureData(_temperatureData),
      movementData(_movementData),
      territories({}),
      transitHumans({}) {
  days = _days;
  incubationPeriod = _incubationPeriod;
  infectionDuration = _infectionDuration;
  initInfectedHumans = _initInfectedHumans;
  initInfectedMosquitoes = _initInfectedMosquitoes;
}

Simulation::~Simulation() {}

void Simulation::initialize() {
  // data input
  int nTerritories = territoriesData[0].size();
  for (int ter = 0; ter < nTerritories - 1; ter++) {
    Rcpp::Rcout << "teritory " << std::to_string(ter) << std::endl;
    // Environment attributes from DANE database
    double density = territoriesData[19][ter];
    int population = territoriesData[7][ter];
    double gas = territoriesData[6][ter];
    double sewage = territoriesData[5][ter];
    double area = territoriesData[1][ter];
    std::vector<double> ageProp = {
        territoriesData[10][ter], territoriesData[11][ter],
        territoriesData[12][ter], territoriesData[13][ter],
        territoriesData[14][ter], territoriesData[15][ter],
        territoriesData[16][ter], territoriesData[17][ter],
        territoriesData[18][ter]};
    double maleProp = territoriesData[8][ter];
    double highEdProp = territoriesData[20][ter];
    std::vector<double> movementPatterns = movementData[ter];
    // Territory creation
    Territory newTerritory(ter, area, density, population, ageProp, maleProp,
                           highEdProp, gas, sewage, movementPatterns);
    // Internal intialization
    newTerritory.initializeHumans(initInfectedHumans, incubationPeriod,
                                  infectionDuration);
    newTerritory.initializeMosquitoes(
        static_cast<int>(population * 1.3 * 2.1 / 2.7), initInfectedMosquitoes,
        false);

    // Updating list
    territories.insert(std::pair<int, Territory>(ter, newTerritory));
  }
}

Rcpp::DataFrame Simulation::simulate() {
  initialize();
  std::vector<int> vectorS = {};
  std::vector<int> vectorI = {};
  std::vector<int> vectorR = {};
  for (int day = 0; day < days; day++) {
    Rcpp::Rcout << "Day " << std::to_string(day) << std::endl;
    int suceptible = 0;
    int infected = 0;
    int recovered = 0;
    if (day == 0) {
      for (auto &territory : territories) {
        Territory *tempTerritory = &territory.second;
        for (auto &human : tempTerritory->humans) {
          int dailyEnv = human.getDailyTerritory();
          int _positionX =
              static_cast<int>(R::runif(0, territories[dailyEnv].getLength()));
          int _positionY =
              static_cast<int>(R::runif(0, territories[dailyEnv].getWidth()));
          human.setDailyCoordinates(_positionX, _positionY);  // RETHINK THIS
        }
      }
    }
    for (int time = 0; time < 2; time++) {
      // paralelizable
      for (auto &territory : territories) {
        Territory *tempTerritory = &territory.second;
        tempTerritory->interaction(
            day);  //, temperatureData[0, day], temperatureData[1, day]);
      }
      for (auto &territory : territories) {
        Territory *tempTerritory = &territory.second;
        // Move mosquitoes and humans
        tempTerritory->moveMosquitoes();
        tempTerritory->moveHumans();
        // Assign to transitHumans
        transitHumans.insert(transitHumans.end(), tempTerritory->humans.begin(),
                             tempTerritory->humans.end());
        tempTerritory->resetHumans();
      }
      // Repopulate environments
      for (auto human : transitHumans) {
        territories[human.getCurrentTerritory()].addHuman(human);
      }
      transitHumans.clear();
    }
    for (auto &territory : territories) {
      Territory *tempTerritory = &territory.second;
      std::vector<int> localSIR = tempTerritory->contagions();
      suceptible += localSIR[0];
      infected += localSIR[1];
      recovered += localSIR[2];
      tempTerritory->updateMosquitoes();
      tempTerritory->updateHumans(day);
      tempTerritory->deathMosquitoes(temperatureData[0][day],
                                     temperatureData[1][day]);
      tempTerritory->birthMosquitoes(temperatureData[0][day],
                                     temperatureData[1][day]);
    }
    Rcpp::Rcout << "suceptible " << suceptible << std::endl;
    Rcpp::Rcout << "infected " << infected << std::endl;
    Rcpp::Rcout << "recovered " << recovered << std::endl;
    vectorS.push_back(suceptible);
    vectorI.push_back(infected);
    vectorR.push_back(recovered);
    Rcpp::Rcout << "Done with day " << std::to_string(day) << std::endl;
  }
  Rcpp::DataFrame SIRM = Rcpp::DataFrame::create(Rcpp::Named("S") = vectorS,
                                                 Rcpp::Named("I") = vectorI,
                                                 Rcpp::Named("R") = vectorR);
  return SIRM;
}
