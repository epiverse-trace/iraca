// Copyright 2023 `iraca` authors.

#ifndef SRC_TERRITORY_H_
#define SRC_TERRITORY_H_

// Territory Class

#include <Rcpp.h>

#include <cstdlib>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>

#include "Human.h"
#include "Mosquito.h"
// #include "Human.cpp"
// #include "Mosquito.cpp"
// #include <time.h>
// #include <random>

class Territory {
 public:
  int getId() { return id; }
  double getArea() { return area; }
  int getWidth() { return width; }
  int getLength() { return length; }
  double getDensity() { return density; }
  int getPopulation() { return population; }
  double getGas() { return gas; }
  double getSewage() { return sewage; }
  std::vector<double> getAgeProp() { return ageProp; }
  double getMaleProp() { return maleProp; }
  double getHighEdProp() { return highEdProp; }
  double getBirthRate() { return birthRate; }
  double getDeathRateAdults() { return deathRateAdults; }
  double getDeathRateAquatic() { return deathRateAquatic; }
  std::list<Human> getHumans() { return humans; }
  std::list<Mosquito> getMosquitoes() { return mosquitoes; }
  std::vector<double> getMovementPatterns() { return movementPatterns; }

  Territory(int, double, double, int, const std::vector<double>&, double, double,
            double, double, const std::vector<double>&);
  Territory();

  void initializeHumans(double, double, double);
  void initializeMosquitoes(int, double, bool);
  void addHuman(Human);
  void removeHuman(Human);
  void resetHumans();
  void addMosquito(Mosquito);
  void removeMosquito(Mosquito);
  void updateDeathRates(double, int);
  void updateBirthRate(double, int);
  void moveHumans();
  void updateMosquitoes();
  void updateHumans(int);
  void interaction(int);
  bool checkProximity(Human, Mosquito);
  void moveMosquitoes();
  void birthMosquitoes(double, double);
  void deathMosquitoes(double, double);
  std::vector<int> contagions();
  int weightedRandom(std::vector<double>);
  std::list<Human> humans;
  std::list<Mosquito> mosquitoes;

 private:
  int id;
  double area;
  int width;
  int length;
  double density;
  int population;
  std::vector<double> ageProp;
  double maleProp;
  double highEdProp;
  double gas;
  double sewage;
  double birthRate;
  double deathRateAdults;
  double deathRateAquatic;
  std::vector<double> movementPatterns;
};

#endif  // SRC_TERRITORY_H_
