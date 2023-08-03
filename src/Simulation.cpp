
// Simulation Class

#include "Simulation.h"

Simulation::Simulation(int _days, std::vector<std::vector<float>> _territoriesData, std::vector<std::vector<float>> _temperatureData, std::vector<std::vector<float>> _movementData)
{
    days = _days;
    territoriesData = _territoriesData;
    temperatureData = _temperatureData;
    movementData = _movementData;
    std::map<int, Territory> territories;
    std::list<Human> transitHumans;
}

Simulation::~Simulation(){
}

void Simulation::initialize()
{
    // data input
    for (int ter = 0; ter < int(territoriesData.size()) - 1; ter++)
    {
        // Environment attributes from DANE database
        float density = territoriesData[ter][45];
        int population = territoriesData[ter][32];
        float gas = territoriesData[ter][29];
        float sewage = territoriesData[ter][30];
        float area = territoriesData[ter][1];
        std::vector<float> ageProp = {territoriesData[ter][35],
                                      territoriesData[ter][36],
                                      territoriesData[ter][37],
                                      territoriesData[ter][38],
                                      territoriesData[ter][39],
                                      territoriesData[ter][40],
                                      territoriesData[ter][41],
                                      territoriesData[ter][42],
                                      territoriesData[ter][43]};
        float maleProp = territoriesData[ter][33];
        float highEdProp = territoriesData[ter][44];
        std::vector<float> movementPatterns = movementData[ter];
        // Territory creation
        Territory newTerritory(ter, area, density, population, gas, sewage, ageProp, maleProp, highEdProp, movementPatterns);

        // Internal intialization
        newTerritory.initializeHumans(0.005);
        newTerritory.initializeMosquitoes(int(population * 1.3 * 2.1 / 2.7), 0.01, false);

        // Updating list
        territories.insert(std::pair<int, Territory>(ter, newTerritory));
    }
}

Rcpp::DataFrame Simulation::simulate(int _ndays)
//void Simulation::simulate(int _ndays)
{
    initialize();
    std::vector<int> vectorS = {};
    std::vector<int> vectorI = {};
    std::vector<int> vectorR = {};
    for (int day = 0; day < _ndays; day++)
    {
        int suceptible = 0;
        int infected = 0;
        int recovered = 0;
        if(day == 0){
            for (auto &territory : territories)
            {
                Territory * tempTerritory = &territory.second;
                for(auto &human : tempTerritory->humans)
                {
                    int dailyEnv = human.getDailyTerritory();
                    int _positionX = int(R::runif(0, territories[dailyEnv].getLength()));
                    int _positionY = int(R::runif(0, territories[dailyEnv].getWidth()));
                    //_positionX = distrHumanX(gen);
                    //_positionY = distrHumanY(gen);
                    human.setDailyCoordinates(_positionX, _positionY);   ////RETHINK THIS
                }
            }
        }
        for (int time = 0; time < 2; time++)
        {
            for (auto &territory : territories) // paralelizable
            {
                Territory *tempTerritory = &territory.second;
                tempTerritory->interaction(day); //, temperatureData[0, day], temperatureData[1, day]);
            }
            for (auto &territory : territories)
            {
                Territory *tempTerritory = &territory.second;
                tempTerritory->moveMosquitoes();
                tempTerritory->moveHumans();

                transitHumans.assign(tempTerritory->humans.begin(), tempTerritory->humans.end());
                tempTerritory->resetHumans();
            }
            for (auto human : transitHumans)
            {
                territories[human.getcurrentTerritory()].addHuman(human);
            }
            transitHumans.clear();
        }
        for (auto &territory : territories)
        {
            Territory *tempTerritory = &territory.second;
            std::vector<int> localSIR = tempTerritory->contagions();
            suceptible += localSIR[0]; infected += localSIR[1]; recovered += localSIR[2];
            tempTerritory->updateMosquitoes();
            tempTerritory->updateHumans(day);
            tempTerritory->deathMosquitoes(temperatureData[0][day], temperatureData[1][day]);
            tempTerritory->birthMosquitoes(temperatureData[0][day], temperatureData[1][day]);
            vectorS.push_back(suceptible);
            vectorI.push_back(infected);
            vectorR.push_back(recovered);
        }
    }
   Rcpp::DataFrame SIRM = Rcpp::DataFrame::create(Named("S") = vectorS, Named("I") = vectorI, Named("R") = vectorR);
   return SIRM;
}
