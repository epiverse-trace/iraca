// Copyright 2023 `iraca` authors.
#ifndef SRC_MOSQUITO_H_
#define SRC_MOSQUITO_H_

// Mosquito Class

#include <Rcpp.h>

#include <cstdlib>
#include <iostream>
#include <vector>

class Mosquito {
 public:
  int getId() { return id; }
  bool isInfected() { return infected; }
  int getAge() { return age; }
  int getCurrentTerritory() { return currentTerritory; }
  int getPositionX() { return positionX; }
  int getPositionY() { return positionY; }
  int getInitialPositionX() { return initialPositionX; }
  int getInitialPositionY() { return initialPositionY; }
  int getDayOfInfection() { return dayOfInfection; }
  int getBiteCount() { return biteCount; }
  int getNeededBites() { return neededBites; }
  bool isAlive() { return alive; }
  bool isAdult() { return adult; }
  int getLifespan() { return lifespan; }
  double getDevelopmentRate() { return developmentRate; }

  bool bite(double);
  bool infectingBite(double, int);
  bool infectiousBite(double);
  void changeToInfected(int);
  void updateAge();
  void die();
  void movement(int, int);
  void updateBiteCount();

  bool operator==(const Mosquito& other) const;
  Mosquito(int, int, int, int, int, double, int);
  Mosquito();
  ~Mosquito();

 private:
  int id;
  bool infected;
  int age;
  int currentTerritory;
  int positionX;
  int positionY;
  int initialPositionX;
  int initialPositionY;
  int dayOfInfection;
  int biteCount;
  int neededBites;
  int maxBitesPerDay;
  int bitesToday;
  bool alive;
  bool adult;
  int lifespan;
  double developmentRate;
};

#endif  // SRC_MOSQUITO_H_
