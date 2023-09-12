// Copyright 2023 `iraca` authors.
#ifndef SRC_MOSQUITO_H_
#define SRC_MOSQUITO_H_

// Mosquito Class

#include <Rcpp.h>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <cstdlib>
#include <iostream>
#include <vector>

class Mosquito {
 public:
  int getId() { return id; }
  bool isInfected() { return infected; }
  int getAge() { return age; }
  int getCurrentTerritory() { return currentTerritory; }
  double getPositionX() { return positionX; }
  double getPositionY() { return positionY; }
  double getInitialPositionX() { return initialPositionX; }
  double getInitialPositionY() { return initialPositionY; }
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
  void movement(double, double);
  void updateBiteCount();

  bool operator==(const Mosquito& other) const;
  Mosquito(int, int, double, double, int, double, int);
  Mosquito();
  ~Mosquito();

  boost::geometry::model::point<double, 2, boost::geometry::cs::cartesian>
      location;

 private:
  int id;
  bool infected;
  int age;
  int currentTerritory;
  double positionX;
  double positionY;
  double initialPositionX;
  double initialPositionY;
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
