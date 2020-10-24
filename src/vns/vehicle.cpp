#include <iostream>
#include <unordered_map>
#include <map>

#include "vehicle.hpp"
#include "random_number_generator.hpp"

Vehicle::Vehicle(int _R, int _L) {

    R = _R; L = _L;
    totalDistance = INF;
    
    container.resize(R);
    for(int r = 0; r < R; ++r) container[r].resize(L);
}

Vehicle::Vehicle(const Vehicle &other) {
    R = other.R; L = other.L;
    container = other.container;
    pickupTour = other.pickupTour;
    deliveryTour = other.deliveryTour;
    totalDistance = other.totalDistance; 
}

Vehicle& Vehicle::operator=(const Vehicle &other) {
    R = other.R; L = other.L;
    container = other.container;
    pickupTour = other.pickupTour;
    deliveryTour = other.deliveryTour;
    totalDistance = other.totalDistance;   
}

int Vehicle::computeDistanceHeuristicStrategy(AREA area) {

    const Data& data = Data::getInstance();

    int alpha = ((area == AREA::PICKUP) ? 1 : 0);

    int numStoredItems = 0;
    std::vector < int > stackOccupations(R);
    for(int r = 0; r < R; ++r) {
        int _l = 0;
        for(int l = 0; l < L; ++l) {
            if(container[r][l] == -1) continue;
            numStoredItems += 1;
            if(_l != l) {
                container[r][_l] = container[r][l];
                container[r][l] = -1;               
            }
            _l += 1;
        }
        stackOccupations[r] = _l;
    }  

    if(numStoredItems == 0) {
        if(alpha == 1) pickupTour = std::vector < int > {data.DEPOT, data.DEPOT}; 
        else deliveryTour = std::vector < int > {data.DEPOT, data.DEPOT};  
        return 0;    
    }
    
    std::vector < int > tops(R);
    for(int r = 0; r < R; ++r) tops[r] = stackOccupations[r] - 1;
    
    std::vector < int > tour;    
    int prevCity = data.DEPOT;
    int shortestDistance, chosenStack;

    std::vector < int > stackOperations;

    while(1) {
        shortestDistance = INF;
        chosenStack = -1;
        for(int r = 0; r < R; ++r) {
            if(tops[r] < 0) continue;
            int nextCity = container[r][tops[r]];
            if(alpha * data.pickupDistances[prevCity][nextCity] + (1 - alpha) * data.deliveryDistances[prevCity][nextCity] < shortestDistance) {
                shortestDistance = alpha * data.pickupDistances[prevCity][nextCity] + (1 - alpha) * data.deliveryDistances[prevCity][nextCity];
                chosenStack = r;
            }
        }
        if(chosenStack == -1) break;
        
        stackOperations.push_back(chosenStack);
        prevCity = container[chosenStack][tops[chosenStack]];
        tops[chosenStack] -= 1;
    }

    auto evaluateOverStackOperations = [&]() { 
        for(int r = 0; r < R; ++r) tops[r] = stackOccupations[r] - 1;    
        prevCity = data.DEPOT;
        int distance = 0;
        for(int i = 0; i < (int)stackOperations.size(); ++i) {
            int r = stackOperations[i];
            int nextCity = container[r][tops[r]];
            distance += alpha * data.pickupDistances[prevCity][nextCity] + (1 - alpha) * data.deliveryDistances[prevCity][nextCity];
            prevCity = nextCity;
            tops[r] -= 1;
        }
       distance += alpha * data.pickupDistances[prevCity][data.DEPOT] + (1 - alpha) * data.deliveryDistances[prevCity][data.DEPOT];
       return distance;
    };

    auto updateTour = [&]() { 
        tour.clear();
        for(int r = 0; r < R; ++r) tops[r] = stackOccupations[r] - 1;    
        tour.push_back(data.DEPOT);
        for(int i = 0; i < (int)stackOperations.size(); ++i) {
            int r = stackOperations[i];
            int nextCity = container[r][tops[r]];
            tour.push_back(nextCity);
            tops[r] -= 1;
        }
        tour.push_back(data.DEPOT);
    };

    int distance = evaluateOverStackOperations();
    updateTour();

    std::vector < int > vt(stackOperations.size());
    for(int i = 0; i < (int)vt.size(); ++i) vt[i] = i;

    RandomNumberGenerator& randomNumberGenerator = RandomNumberGenerator::getInstance(); 

    while(1) {

        shuffle(vt.begin(), vt.end(), randomNumberGenerator.getGenerator());
    
        bool improvement = false;
        
        for(int i = 0; i < vt.size() && !improvement; ++i) {
            for(int j = i+1; j < vt.size() && !improvement; ++j) {
                if(stackOperations[vt[i]] == stackOperations[vt[j]]) continue;
                int prevDistance = distance;
                std::swap(stackOperations[vt[i]], stackOperations[vt[j]]);
                distance = evaluateOverStackOperations();
                if(distance < prevDistance) {
                    improvement = true; 
                    updateTour(); break;
                }
                else {
                    std::swap(stackOperations[vt[i]], stackOperations[vt[j]]);
                    distance = prevDistance;
                }
            }
        }

        if(improvement == false) break;
    }    

    if(alpha == 1) pickupTour.swap(tour); 
    else deliveryTour.swap(tour);

    return distance;
}

