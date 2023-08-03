#ifndef SIMULATION_H
#define SIMULATION_H

// Simulation Class

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iterator>
#include <Rcpp.h>
#include "Territory.h"

using namespace Rcpp;

class Simulation
{
public:
    int getDays() {return days; };
    std::map<int, Territory> getTerritories() {return territories; };
    std::vector<std::vector<float>> getTerritoriesData() {return territoriesData; };
    std::vector<std::vector<float>> getTemperatureData() {return temperatureData; };
    std::vector<std::vector<float>> getMovementData() {return movementData; };

    Simulation(int, std::vector<std::vector<float>>, std::vector<std::vector<float>>, std::vector<std::vector<float>>);
    ~Simulation();

    void initialize();
    Rcpp::DataFrame simulate(int);
    void moveHumans();

private:
    int days;
    std::map<int, Territory> territories;
    std::vector<std::vector<float>> territoriesData;
    std::vector<std::vector<float>> temperatureData;
    std::vector<std::vector<float>> movementData;
    std::list<Human> transitHumans;

};

#endif