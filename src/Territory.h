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
  float getArea() { return area; }
  int getWidth() { return width; }
  int getLength() { return length; }
  float getDensity() { return density; }
  int getPopulation() { return population; }
  float getGas() { return gas; }
  float getSewage() { return sewage; }
  std::vector<float> getAgeProp() { return ageProp; }
  float getMaleProp() { return maleProp; }
  float getHighEdProp() { return highEdProp; }
  double getBirthRate() { return birthRate; }
  double getDeathRateAdults() { return deathRateAdults; }
  double getDeathRateAquatic() { return deathRateAquatic; }
  std::list<Human> getHumans() { return humans; }
  std::list<Mosquito> getMosquitoes() { return mosquitoes; }
  std::vector<float> getMovementPatterns() { return movementPatterns; }

  Territory(int, float, float, int, const std::vector<float>&, float, float,
            float, float, const std::vector<float>&);
  Territory();

  void initializeHumans(float);
  void initializeMosquitoes(int, float, bool);
  void addHuman(Human);
  void removeHuman(Human);
  void resetHumans();
  void addMosquito(Mosquito);
  void removeMosquito(Mosquito);
  void updateDeathRates(float, int);
  void updateBirthRate(float, int);
  void moveHumans();
  void updateMosquitoes();
  void updateHumans(int);
  void interaction(int);
  bool checkProximity(Human, Mosquito);
  void moveMosquitoes();
  void birthMosquitoes(float, float);
  void deathMosquitoes(float, float);
  std::vector<int> contagions();
  int weightedRandom(std::vector<float>);
  std::list<Human> humans;
  std::list<Mosquito> mosquitoes;

 private:
  int id;
  float area;
  int width;
  int length;
  float density;
  int population;
  std::vector<float> ageProp;
  float maleProp;
  float highEdProp;
  float gas;
  float sewage;
  double birthRate;
  double deathRateAdults;
  double deathRateAquatic;
  std::vector<float> movementPatterns;
};

#endif  // SRC_TERRITORY_H_
