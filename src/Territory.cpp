// Copyright 2023 `iraca` authors.

#include "Territory.h"

// std::random_device rd;
// std::mt19937 gen(static_cast<unsigned int>(time(nullptr)));

Territory::Territory(int _id, float _area, float _density, int _population,
                     const std::vector<float> &_ageProp, float _maleProp,
                     float _highedProp, float _gas, float _sewage,
                     const std::vector<float> &_movementPatterns)
    : humans({}),
      mosquitoes({}),
      ageProp(_ageProp),
      movementPatterns(_movementPatterns) {
  id = _id;
  area = _area;
  width = static_cast<int>(round(sqrt(_area / 1.6) / 22));
  // horizontal, 1.6 ratio between v and h, 10m blocks (can be changed)
  length = static_cast<int>(round(sqrt(_area / 1.6) * 1.6 / 22));
  density = _density;
  population = _population;
  // ageProp = _ageProp;
  maleProp = _maleProp;
  gas = _gas;
  sewage = _sewage;
  highEdProp = _highedProp;
  birthRate = 0.04;
  deathRateAdults = 0.04;
  deathRateAquatic = 0.04;
  // vertical
  // std::list<Human> humans;
  // std::list<Mosquito> mosquitoes;
  // std::vector<float> movementPatterns = _movementPatterns;
}

Territory::Territory() {}

// Initialize humans and mosquitoes
void Territory::initializeHumans(float _infectedHumans = 0) {
  // Human possible attributes
  std::vector<int> possibleAges{0, 10, 20, 30, 40, 50, 60, 70, 80};
  std::vector<std::string> possibleGenders{"M", "F"};
  std::vector<bool> possibleHighEd{true, false};
  for (int hum = 0; hum < population; hum++) {
    int _age = possibleAges[weightedRandom(ageProp)];
    std::string _gender =
        possibleGenders[weightedRandom({maleProp, 1 - maleProp})];
    int _highEd = possibleHighEd[weightedRandom({highEdProp, 1 - highEdProp})];
    int _dailyEnv = weightedRandom(movementPatterns);
    // Human creation
    Human newHuman(hum, _age, _gender, _highEd, id, _dailyEnv, 0.65);

    // random position in grid
    int _positionX = static_cast<int>(R::runif(0, length));
    int _positionY = static_cast<int>(R::runif(0, width));
    // std::uniform_int_distribution<> distrHumanX(0, length);
    // int _positionX = distrHumanX(gen);
    // std::uniform_int_distribution<> distrHumanY(0, width);
    // int _positionY = distrHumanY(gen);
    newHuman.setHomeCoordinates(_positionX, _positionY);

    // Will human be infected?
    bool infectedHuman = R::runif(0, 1) <= _infectedHumans;
    // std::uniform_real_distribution<> infected(0, 1.0);
    // bool infectedHuman = infected(gen) < _infectedHumans;
    if (infectedHuman) {
      newHuman.changeToInfected(static_cast<int>(R::runif(-14, 0)));
      // std::uniform_int_distribution<> changeInfH(-14, 0);
      // int _tempInfH = changeInfH(gen);
      // newHuman.changeToInfected(_tempInfH);
      newHuman.updateViremia(0);
    }

    // Adding to human list
    addHuman(newHuman);
  }
}

