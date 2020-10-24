#include <iostream>
#include <memory>
#include <iomanip>
#include <fstream>

#include "solution.hpp"
#include "random_number_generator.hpp"

Solution::Solution() : totalRoutingCost(INF), vehicles(Data::getInstance().numVehicles) {
    for(int k = 0; k < (int)vehicles.size(); ++k) {
        vehicles[k] = std::make_unique < Vehicle > (Data::getInstance().dimContainers[k].first, Data::getInstance().dimContainers[k].second);
    }

    if((int)containerPositions.size() == 0) {
        for(int k = 0; k < (int)vehicles.size(); ++k) {
            for(int r = 0; r < vehicles[k]->R; ++r) {
                for(int l = 0; l < vehicles[k]->L; ++l) {
                    containerPositions.push_back(std::make_pair(k, std::make_pair(r, l)));
                }
            }
        }
    }
}

Solution& Solution::operator=(const Solution &other) {
    
    for(int k = 0; k < (int)vehicles.size(); ++k) *vehicles[k] = *other.vehicles[k];
    totalRoutingCost = other.totalRoutingCost;         
    return *this; 
}  

int Solution::getTotalRoutingCost() const {
    return totalRoutingCost;
}

void Solution::randomNeighborIS(Solution &randomNeighbor, FITNESS_EVALUATION fitnessEvaluation) {

    RandomNumberGenerator& randomNumberGenerator = RandomNumberGenerator::getInstance();

    for(int k = 0; k < (int)vehicles.size(); ++k) *randomNeighbor.vehicles[k] = *vehicles[k];     
    
    int k1, k2, r1, l1, r2, l2;

    k1 = randomNumberGenerator.generateRandomNumberUniformDistribution(0, (int)vehicles.size()-1);
    r1 = randomNumberGenerator.generateRandomNumberUniformDistribution(0, vehicles[k1]->R-1);
    l1 = randomNumberGenerator.generateRandomNumberUniformDistribution(0, vehicles[k1]->L-1);    
        
    do {                    
        k2 = randomNumberGenerator.generateRandomNumberUniformDistribution(0, (int)vehicles.size()-1);
        r2 = randomNumberGenerator.generateRandomNumberUniformDistribution(0, vehicles[k2]->R-1);
        l2 = randomNumberGenerator.generateRandomNumberUniformDistribution(0, vehicles[k2]->L-1);
    } while((k1 == k2 && r1 == r2 && l1 == l2) || (vehicles[k1]->container[r1][l1] == -1 && vehicles[k2]->container[r2][l2] == -1));

    std::swap(randomNeighbor.vehicles[k1]->container[r1][l1], randomNeighbor.vehicles[k2]->container[r2][l2]);

    randomNeighbor.evaluateSolution(fitnessEvaluation, k1, k2);    
}
    

void Solution::randomNeighborIC(Solution &randomNeighbor, FITNESS_EVALUATION fitnessEvaluation) {

    RandomNumberGenerator& randomNumberGenerator = RandomNumberGenerator::getInstance();

    for(int k = 0; k < (int)vehicles.size(); ++k) *randomNeighbor.vehicles[k] = *vehicles[k];

    int k1, k2, k3, r1, l1, r2, l2, r3, l3;

    k1 = randomNumberGenerator.generateRandomNumberUniformDistribution(0, (int)vehicles.size()-1);
    r1 = randomNumberGenerator.generateRandomNumberUniformDistribution(0, vehicles[k1]->R-1);
    l1 = randomNumberGenerator.generateRandomNumberUniformDistribution(0, vehicles[k1]->L-1);    

    do {                    
        k2 = randomNumberGenerator.generateRandomNumberUniformDistribution(0, (int)vehicles.size()-1);
        r2 = randomNumberGenerator.generateRandomNumberUniformDistribution(0, vehicles[k2]->R-1);
        l2 = randomNumberGenerator.generateRandomNumberUniformDistribution(0, vehicles[k2]->L-1);
    } while((k1 == k2 && r1 == r2 && l1 == l2) || (vehicles[k1]->container[r1][l1] == -1 && vehicles[k2]->container[r2][l2] == -1));

    do {                    
        k3 = randomNumberGenerator.generateRandomNumberUniformDistribution(0, (int)vehicles.size()-1);
        r3 = randomNumberGenerator.generateRandomNumberUniformDistribution(0, vehicles[k3]->R-1);
        l3 = randomNumberGenerator.generateRandomNumberUniformDistribution(0, vehicles[k3]->L-1);
    } while((k1 == k3 && r1 == r3 && l1 == l3) || (k2 == k3 && r2 == r3 && l2 == l3) || (vehicles[k1]->container[r1][l1] + vehicles[k2]->container[r2][l2] + vehicles[k3]->container[r3][l3] <= -2));

    std::swap(randomNeighbor.vehicles[k1]->container[r1][l1], randomNeighbor.vehicles[k3]->container[r3][l3]);
    std::swap(randomNeighbor.vehicles[k2]->container[r2][l2], randomNeighbor.vehicles[k3]->container[r3][l3]);

    randomNeighbor.evaluateSolution(fitnessEvaluation, k1, k2, k3);   
}