int Vehicle::computeDistanceDynamicProgramming(AREA area) {

    const Data& data = Data::getInstance();

    int alpha = ((area == AREA::PICKUP) ? 1 : 0);

    int numStoredItems = 0;
    std::vector < int > stackOccupations(R);
    for(int r = 0; r < R; ++r) {
        int _l = 0;
        for(int l = 0; l < L; ++l) {
            if(container[r][l] == -1) continue;
            numStoredItems += 1;
            if(_l != l) {
                container[r][_l] = container[r][l];
                container[r][l] = -1;               
            }
            _l += 1;
        }
        stackOccupations[r] = _l;
    }  

    if(numStoredItems == 0) {
        if(alpha == 1) pickupTour = std::vector < int > {data.DEPOT, data.DEPOT}; 
        else deliveryTour = std::vector < int > {data.DEPOT, data.DEPOT};   
        return 0;   
    }
    
    std::vector < int > tops(R);
    int numStages = 0;
    for(int r = 0; r < R; ++r) {
        tops[r] = stackOccupations[r] - 1;
        numStages += stackOccupations[r];
    }
    
    std::vector < std::map < std::vector < int >, std::pair < std::vector < int > , int > > > stages(numStages);

    int stageID = 0;
    std::vector < int > subproblem = stackOccupations;
    subproblem.push_back(-1);
    for(int r = 0; r < R; ++r) {   
        if(subproblem[r] ==  0) continue;     
        subproblem[r] -= 1; subproblem.back() = r;
        int nextCity = container[r][subproblem[r]];
        int distance = alpha * data.pickupDistances[data.DEPOT][nextCity] + (1 - alpha) * data.deliveryDistances[data.DEPOT][nextCity];
        stages[stageID][subproblem] = std::make_pair(std::vector < int > {nextCity}, distance);
        subproblem[r] += 1;
    }
    
    stageID += 1;
    for(; stageID < numStages; ++stageID) {
        for(const auto& x : stages[stageID-1]) {
            std::vector < int > subproblem = x.first;
            int prevCity = x.second.first.back();
            for(int r = 0; r < R; ++r) {
                if(subproblem[r] ==  0) continue;
                subproblem[r] -= 1; subproblem.back() = r;                
                int nextCity = container[r][subproblem[r]];
                int distance = x.second.second;                
                distance += alpha * data.pickupDistances[prevCity][nextCity] + (1 - alpha) * data.deliveryDistances[prevCity][nextCity];
                auto it = stages[stageID].find(subproblem);
                if(it == stages[stageID].end()) {
                    std::vector < int > partialTour = x.second.first;
                    partialTour.push_back(nextCity);
                    stages[stageID][subproblem] = std::make_pair(partialTour, distance);
                }
                else if(distance < it->second.second) {
                    std::vector < int > partialTour = x.second.first;
                    partialTour.push_back(nextCity);
                    it->second = std::make_pair(partialTour, distance);
                }                
                subproblem[r] += 1;
            }
        }
    }

    int distance = INF;
    std::vector < int > tour;
    for(const auto& x : stages.back()) {
        if(distance > x.second.second + alpha * data.pickupDistances[x.second.first.back()][data.DEPOT] + (1 - alpha) * data.deliveryDistances[x.second.first.back()][data.DEPOT]) {
            distance = x.second.second + alpha * data.pickupDistances[x.second.first.back()][data.DEPOT] + (1 - alpha) * data.deliveryDistances[x.second.first.back()][data.DEPOT];
            tour = x.second.first;
        }
    }
    tour.insert(tour.begin(), data.DEPOT);    
    tour.push_back(data.DEPOT);

    if(alpha == 1) pickupTour.swap(tour); 
    else deliveryTour.swap(tour);      
    
    return distance;
}

int Vehicle::getTraveledPickupDistanceHeuristicStrategy() { 

    for(int r = 0; r < R; ++r) {
        reverse(container[r].begin(), container[r].end());
    }
    int ans = computeDistanceHeuristicStrategy(AREA::PICKUP);
    for(int r = 0; r < R; ++r) {
        reverse(container[r].begin(), container[r].end());
    }
    return ans;
}

int Vehicle::getTraveledDeliveryDistanceHeuristicStrategy() { 

    return computeDistanceHeuristicStrategy(AREA::DELIVERY);    
}

int Vehicle::getTraveledPickupDistanceDynamicProgramming() {
    for(int r = 0; r < R; ++r) {
        reverse(container[r].begin(), container[r].end());
    }
    int ans = computeDistanceDynamicProgramming(AREA::PICKUP);
    for(int r = 0; r < R; ++r) {
        reverse(container[r].begin(), container[r].end());
    }
    return ans;
}

int Vehicle::getTraveledDeliveryDistanceDynamicProgramming() { 

    return computeDistanceDynamicProgramming(AREA::DELIVERY); 
}
 
void Vehicle::getTraveledDistances(FITNESS_EVALUATION fitnessEvaluation) {
                    
    totalDistance = 0;

    switch(fitnessEvaluation) {
        case FITNESS_EVALUATION::HEURISTIC: 
            totalDistance += getTraveledPickupDistanceHeuristicStrategy();
            totalDistance += getTraveledDeliveryDistanceHeuristicStrategy();
            break;
        case FITNESS_EVALUATION::EXACT:
            totalDistance += getTraveledPickupDistanceDynamicProgramming();
            totalDistance += getTraveledDeliveryDistanceDynamicProgramming();
            break;
        default:
            std::clog << "Error! The fitness evaluation \" << fitnessEvaluation << \" is not defined" << std::endl;
            std::abort();
    }    
}
