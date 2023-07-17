#ifndef MOSQUITO_H
#define	MOSQUITO_H

// Mosquito Class
#include <Rcpp.h>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>
#include <random>

using namespace std;
using std::vector;

class Mosquito{
    public:
        int getId() {return id;};
        bool isInfected() {return infected;};
        int getAge() {return age;};
        int getCurrentEnvironment() {return currentEnvironment;};
        int getPositionX() {return positionX;};
        int getPositionY() {return positionY;};
        int getDayOfInfection() {return dayOfInfection;};
        int getBiteCount() {return biteCount;};
        int getNeededBites() {return neededBites;};
        bool isAlive() {return alive;};
        bool isAdult() {return adult;};
        int getLifespan() {return lifespan;};
        float getDevelopmentRate() {return developmentRate;};

        bool bite(float);
        bool infectingBite(float, int);
        bool infectiousBite(float);
        void changeToInfected(int);
        void updateAge();
        void die();
        void move(int, int);
        void updateNumberOfBites();

        Mosquito(int, int , int, int , int, float, int);
        Mosquito();
        ~Mosquito();

    private:
        int id;
        bool infected;
        int age;
        int currentEnvironment;
        int positionX;
        int positionY;
        int dayOfInfection;
        int biteCount;
        int neededBites;
        int maxBitesPerDay;
        int bitesToday;
        bool alive;
        bool adult;
        int lifespan;
        float developmentRate;
};

#endif