void Solution::randomNeighborSP(Solution &randomNeighbor, FITNESS_EVALUATION fitnessEvaluation) {

    RandomNumberGenerator& randomNumberGenerator = RandomNumberGenerator::getInstance();

    for(int k = 0; k < (int)vehicles.size(); ++k) *randomNeighbor.vehicles[k] = *vehicles[k]; 

    int k1, r1;

    k1 = randomNumberGenerator.generateRandomNumberUniformDistribution(0, (int)vehicles.size()-1);
    r1 = randomNumberGenerator.generateRandomNumberUniformDistribution(0, vehicles[k1]->R-1);
                
    shuffle(randomNeighbor.vehicles[k1]->container[r1].begin(), randomNeighbor.vehicles[k1]->container[r1].end(), randomNumberGenerator.getGenerator());
    
    randomNeighbor.evaluateSolution(fitnessEvaluation, k1); 
}   

void Solution::randomNeighborSS(Solution &randomNeighbor, FITNESS_EVALUATION fitnessEvaluation) {

    RandomNumberGenerator& randomNumberGenerator = RandomNumberGenerator::getInstance();

    for(int k = 0; k < (int)vehicles.size(); ++k) *randomNeighbor.vehicles[k] = *vehicles[k];
    
    int k1, k2, r1, r2;

    k1 = randomNumberGenerator.generateRandomNumberUniformDistribution(0, (int)vehicles.size()-1);
    r1 = randomNumberGenerator.generateRandomNumberUniformDistribution(0, vehicles[k1]->R-1);  

    do {                    
        k2 = randomNumberGenerator.generateRandomNumberUniformDistribution(0, (int)vehicles.size()-1);
        r2 = randomNumberGenerator.generateRandomNumberUniformDistribution(0, vehicles[k2]->R-1);        
    } while(k1 == k2);

    int smallerStackHeight = std::min(vehicles[k1]->L, vehicles[k2]->L);

    for(int l = 0; l < smallerStackHeight; ++l) {
        std::swap(randomNeighbor.vehicles[k1]->container[r1][l], randomNeighbor.vehicles[k2]->container[r2][l]);
    }

    randomNeighbor.evaluateSolution(fitnessEvaluation, k1, k2); 
}

void Solution::evaluateSolution(FITNESS_EVALUATION fitnessEvaluation, int k1, int k2, int k3) {

    totalRoutingCost = 0;

    for(int k = 0; k < (int)vehicles.size(); ++k) {
        if(k == k1 || k == k2 || k == k3 || k1 + k2 + k3 == -3) {
            vehicles[k]->getTraveledDistances(fitnessEvaluation);
        }
        totalRoutingCost += vehicles[k]->totalDistance;
    }    
}

void Solution::generateARandomSolution(FITNESS_EVALUATION fitnessEvaluation) {
    
    RandomNumberGenerator& randomNumberGenerator = RandomNumberGenerator::getInstance();

    const Data& data = Data::getInstance();

    std::vector < int > vt(data.numItems);
    for(int i = 0; i < (int)vt.size(); ++i) vt[i] = i+1;
    
    while((int)vt.size() < data.capacityOfFleet) {
        vt.push_back(-1); // insert fictitious items
    }

    shuffle(vt.begin(), vt.end(), randomNumberGenerator.getGenerator());     

    int position = 0;
    for(int k = 0; k < (int)vehicles.size(); ++k) {
        for(int r = 0; r < vehicles[k]->R; ++r) {
            for(int l = 0; l < vehicles[k]->L; ++l) {
                vehicles[k]->container[r][l] = vt[position++];
            }
        }        
    }    

    evaluateSolution(fitnessEvaluation);            
}