void Territory::initializeMosquitoes(int _ammount, float _infectedMosquitoes,
                                     bool onlyLarvae = true) {
  for (int mos = 0; mos <= _ammount; mos++) {
    int _age = 0;

    if (!onlyLarvae) {
      _age = static_cast<int>(R::runif(0, 32));
      // std::uniform_int_distribution<> ageM(0, 32);
      // _age = ageM(gen);
    }
    int _positionX = static_cast<int>(R::runif(0, length));
    int _positionY = static_cast<int>(R::runif(0, width));
    // std::uniform_int_distribution<> distrHumanX(0, length);
    // int _positionX = distrHumanX(gen);
    // std::uniform_int_distribution<> distrHumanY(0, width);
    // int _positionY = distrHumanY(gen);
    float _developmentRate = 0.091;
    int _lifespan = 30;
    // mosquito creation
    Mosquito newMosquito(mos, _age, _positionX, _positionY, id,
                         _developmentRate, _lifespan);

    // Is mosquito infected?
    bool infectedMosquito = R::runif(0, 1) <= _infectedMosquitoes;
    // std::uniform_real_distribution<> infected_2(0, 1.0);
    // bool infectedMosquito = infected_2(gen) < _infectedMosquitoes;
    if (infectedMosquito && !onlyLarvae) {
      newMosquito.changeToInfected(static_cast<int>(R::runif(-_lifespan, 0)));
      // std::uniform_int_distribution<> changeInf(-_lifespan, 0);
      // int _tempInf = changeInf(gen);
      // newMosquito.changeToInfected(_tempInf);
      newMosquito.updateBiteCount();
    }

    addMosquito(newMosquito);
  }
}

// Add and remove humans and mosquitoes
void Territory::addHuman(Human _human) { humans.push_back(_human); }

void Territory::removeHuman(Human _human) { humans.remove(_human); }

void Territory::addMosquito(Mosquito _mosquito) {
  mosquitoes.push_back(_mosquito);
}

void Territory::removeMosquito(Mosquito _mosquito) {
  mosquitoes.remove(_mosquito);
}

void Territory::resetHumans() { humans.clear(); }

// Birth and Death rates for mosquitoes
void Territory::updateDeathRates(float _temperature, int _days_ov_90) {
  double temperature = static_cast<double>(_temperature);
  double days_ov_90 = static_cast<double>(_days_ov_90);
  deathRateAdults = static_cast<double>(
      0.8692 - 0.159 * temperature + 0.01116 * pow(temperature, 2) -
      0.0003409 * pow(temperature, 3) + 0.000003804 * pow(temperature, 4) +
      0.027 + 0.047 * days_ov_90);
  deathRateAquatic = static_cast<double>(
      2.13 - 0.3797 * temperature + 0.02457 * pow(temperature, 2) -
      0.0006778 * pow(temperature, 3) + 0.000006792 * pow(temperature, 4) +
      0.026 + 0.03 * days_ov_90);
}

void Territory::updateBirthRate(float _temperature, int _days_ov_90) {
  double temperature = static_cast<double>(_temperature);
  double days_ov_90 = static_cast<double>(_days_ov_90);
  birthRate = static_cast<double>(
      -0.000016 * pow(temperature, 3) + 0.00117114 * pow(temperature, 2) -
      0.024371 * temperature + 0.186171 + 0.004 + 0.018 * days_ov_90);
}

void Territory::moveMosquitoes() {
  for (auto mosquito : mosquitoes) {
    if (mosquito.isAdult()) {
      Mosquito *tempMosquito = &mosquito;
      int currentPositionX = tempMosquito->getPositionX();
      int currentPositionY = tempMosquito->getPositionY();

      int newPositionX =
          tempMosquito->getPositionX() + static_cast<int>(R::runif(-2, 2));
      int newPositionY =
          tempMosquito->getPositionY() + static_cast<int>(R::runif(-2, 2));
      // std::uniform_int_distribution<> distrHumanX(-2, 2);
      // int newPositionX = distrHumanX(gen) + tempMosquito->getPositionX();
      // std::uniform_int_distribution<> distrHumanY(-2, 2);
      // int newPositionY = distrHumanY(gen) + tempMosquito->getPositionY();
      if (newPositionX <= length && newPositionX >= 0 &&
          abs(newPositionX - mosquito.getInitialPositionX()) <= 6) {
        currentPositionX = newPositionX;
      }
      if (newPositionY <= width && newPositionY >= 0 &&
          abs(newPositionY - mosquito.getInitialPositionY()) <= 6) {
        currentPositionY = newPositionY;
      }

      tempMosquito->movement(currentPositionX, currentPositionY);
    }
  }
}

