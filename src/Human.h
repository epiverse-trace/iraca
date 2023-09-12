// Copyright 2023 `iraca` authors.
#ifndef SRC_HUMAN_H_
#define SRC_HUMAN_H_

// Human Class

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

class Human {
 public:
  int getId() { return id; }
  std::string getState() { return state; }
  int getAge() { return age; }
  std::string getGender() { return gender; }
  int getHighEd() { return highEd; }
  int getCurrentTerritory() { return currentTerritory; }
  double getPositionX() { return positionX; }
  double getPositionY() { return positionY; }
  int getHomeTerritory() { return homeTerritory; }
  int getDailyTerritory() { return dailyTerritory; }
  int getIncubationPeriod() { return incubationPeriod; }
  int getInfectionDuration() { return infectionDuration; }
  int getDayOfInfection() { return dayOfInfection; }
  bool getViremia() { return viremia; }
  double getBiteRate() { return biteRate; }

  std::vector<double> getHomeCoordinates() { return homeCoordinates; }
  std::vector<double> getDailyCoordinates() { return dailyCoordinates; }

  void changeToInfected(int);
  void changeToRecovered();
  void setDayOfInfection(int);
  void updateViremia(int);
  void changeTerritory(int);
  void setDailyCoordinates(double, double);
  void setHomeCoordinates(double, double);
  void updatePosition(int, double, double);
  void updateBiteRate(double);

  bool operator==(const Human &other) const;
  Human(int, int, const std::string &, int, int, int, int, int, double);
  Human();
  boost::geometry::model::point<double, 2, boost::geometry::cs::cartesian>
      location;
  boost::geometry::model::point<double, 2, boost::geometry::cs::cartesian>
      homeLocation;
  boost::geometry::model::point<double, 2, boost::geometry::cs::cartesian>
      dailyLocation;

 private:
  int id;
  std::string state;
  int age;
  std::string gender;
  bool highEd;
  int currentTerritory;
  double positionX;
  double positionY;
  int homeTerritory;
  int dailyTerritory;
  int incubationPeriod;
  int infectionDuration;
  int dayOfInfection;
  bool viremia;
  double biteRate;
  std::vector<double> homeCoordinates;
  std::vector<double> dailyCoordinates;
};

#endif  // SRC_HUMAN_H_