void Solution::generateARandomNeighbor(Solution &randomNeighbor, NEIGHBORHOOD_STRUCTURE neighborhoodStructure, FITNESS_EVALUATION fitnessEvaluation) {

    switch (neighborhoodStructure) {
        case NEIGHBORHOOD_STRUCTURE::IS:
            return randomNeighborIS(randomNeighbor, fitnessEvaluation);
        case NEIGHBORHOOD_STRUCTURE::IC:
            return randomNeighborIC(randomNeighbor, fitnessEvaluation);
        case NEIGHBORHOOD_STRUCTURE::SP:
            return randomNeighborSP(randomNeighbor, fitnessEvaluation);
        case NEIGHBORHOOD_STRUCTURE::SS:
            return randomNeighborSS(randomNeighbor, fitnessEvaluation);                
        default:
            std::clog << "Error! The neighborhood structure \" << neighborhoodStructure << \" is not defined" << std::endl;
            std::abort();
    }
}

void Solution::applyLocalSearchIS(FITNESS_EVALUATION fitnessEvaluation) {

    RandomNumberGenerator& randomNumberGenerator = RandomNumberGenerator::getInstance();

    int k1, k2, r1, l1, r2, l2;

    while (1) {

        shuffle(containerPositions.begin(), containerPositions.end(), randomNumberGenerator.getGenerator());
    
        bool improvement = false;
        
        for(int i = 0; i < containerPositions.size() && !improvement; ++i) {
            for(int j = i+1; j < containerPositions.size() && !improvement; ++j) {
                k1 = containerPositions[i].first; r1 = containerPositions[i].second.first; l1 = containerPositions[i].second.second;
                k2 = containerPositions[j].first; r2 = containerPositions[j].second.first; l2 = containerPositions[j].second.second;                
                if(k1 == k2 && r1 == r2 && l1 == l2) continue;
                if(vehicles[k1]->container[r1][l1] == -1 && vehicles[k2]->container[r2][l2] == -1) continue;
                int prevTotalRoutingCost = totalRoutingCost;
                Vehicle prevK1 = *vehicles[k1];
                Vehicle prevK2 = *vehicles[k2];                                               
                std::swap(vehicles[k1]->container[r1][l1], vehicles[k2]->container[r2][l2]);
                evaluateSolution(fitnessEvaluation, k1, k2);
                if(totalRoutingCost < prevTotalRoutingCost) {
                    improvement = true; break;
                }
                else {
                    std::swap(vehicles[k1]->container[r1][l1], vehicles[k2]->container[r2][l2]);
                    totalRoutingCost = prevTotalRoutingCost;
                    *vehicles[k1] = prevK1;
                    *vehicles[k2] = prevK2;                                              
                }
            }
        }

        if(improvement == false) break;
    }
}

void Solution::saveSolution(const std::string& outputFileName) {

    std::ofstream fout(outputFileName.c_str());
    fout << "Total routing cost: " << totalRoutingCost << std::endl << std::endl;
    for(int k = 0; k < (int)vehicles.size(); ++k) {
        fout << "Vehicle " << k + 1 << " (" << vehicles[k]->R << " x " << vehicles[k]->L << "):" << std::endl;
        for(int l = vehicles[k]->L-1; l >= 0; --l) {
            fout << "                  ";
            for(int r = 0; r < vehicles[k]->R; ++r) {
                if(vehicles[k]->container[r][l] == -1) {
                    fout << vehicles[k]->container[r][l] << ' ';
                }
                else {
                    fout << std::setfill('0') << std::setw(2) << vehicles[k]->container[r][l] << ' ';
                }
            }
            fout << std::endl;            
        }
        if((int)vehicles[k]->pickupTour.size() == 2) {
            fout << "\nPickup tour  : -1";
        }
        else {
            fout << "\nPickup tour  : 00";
            for(int i = 1; i < (int)vehicles[k]->pickupTour.size(); ++i) {
                fout << " --> " << std::setfill('0') << std::setw(2) << vehicles[k]->pickupTour[i];
            }
        }
        fout << std::endl;
        if((int)vehicles[k]->deliveryTour.size() == 2) {
            fout << "Delivery tour: -1";
        }
        else {
            fout << "Delivery tour: 00";
            for(int i = 1; i < (int)vehicles[k]->deliveryTour.size(); ++i) {
                fout << " --> " << std::setfill('0') << std::setw(2) << vehicles[k]->deliveryTour[i];
            }
        }
        fout << std::endl << std::endl;
    }    
    
    fout.close();
}
