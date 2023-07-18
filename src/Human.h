#ifndef HUMAN_H
#define HUMAN_H

// Human Class
#include <Rcpp.h>
#include <iostream>
#include <cstdlib>
#include <map>
#include <vector>

using namespace Rcpp;

class Human
{
public:
    int getId() { return id; };
    std::string getState() { return state; };
    int getAge() { return age; };
    std::string getGender() { return gender; };
    int getHighEd() { return highEd; };
    int getcurrentEnvironment() { return currentEnvironment; };
    int getPositionX() { return positionX; };
    int getPositionY() { return positionY; };
    int getHomeEnvironment() { return homeEnvironment; };
    int getDailyEnvironment() { return dailyEnvironment; };
    int getDayOfInfection() { return dayOfInfection; };
    bool getViremia() { return viremia; };
    float getBiteRate() { return biteRate; };

    vector<int> getHomeCoordinates() { return homeCoordinates; };
    vector<int> getDailyCoordinates() { return dailyCoordinates; };

    void changeToInfected(int);
    void changeToRecovered();
    void setDayOfInfection(int);
    void updateViremia(int);
    void changeEnvironment(int);
    void setDailyCoordinates(int, int);
    void setHomeCoordinates(int, int);
    void updatePosition(int, int, int);
    void updateBiteRate(float);

    Human(int, int, std::string, int, int, int, float);
    Human();

private:
    int id;
    std::string state;
    int age;
    std::string gender;
    bool highEd;
    int currentEnvironment;
    int positionX;
    int positionY;
    int homeEnvironment;
    int dailyEnvironment;
    int dayOfInfection;
    bool viremia;
    float biteRate;
    std::vector<int> homeCoordinates;
    std::vector<int> dailyCoordinates;
};

#endif