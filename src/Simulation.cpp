// Copyright 2023 `iraca` authors.

#include "Simulation.h"

Simulation::Simulation(int _days,
                       const std::vector<std::vector<float>> &_territoriesData,
                       const std::vector<std::vector<float>> &_temperatureData,
                       const std::vector<std::vector<float>> &_movementData,
                       int _incubationPeriod, int _infectionDuration,
                       float _initInfectedHumans, float _initInfectedMosquitoes)
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
  int nTerritories = territoriesData.size();
  for (int ter = 0; ter < nTerritories - 1; ter++) {
    // Environment attributes from DANE database
    float density = territoriesData[ter][45];
    int population = territoriesData[ter][32];
    float gas = territoriesData[ter][29];
    float sewage = territoriesData[ter][30];
    float area = territoriesData[ter][1];
    std::vector<float> ageProp = {
        territoriesData[ter][35], territoriesData[ter][36],
        territoriesData[ter][37], territoriesData[ter][38],
        territoriesData[ter][39], territoriesData[ter][40],
        territoriesData[ter][41], territoriesData[ter][42],
        territoriesData[ter][43]};
    float maleProp = territoriesData[ter][33];
    float highEdProp = territoriesData[ter][44];
    std::vector<float> movementPatterns = movementData[ter];
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
        tempTerritory->moveMosquitoes();
        tempTerritory->moveHumans();

        transitHumans.assign(tempTerritory->humans.begin(),
                             tempTerritory->humans.end());
        tempTerritory->resetHumans();
      }
      for (auto human : transitHumans) {
        territories[human.getcurrentTerritory()].addHuman(human);
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
      vectorS.push_back(suceptible);
      vectorI.push_back(infected);
      vectorR.push_back(recovered);
    }
  }
  Rcpp::DataFrame SIRM = Rcpp::DataFrame::create(Rcpp::Named("S") = vectorS,
                                                 Rcpp::Named("I") = vectorI,
                                                 Rcpp::Named("R") = vectorR);
  return SIRM;
}
