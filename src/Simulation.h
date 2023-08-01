#ifndef SIMULATION_H
#define SIMULATION_H

// Simulation Class

#include <iostream>
#include <Rcpp.h>
#include <omp.h>
#include <cmath>
#include <sstream>
#include <fstream>
#include <string>
#include <iterator>
#include "Territory.h"
#include "Territory.cpp"

class Simulation
{
public:
    int getDays() {return days; };
    std::map<int, Territory> getTerritories() {return territories; };
    Rcpp::DataFrame getTerritoriesData() {return territoriesData; };
    Rcpp::DataFrame getTemperatureData() {temperatureData; };
    Rcpp::DataFrame getMovementData() {return movementData; };

    Simulation(int, Rcpp::DataFrame, Rcpp::DataFrame, Rcpp::DataFrame);

    void initialize();
    Rcpp::DataFrame simulate(int);
    void moveHumans();



private:
    int days;
    std::map<int, Territory> territories;
    Rcpp::DataFrame territoriesData;
    Rcpp::DataFrame temperatureData;
    Rcpp::DataFrame movementData;
    std::list<Human> transitHumans;

};

#endif