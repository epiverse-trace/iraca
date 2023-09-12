// Copyright 2023 `iraca` authors.

#include "Territory.h"

Territory::Territory(
    int _id, double _area, double _density, int _population,
    const std::vector<double> &_ageProp, double _maleProp, double _highedProp,
    double _gas, double _sewage, const std::vector<double> &_movementPatterns,
    boost::geometry::model::multi_polygon<
        boost::geometry::model::polygon<boost::geometry::model::point<
            double, 2, boost::geometry::cs::cartesian>>>
        _geometry)
    : humans({}),
      mosquitoes({}),
      ageProp(_ageProp),
      movementPatterns(_movementPatterns) {
  id = _id;
  area = _area;
  width = static_cast<int>(round(sqrt(_area / 1.6) / 22));
  //  horizontal, 1.6 ratio between v and h, 10m blocks (can be changed)
  length = static_cast<int>(round(sqrt(_area / 1.6) * 1.6 / 22));
  density = _density;
  population = _population;
  maleProp = _maleProp;
  gas = _gas;
  sewage = _sewage;
  highEdProp = _highedProp;
  birthRate = 0.04;
  deathRateAdults = 0.04;
  deathRateAquatic = 0.04;
  boost::geometry::model::box<
      boost::geometry::model::point<double, 2, boost::geometry::cs::cartesian>>
      bbox;
  boost::geometry::envelope(geometry, bbox);
  double min_x = bbox.min_corner().get<0>();
  double max_x = bbox.max_corner().get<0>();
  double min_y = bbox.min_corner().get<1>();
  double max_y = bbox.max_corner().get<1>();
  coordsX = {min_x, max_x};
  coordsY = {min_y, max_y};
  geometry = _geometry;
  boost::geometry::index::rtree<
      std::pair<boost::geometry::model::point<double, 2,
                                              boost::geometry::cs::cartesian>,
                Human *>,
      boost::geometry::index::rstar<4>>
      humansTree;
}

Territory::Territory() {}

// Initialize humans and mosquitoes
void Territory::initializeHumans(double _infectedHumans = 0,
                                 double _incubationPeriod = 4,
                                 double _infectionDuration = 14) {
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
    Human newHuman(hum, _age, _gender, _highEd, _incubationPeriod,
                   _infectionDuration, id, _dailyEnv, 0.65);

    // random position in grid
    double _positionX = R::runif(coordsX[0], coordsX[1]);
    double _positionY = R::runif(coordsY[0], coordsY[1]);
    newHuman.setHomeCoordinates(_positionX, _positionY);

    // Will human be infected?
    bool infectedHuman = R::runif(0, 1) <= _infectedHumans;
    if (infectedHuman) {
      newHuman.changeToInfected(static_cast<int>(R::runif(-14, 0)));
      newHuman.updateViremia(0);
    }

    // Adding to human list
    addHuman(newHuman);
  }
}

void Territory::initializeMosquitoes(int _amount, double _infectedMosquitoes,
                                     bool onlyLarvae = true) {
  for (int mos = 0; mos <= _amount; mos++) {
    int _age = 0;

    if (!onlyLarvae) {
      _age = static_cast<int>(R::runif(0, 32));
    }
    double _positionX = R::runif(coordsX[0], coordsX[1]);
    double _positionY = R::runif(coordsY[0], coordsY[1]);
    double _developmentRate = 0.091;
    int _lifespan = 30;
    // mosquito creation
    Mosquito newMosquito(mos, _age, _positionX, _positionY, id,
                         _developmentRate, _lifespan);

    // Is mosquito infected?
    bool infectedMosquito = R::runif(0, 1) <= _infectedMosquitoes;
    if (infectedMosquito && !onlyLarvae) {
      newMosquito.changeToInfected(static_cast<int>(R::runif(-_lifespan, 0)));
      newMosquito.updateBiteCount();
    }

    addMosquito(newMosquito);
  }
}

// Add and remove humans and mosquitoes
void Territory::addHuman(Human _human) {
  boost::geometry::model::point<double, 2, boost::geometry::cs::cartesian>
      location(_human.getPositionX(), _human.getPositionY());
  humansTree.insert(std::make_pair(location, &_human));
  humans.push_back(_human);
}

void Territory::removeHuman(Human _human) { humans.remove(_human); }

void Territory::addMosquito(Mosquito _mosquito) {
  mosquitoes.push_back(_mosquito);
}

void Territory::removeMosquito(Mosquito _mosquito) {
  mosquitoes.remove(_mosquito);
}

void Territory::resetHumans() { humans.clear(); }

// Birth and Death rates for mosquitoes
void Territory::updateDeathRates(double _temperature, int _days_ov_90) {
  double temperature = static_cast<double>(_temperature);
  double days_ov_90 = static_cast<double>(_days_ov_90);
  deathRateAdults = static_cast<double>(
      0.8692 - 0.159 * temperature + 0.01116 * pow(temperature, 2) -
      0.0003409 * pow(temperature, 3) + 0.000003804 * pow(temperature, 4) +
      0.027 + 0.0014 * days_ov_90);
  deathRateAquatic = static_cast<double>(
      2.13 - 0.3797 * temperature + 0.02457 * pow(temperature, 2) -
      0.0006778 * pow(temperature, 3) + 0.000006792 * pow(temperature, 4) +
      0.026 + 0.006 * days_ov_90);
}

