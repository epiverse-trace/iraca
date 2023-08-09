// Copyright 2023 `iraca` authors.

#include "Mosquito.h"

// Initialization (different ages and probability to )
Mosquito::Mosquito(int _id, int _age, int _positionX, int _positionY,
                   int _currentTerritory, float _developmentRate,
                   int _lifespan) {
  id = _id;
  infected = false;
  age = _age;
  currentTerritory = _currentTerritory;
  positionX = _positionX;
  positionY = _positionY;
  initialPositionX = _positionX;
  initialPositionY = _positionY;
  dayOfInfection = 99999;
  biteCount = 0;
  neededBites = 10;
  maxBitesPerDay = 4;
  bitesToday = 0;
  alive = true;
  adult = (_age * _developmentRate >= 1) ? true : false;
  lifespan = _lifespan;
  developmentRate = _developmentRate;
}

Mosquito::Mosquito() {}

Mosquito::~Mosquito() {}

bool Mosquito::operator==(const Mosquito& other) const {
  // Define the comparison logic for the Human class
  return id == other.id && currentTerritory == other.currentTerritory;
}

// Mosquito bite
// if(0.2 <= _biteProbability) on inner if
bool Mosquito::bite(float _biteProbability) {
  bool output = false;
  if (adult) {
    if (biteCount <= neededBites && bitesToday <= maxBitesPerDay) {
      if (R::runif(0, 1) <= _biteProbability) {
        // if bites, update bite count
        updateBiteCount();
        output = true;
      }
    }
  }
  return output;
}

// Is bite infecting? (Human to Mosquito)
bool Mosquito::infectingBite(float _infectingProb, int _day) {
  bool output = false;
  if (dayOfInfection != 0 && _day - dayOfInfection > 5) {
    output = (R::runif(0, 1) <= _infectingProb);
  }
  return output;
}

// Is bite infectious? (Mosquito to Human)
bool Mosquito::infectiousBite(float _infectiousProb) {
  return (R::runif(0, 1) <= _infectiousProb);
}

// Change state
void Mosquito::changeToInfected(int _day) {
  dayOfInfection = _day;
  infected = true;
}

// Update age (die if older than lifespan)
void Mosquito::updateAge() {
  bitesToday = 0;
  age++;
  if (developmentRate * age >= 1) {
    adult = true;
  }
  if (age >= lifespan) {
    die();
  }
}

// Change alive state
void Mosquito::die() { alive = false; }

// Movement
void Mosquito::movement(int _positionX, int _positionY) {
  positionX = _positionX;
  positionY = _positionY;
}

// Bite count
void Mosquito::updateBiteCount() {
  bitesToday++;
  biteCount++;
}
