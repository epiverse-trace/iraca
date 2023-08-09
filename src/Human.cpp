// Copyright 2023 `iraca` authors.

#include "Human.h"

Human::Human(int _id, int _age, std::string _gender, int _highEd,
             int _homeTerritory, int _dailyTerritory, float _biteRate)
    : state("S"), gender(_gender), homeCoordinates({}), dailyCoordinates({}) {
  id = _id;
  // state = "S";
  age = _age;
  // gender = _gender;
  highEd = _highEd;
  positionX = 0;
  positionY = 0;
  // homeCoordinates = {};
  // dailyCoordinates = {};
  currentTerritory = _homeTerritory;
  dayOfInfection = 99999;
  viremia = false;
  biteRate = _biteRate;
  homeTerritory = _homeTerritory;
  dailyTerritory = _dailyTerritory;
  // Coordinates
}

Human::Human() {}

bool Human::operator==(const Human &other) const {
  // Define the comparison logic for the Human class
  return id == other.id && homeTerritory == other.homeTerritory &&
         age == other.age && gender == other.gender;
}

// Change states
void Human::changeToInfected(int _day) {
  if (state == "S") {
    state = "I";
    setDayOfInfection(_day);
  }
}

void Human::changeToRecovered() { state = "R"; }

// Set infection day
void Human::setDayOfInfection(int _day) {
  if (dayOfInfection == 99999) {
    dayOfInfection = _day;
  }
}

// Update viremic state (10 days of viremia aprox.)
// if not viremia and infected, and infected over 4 days ago turn on viremia
void Human::updateViremia(int _day) {
  if (state == "I" && viremia == false) {
    if (_day - dayOfInfection >= 4 && dayOfInfection < 99999) {
      viremia = true;
    }
  }
  if (viremia == true) {
    if (state == "I" && _day - dayOfInfection > 14) {
      changeToRecovered();
      viremia = false;
    }
  }
}

// change territory when moving
void Human::changeTerritory(int _territoryId) {
  currentTerritory = _territoryId;
}

// Set coordinates for daily interaction (work/school)
void Human::setDailyCoordinates(int _positionX, int _positionY) {
  dailyCoordinates = {_positionX, _positionY};
}

// Set coordinates for home
void Human::setHomeCoordinates(int _positionX, int _positionY) {
  homeCoordinates = {_positionX, _positionY};
}

// Update position (constant if home or work/school)
void Human::updatePosition(int _territoryId, int _positionX, int _positionY) {
  if (_territoryId == dailyTerritory) {
    positionX = dailyCoordinates[0];
    positionY = dailyCoordinates[1];
  } else if (_territoryId == homeTerritory) {
    positionX = homeCoordinates[0];
    positionY = homeCoordinates[1];
  } else {
    positionX = _positionX;
    positionY = _positionY;
  }
}

// Bite rate (can change if usses net intervention)
void Human::updateBiteRate(float _biteRate) { biteRate = _biteRate; }