void Territory::updateBirthRate(double _temperature, int _days_ov_90) {
  double temperature = static_cast<double>(_temperature);
  double days_ov_90 = static_cast<double>(_days_ov_90);
  birthRate = static_cast<double>(
      -0.000016 * pow(temperature, 3) + 0.00117114 * pow(temperature, 2) -
      0.024371 * temperature + 0.186171 + 0.004 + 0.0005 * days_ov_90);
}

void Territory::moveMosquitoes() {
  for (auto &mosquito : mosquitoes) {
    if (mosquito.isAdult()) {
      double currentPositionX = mosquito.getPositionX();
      double currentPositionY = mosquito.getPositionY();

      double newPositionX = mosquito.getPositionX() + (R::runif(-2, 2));
      double newPositionY = mosquito.getPositionY() + (R::runif(-2, 2));
      if (newPositionX <= length && newPositionX >= 0 &&
          abs(newPositionX - mosquito.getInitialPositionX()) <= 6) {
        currentPositionX = newPositionX;
      }
      if (newPositionY <= width && newPositionY >= 0 &&
          abs(newPositionY - mosquito.getInitialPositionY()) <= 6) {
        currentPositionY = newPositionY;
      }

      mosquito.movement(currentPositionX, currentPositionY);
    }
  }
}

void Territory::moveHumans() {
  for (auto &human : humans) {
    // If im at home then move to Daily
    if (human.getCurrentTerritory() == id && human.getHomeTerritory() == id) {
      human.changeTerritory(human.getDailyTerritory());
      human.updatePosition(human.getDailyTerritory(), 0, 0);
    } else {
      human.changeTerritory(human.getHomeTerritory());
      human.updatePosition(human.getHomeTerritory(), 0, 0);
    }
  }
}

void Territory::updateHumans(int _day) {
  for (auto &human : humans) {
    human.updateViremia(_day);
  }
}

void Territory::birthMosquitoes(double _temperature, double _maxTemperature) {
  updateBirthRate(_temperature, _maxTemperature);
  int newMosquitoes = static_cast<int>(birthRate * mosquitoes.size());
  initializeMosquitoes(newMosquitoes, 0);
}

void Territory::deathMosquitoes(double _temperature, double _maxTemperature) {
  updateDeathRates(_temperature, _maxTemperature);
  for (auto mosquito : mosquitoes) {
    if (mosquito.isAdult()) {
      bool die = R::runif(0, 1) <= deathRateAdults;
      if (die) {
        mosquito.die();
      }
    } else {
      bool die = R::runif(0, 1) <= deathRateAquatic;
      if (die) {
        mosquito.die();
      }
    }
  }
}

void Territory::updateMosquitoes() {
  for (auto &mosquito : mosquitoes) {
    mosquito.updateAge();
  }
  auto predicate = [](auto mosquito) { return mosquito.isAlive() == false; };
  auto newEnd = std::remove_if(mosquitoes.begin(), mosquitoes.end(), predicate);
  mosquitoes.erase(newEnd, mosquitoes.end());
}

void Territory::interaction(int _day) {
  double buffer_distance = 1000/111139;  //  distance in meters

  for (auto &mosquito : mosquitoes) {
    auto query_point = mosquito.location;
    std::vector<std::pair<boost::geometry::model::point<
                              double, 2, boost::geometry::cs::cartesian>,
                          Human *>>
        result;
    humansTree.query(
        boost::geometry::index::satisfies(
            [&query_point, &buffer_distance](
                const std::pair<boost::geometry::model::point<
                                    double, 2, boost::geometry::cs::cartesian>,
                                Human *> &value) {
              return boost::geometry::distance(value.first, query_point) <=
                     buffer_distance;
            }),
        std::back_inserter(result));

    if (!result.empty()) {
      int max_iter = std::min(static_cast<int>(result.size()), 2);
      for (int i = 0; i < max_iter; i++) {
        Human *human = result[i].second;
        if (mosquito.bite(human->getBiteRate())) {
          // Could or not be infectuous
          if (mosquito.isInfected()) {
            // Mosquit to Human intection
            if (mosquito.infectingBite(0.11, _day)) {
              human->changeToInfected(_day);
            }
          } else if (human->getViremia()) {
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

// for (auto &human : humans) {
//   for (auto &mosquito : mosquitoes) {
//     // Check if human and mosquito are in the same position
//     if (checkProximity(human, mosquito)) {
//       // Try to bite
//       if (mosquito.bite(human.getBiteRate())) {
//         // Could or not be infectuous
//         if (mosquito.isInfected()) {
//           // Mosquit to Human intection
//           if (mosquito.infectingBite(0.11, _day)) {
//             human.changeToInfected(_day);
//           }
//         } else if (human.getViremia()) {
//           // Human to Mosquito intection
//           if (mosquito.infectiousBite(1)) {
//             mosquito.changeToInfected(_day);
//           }
//         }
//       }
//     }
//   }
// }

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

int Territory::weightedRandom(std::vector<double> probabilities) {
  int output = 0;
  std::vector<int> transformedP;
  int accumulated = 0;
  for (double i : probabilities) {
    int transformed = static_cast<int>(i * 1000) + accumulated;
    transformedP.push_back(transformed);
    accumulated = transformed;
  }
  int randomInteger = Rcpp::sample(1000, 1)[0];
  for (int i = 0; i < static_cast<int>(probabilities.size()); i++) {
    if (randomInteger < transformedP[i]) {
      output = i;
      break;
    }
  }
  return output;
}
