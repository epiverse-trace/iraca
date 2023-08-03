#ifndef TERRITORY_H
#define TERRITORY_H

// Territory Class

#include <cstdlib>
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <Rcpp.h>
#include "Human.h"
// #include "Human.cpp"
#include "Mosquito.h"
// #include "Mosquito.cpp"
//#include <time.h>
//#include <random>

using namespace Rcpp;

class Territory
{
public:
    int getId() { return id; };
    float getArea() { return area; };
    int getWidth() { return width; };
    int getLength() { return length; };
    float getDensity() { return density; };
    int getPopulation() { return population; };
    float getGas() { return gas; };
    float getSewage() { return sewage; };
    std::vector<float> getAgeProp() {return ageProp; };
    float getMaleProp() {return maleProp; };
    float getHighEdProp() {return highEdProp; };
    float getNdvi() { return ndvi; };
    float getElevation() { return elevation; };
    double getBirthRate() { return birthRate; };
    double getDeathRateAdults() { return deathRateAdults; };
    double getDeathRateAquatic() { return deathRateAquatic; };
    std::list<Human> getHumans() {return humans; };
    std::list<Mosquito> getMosquitoes() {return mosquitoes; };
    std::vector<float> getMovementPatterns() {return movementPatterns; };

    Territory(int, float, float, int, float, float, std::vector<float>, float, float, std::vector<float>);
    Territory();

    void initializeHumans(float);
    void initializeMosquitoes(int, float, bool);
    void addHuman(Human);
    void removeHuman(Human);
    void resetHumans();
    void addMosquito(Mosquito);
    void removeMosquito(Mosquito);
    void updateDeathRates(float, int);
    void updateBirthRate(float, int);
    void moveHumans();
    void updateMosquitoes();
    void updateHumans(int);
    void interaction(int);
    bool checkProximity(Human, Mosquito);
    void moveMosquitoes();
    void birthMosquitoes(float, float);
    void deathMosquitoes(float, float);
    std::vector<int> contagions();
    int weightedRandom(std::vector<float>);
    std::list<Human> humans;
    std::list<Mosquito> mosquitoes;
private:
    int id;
    float area;
    int width;
    int length;
    float density;
    int population;
    std::vector<float> ageProp;
    float maleProp;
    float highEdProp;
    float gas;
    float sewage;
    float ndvi;
    float elevation;
    double birthRate;
    double deathRateAdults;
    double deathRateAquatic;
    std::list<int> intervenedAreas;
    std::vector<float> movementPatterns;

};

#endif