void Territory::moveHumans() {
  for (auto human : humans) {
    Human *tempHuman = &human;
    if (tempHuman->getHomeTerritory() == id) {
      tempHuman->changeTerritory(tempHuman->getDailyTerritory());
      tempHuman->updatePosition(tempHuman->getDailyTerritory(), 0, 0);
    } else {
      tempHuman->changeTerritory(tempHuman->getHomeTerritory());
      tempHuman->updatePosition(tempHuman->getHomeTerritory(), 0, 0);
    }
  }
}

void Territory::updateHumans(int _day) {
  for (auto human : humans) {
    Human *tempHuman = &human;
    tempHuman->updateViremia(_day);
  }
}

void Territory::birthMosquitoes(float _temperature, float _maxTemperature) {
  updateBirthRate(_temperature, _maxTemperature);
  int newMosquitoes = static_cast<int>(birthRate * mosquitoes.size());
  initializeMosquitoes(newMosquitoes, 0);
}

void Territory::deathMosquitoes(float _temperature, float _maxTemperature) {
  updateDeathRates(_temperature, _maxTemperature);
  for (auto mosquito : mosquitoes) {
    if (mosquito.isAdult()) {
      bool die = R::runif(0, 1) <= deathRateAdults;
      // std::uniform_real_distribution<> death(0, 1.0);
      // bool die = death(gen) < deathRateAdults;
      if (die) {
        mosquito.die();
      }
    } else {
      bool die = R::runif(0, 1) <= deathRateAquatic;
      // std::uniform_real_distribution<> death_2(0, 1.0);
      // bool die = death_2(gen) < deathRateAdults;
      if (die) {
        mosquito.die();
      }
    }
  }
}

void Territory::updateMosquitoes() {
  int deaths = 0;
  for (auto &mosquito : mosquitoes) {
    // Mosquito *tempMosquito = &mosquito;
    mosquito.updateAge();
    if (!mosquito.isAlive()) {
      deaths++;
      mosquitoes.remove(mosquito);
    }
  }
}

void Territory::interaction(int _day) {
  for (auto &human : humans) {
    for (auto &mosquito : mosquitoes) {
      // Check if human and mosquito are in the same position
      if (checkProximity(human, mosquito)) {
        // Try to bite
        if (mosquito.bite(human.getBiteRate())) {
          // Could or not be infectuous
          if (mosquito.isInfected()) {
            // Mosquit to Human intection
            if (mosquito.infectingBite(0.11, _day)) {
              human.changeToInfected(_day);
            }
          } else if (human.getViremia()) {
            // Human to Mosquito intection
            if (mosquito.infectiousBite(1)) {
              mosquito.changeToInfected(_day);
            }
          }
        }
      }
    }
  }
}

bool Territory::checkProximity(Human human, Mosquito mosquito) {
  bool sameLocation = false;
  if (human.getPositionX() == mosquito.getPositionX() &&
      human.getPositionY() == mosquito.getPositionY()) {
    sameLocation = true;
  }
  return sameLocation;
}

std::vector<int> Territory::contagions() {
  std::vector<int> SIR = {0, 0, 0};
  for (auto human : humans) {
    if (human.getState() == "I") {
      SIR[1] += 1;
    } else if (human.getState() == "R") {
      SIR[2] += 1;
    } else {
      SIR[0] += 1;
    }
  }
  return SIR;
}

int Territory::weightedRandom(std::vector<float> probabilities) {
  int output = 0;
  std::vector<int> transformedP;
  int accumulated = 0;
  for (float i : probabilities) {
    int transformed = static_cast<int>(i * 1000) + accumulated;
    transformedP.push_back(transformed);
    accumulated = transformed;
  }
  int randomInteger = Rcpp::sample(1000, 1)[0];
  // std::uniform_int_distribution<> distInt(0, 1000);
  // int randomInteger =  distInt(gen);
  for (int i = 0; i < static_cast<int>(probabilities.size()); i++) {
    if (randomInteger < transformedP[i]) {
      output = i;
      break;
    }
  }
  return output;
}
