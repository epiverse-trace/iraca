// Mosquito Class

#include <iostream>
#include <cstdlib>
#include <map>
#include <vector>
#include <time.h>
#include "Mosquito.h"

using namespace std;
using std::vector;

// Random numbers
std::random_device rd;
std::mt19937 gen(static_cast<unsigned int>(time(nullptr)));

Mosquito::Mosquito(int _id, int _age, int _positionX, int _positionY, int _currentEnvironment, float _developmentRate, int _lifespan)
{
    id = _id;
    infected = false;
    age = _age;
    currentEnvironment = _currentEnvironment;
    positionX = _positionX;
    positionY = _positionY;
    dayOfInfection = 99999;
    biteCount = 0;
    neededBites = 10;
    maxBitesPerDay = 4;
    bitesToday = 0;
    alive = true;
    adult = (_age * _developmentRate >= 1) ? true : false;
    lifespan = _lifespan;
    developmentRate = _developmentRate;
};

Mosquito::Mosquito(){};

Mosquito::~Mosquito(){};

// Mosquito bite
bool Mosquito::bite(float _biteProbability)
{
    bool output = false;
    if (adult)
    {
        if (biteCount <= neededBites && bitesToday <= maxBitesPerDay)
        {
            if (Rcpp::runif(1) <= _biteProbability)
            {
                // if bites, update bite count
                updateNumberOfBites();
                output = true;
            }
        }
    }
    return output;
};

// Is bite infecting? (Human to Mosquito)
bool Mosquito::infectingBite(float _infectingProb, int _day)
{
    bool output = false;
    if (dayOfInfection != 0 && _day - dayOfInfection > 5)
    {
        output = (Rcpp::runif(1) <= _infectingProb ); // rate
    }
    return output;
};

// Is bite infectious? (Mosquito to Human)
bool Mosquito::infectiousBite(float _infectiousProb)
{
    return (Rcpp::runif(1) <= _infectiousProb); // rate
};

// Change state
void Mosquito::changeToInfected(int _day)
{
    dayOfInfection = _day;
    infected = true;
};

// Update age (die if older than lifespan)
void Mosquito::updateAge()
{
    bitesToday = 0;
    age++;
    if (developmentRate * age >= 1)
    {
        adult = true;
    }
    if (age >= lifespan)
    {
        die();
    }
};

// Change alive state
void Mosquito::die()
{
    alive = false;
};

// Movement
void Mosquito::move(int _newX, int _newY)
{
    positionX = _newX;
    positionY = _newY;
};

// Bite count
void Mosquito::updateNumberOfBites()
{
    bitesToday++;
    biteCount++;
};
