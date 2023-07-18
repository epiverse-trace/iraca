// Territory Class

#include <Rcpp.h>
#include <iostream>
#include <cstdlib>
#include <map>
#include <vector>
#include <algorithm>
#include "Territory.h"
#include "utils.cpp"

Territory::Territory(int _id, float _area, float _density, int _population, float _gas, float _sewage, std::vector<float> _ageProp, float _maleProp, float _highedProp, float _ndvi, float _elevation, vector<float> _movementPatterns)
{
    id = _id;
    density = _density;
    population = _population;
    gas = _gas;
    sewage = _sewage;
    ageProp = _ageProp;
    maleProp = _maleProp;
    highEdProp = _highedProp;
    ndvi = _ndvi;
    elevation = _elevation;
    birthRate = 0.04;
    deathRateAdults = 0.04;
    deathRateAquatic = 0.04;
    area = _area;
    width = int(round(sqrt(_area / 1.6) / 22));        // horizontal, 1.6 ratio between v and h, 10m blocks (can be changed)
    length = int(round(sqrt(_area / 1.6) * 1.6 / 22)); // vertical
    std::list<Human> humans;
    std::list<Mosquito> mosquitoes;
    std::list<int> intervenedAreas;
    std::vector<float> movementPatterns = _movementPatterns;
};

Territory::Territory(){};

// Initialize humans and mosquitoes
void Territory::initializeHumans()
{
    // Human possible attributes
    std::vector<int> possibleAges{0, 10, 20, 30, 40, 50, 60, 70, 80};
    std::vector<string> possibleGenders{"M", "F"};
    std::vector<int> possibleHighEd{true, false};
    std::vector<int> possibleHighEd{1, 0};
    std::vector<int> possibleSewage{1, 0};
    std::vector<int> possibleGas{1, 0};
    for (int hum = 0; hum < population; hum++)
    {
        int _age = possibleAges[weightedRandom(ageProp)];
        std::string _gender = possibleGenders[weightedRandom({maleProp, 1 - maleProp})];
        int _highEd = possibleHighEd[weightedRandom({highEdProp, 1 - highEdProp})];
        int _sewageHuman = possibleSewage[weightedRandom({sewage, 1 - sewage})];
        int _gasHuman = possibleGas[weightedRandom({gas, 1 - gas})];
        // random position in grid
        int _positionX = int(R::runif(0, length));
        int _positionY = int(R::runif(0, width));
        // daily interaction (Work, school, etc.)
        int _dailyEnv = weightedRandom(movementPatterns);
        // human creation
        Human newHuman(hum, _age, _gender, _highEd, id, _dailyEnv, 0.65);
        addHuman(newHuman);
    }
};

void Territory::initializeMosquitoes()
{
    int nMosquitoes = population * 1.3;
    std::vector<bool> infectedMosquito{false, true};
        std::uniform_int_distribution<> ageMosquito(0, 30);
        for (int mos = 0; mos <= nMosquitoes; mos++)
        {
            int _age = int(R::runif(0,32));
            int _positionX = int(R::runif(0, length));
            int _positionY = int(R::runif(0, width));
            float _developmentRate = 0.091;
            int _lifespan = 30;
            // mosquito creation
            Mosquito newMosquito(mos, _age, _positionX, _positionY, id, _developmentRate, _lifespan);
            addMosquito(newMosquito);
        }
};

// Add and remove humans and mosquitoes
void Territory::addHuman(Human _human)
{
    humans.push_back(_human);
};

void Territory::removeHuman(Human _human)
{
    humans.remove(_human);
};

void Territory::addMosquito(Mosquito _mosquito)
{
    mosquitoes.push_back(_mosquito);
};

void Territory::removeMosquito(Mosquito _mosquito)
{
    mosquitoes.remove(_mosquito);
};

void Territory::resetHumans()
{
    humans.clear();
};

// Birth and Death rates for mosquitoes
void Territory::updateDeathRates(float _temperature, int _days_ov_90)
{
    double temperature = (double)(_temperature);
    double days_ov_90 = (double)(_days_ov_90);
    deathRateAdults = (double)(0.8692 - 0.159 * temperature + 0.01116 * pow(temperature, 2) - 0.0003409 * pow(temperature, 3) + 0.000003804 * pow(temperature, 4) + 0.027 + 0.047 * days_ov_90);
    deathRateAquatic = (double)(2.13 - 0.3797 * temperature + 0.02457 * pow(temperature, 2) - 0.0006778 * pow(temperature, 3) + 0.000006792 * pow(temperature, 4) + 0.026 + 0.03 * _days_ov_90);
};

void Territory::updateBirthRate(float _temperature, int _days_ov_90)
{
    double temperature = (double)(_temperature);
    double days_ov_90 = (double)(_days_ov_90);
    birthRate = (double)(-0.000016 * pow(temperature, 3) + 0.00117114 * pow(temperature, 2) - 0.024371 * temperature + 0.186171 + 0.004 + 0.018 * _days_ov_90);
};

void Territory::updateMosquitoes()
{

};

void updateHumans()
{

};

void interaction()
{

};

void moveMosquitoes()
{

};