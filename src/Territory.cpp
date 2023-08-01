// Territory Class

#include <Rcpp.h>
#include <iostream>
#include <cstdlib>
#include <map>
#include <vector>
#include <algorithm>
#include "Territory.h"
#include "utils.cpp"

Territory::Territory(int _id, float _area, float _density, int _population, float _gas, float _sewage, std::vector<float> _ageProp, float _maleProp, float _highedProp, std::vector<float> _movementPatterns)
{
    id = _id;
    density = _density;
    population = _population;
    gas = _gas;
    sewage = _sewage;
    ageProp = _ageProp;
    maleProp = _maleProp;
    highEdProp = _highedProp;
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
void Territory::initializeHumans(float _infectedHumans = 0)
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
        int _dailyEnv = weightedRandom(movementPatterns);
        // Human creation
        Human newHuman(hum, _age, _gender, _highEd, id, _dailyEnv, 0.65);

        // random position in grid
        int _positionX = int(R::runif(0, length));
        int _positionY = int(R::runif(0, width));
        newHuman.setHomeCoordinates(_positionX, _positionY);

        // Will human be infected?
        bool infectedHuman = R::runif(0, 1) <= _infectedHumans;
        if (infectedHuman)
        {
            newHuman.changeToInfected(int(R::runif(-14, 0)));
            newHuman.updateViremia(0);
        }

        // Adding to human list
        addHuman(newHuman);
    }
};

void Territory::initializeMosquitoes(int _ammount, float _infectedMosquitoes, bool onlyLarvae = true)
{
    std::uniform_int_distribution<> ageMosquito(0, 30);
    for (int mos = 0; mos <= _ammount; mos++)
    {
        int _age = 0;
        
        if(!onlyLarvae){
            _age = int(R::runif(0, 32));   
        }
        int _positionX = int(R::runif(0, length));
        int _positionY = int(R::runif(0, width));
        float _developmentRate = 0.091;
        int _lifespan = 30;
        // mosquito creation
        Mosquito newMosquito(mos, _age, _positionX, _positionY, id, _developmentRate, _lifespan);

        // Is mosquito infected?
        bool infectedMosquito = R::runif(0, 1) <= _infectedMosquitoes;
        if (infectedMosquito && !onlyLarvae)
        {
            newMosquito.changeToInfected(int(R::runif(-_lifespan, 0)));
            newMosquito.updateBiteCount();
        }

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

void Territory::moveMosquitoes()
{
    for (auto mosquito : mosquitoes)
    {
        if (mosquito.isAdult())
        {
            Mosquito *tempMosquito = &mosquito;
            int currentPositionX = tempMosquito->getPositionX();
            int currentPositionY = tempMosquito->getPositionY();

            int newPositionX = tempMosquito->getPositionX() + int(R::runif(-2, 2));
            int newPositionY = tempMosquito->getPositionY() + int(R::runif(-2, 2));

            if (newPositionX <= length && newPositionX >= 0 && abs(newPositionX - mosquito.getInitialPositionX()) <= 6)
            {
                currentPositionX = newPositionX;
            };
            if (newPositionY <= width && newPositionY >= 0 && abs(newPositionY - mosquito.getInitialPositionY()) <= 6)
            {
                currentPositionY = newPositionY;
            };

            tempMosquito->updateMosquito(currentPositionX, currentPositionY);
        }
    }
};

void Territory::moveHumans()
{
    for (auto human : humans)
    {
        Human *tempHuman = &human;
        if(tempHuman->getHomeTerritory() == id){
            tempHuman->changeTerritory(tempHuman->getDailyTerritory());
            tempHuman->updatePosition(tempHuman->getDailyTerritory(), 0, 0);
        }
        else
        {
            tempHuman->changeTerritory(tempHuman->getHomeTerritory());
            tempHuman->updatePosition(tempHuman->getHomeTerritory(), 0, 0);
        }
    }
};

void Territory::updateHumans(int _day)
{
    for (auto human : humans)
    {
        Human *tempHuman = &human;
        tempHuman->updateViremia(_day);
    }
};

void Territory::birthMosquitoes(float _temperature, float _maxTemperature)
{
    updateBirthRate(_temperature, _maxTemperature);
    int newMosquitoes = int(birthRate * mosquitoes.size());
    initializeMosquitoes(newMosquitoes, 0);
};

void Territory::deathMosquitoes(float _temperature, float _maxTemperature)
{
    updateDeathRates(_temperature, _maxTemperature);
    for (auto mosquito : mosquitoes)
    {
        if (mosquito.isAdult())
        {
            bool die = R::runif(0, 1) <= deathRateAdults;
            if (die)
            {
                mosquito.die();
            }
        }
        else        
        {
            bool die = R::runif(0, 1) <= deathRateAquatic;
            if (die)
            {
                mosquito.die();
            }
        }
    }
}

void Territory::updateMosquitoes()
{
    int deaths = 0;
    for (auto mosquito : mosquitoes)
    {
        Mosquito *tempMosquito = &mosquito;
        tempMosquito->updateAge();
        if (!tempMosquito->isAlive())
        {
            deaths++;
            mosquitoes.remove(mosquito);
        }
    }
};

void Territory::interaction(int _day)
{
    for (auto &human : humans)
    {
        Human *tempHuman = &human;
        for (auto &mosquito : mosquitoes)
        {
            Mosquito *tempMosquito = &mosquito;
            // Check if human and mosquito are in the same position
            if (checkProximity(tempHuman, tempMosquito))
            {
                // Try to bite
                if (tempMosquito->bite(tempHuman->getBiteRate()))
                {
                    // Could or not be infectuous
                    if (tempMosquito->isInfected())
                    {
                        // Mosquit to Human intection
                        if (tempMosquito->infectingBite(0.11, _day))
                        {
                            tempHuman->changeToInfected(_day);
                        }
                    }
                    else if (tempHuman->getViremia())
                    {
                        // Human to Mosquito intection
                        if (tempMosquito->infectiousBite(1))
                        {
                            tempMosquito->changeToInfected(_day);
                        }
                    }
                }
            }
        }
    }
};

bool Territory::checkProximity(Human *human, Mosquito *mosquito)
{
    bool sameLocation = false;
    if (human->getPositionX() == mosquito->getPositionX() && human->getPositionY() == mosquito->getPositionY())
    {
        sameLocation = true;
    }
    return sameLocation;
}

std::vector<int> Territory::contagions()
{
    std::vector<int> SIR = {0, 0, 0};
    for(auto human : humans){
        if(human.getState() == "I")
        {
            SIR[1] += 1;
        }
        else if(human.getState() == "R")
        {
            SIR[2] += 1;
        }
        else
        {
            SIR[0] += 1;
        }
    }
    return SIR;
};
