// Copyright 2023 `iraca` authors.
#ifndef SRC_HUMAN_H_
#define SRC_HUMAN_H_

// Human Class

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
  int getPositionX() { return positionX; }
  int getPositionY() { return positionY; }
  int getHomeTerritory() { return homeTerritory; }
  int getDailyTerritory() { return dailyTerritory; }
  int getIncubationPeriod() { return incubationPeriod; }
  int getInfectionDuration() { return infectionDuration; }
  int getDayOfInfection() { return dayOfInfection; }
  bool getViremia() { return viremia; }
  double getBiteRate() { return biteRate; }

  std::vector<int> getHomeCoordinates() { return homeCoordinates; }
  std::vector<int> getDailyCoordinates() { return dailyCoordinates; }

  void changeToInfected(int);
  void changeToRecovered();
  void setDayOfInfection(int);
  void updateViremia(int);
  void changeTerritory(int);
  void setDailyCoordinates(int, int);
  void setHomeCoordinates(int, int);
  void updatePosition(int, int, int);
  void updateBiteRate(double);

  bool operator==(const Human &other) const;
  Human(int, int, const std::string &, int, int, int, int, int, double);
  Human();

 private:
  int id;
  std::string state;
  int age;
  std::string gender;
  bool highEd;
  int currentTerritory;
  int positionX;
  int positionY;
  int homeTerritory;
  int dailyTerritory;
  int incubationPeriod;
  int infectionDuration;
  int dayOfInfection;
  bool viremia;
  double biteRate;
  std::vector<int> homeCoordinates;
  std::vector<int> dailyCoordinates;
};

#endif  // SRC_HUMAN_H_
