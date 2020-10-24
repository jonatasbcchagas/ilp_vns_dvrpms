#pragma once

#include <algorithm>

#include "data.hpp"

enum class FITNESS_EVALUATION { HEURISTIC, EXACT };
enum class AREA { PICKUP, DELIVERY };

class Vehicle {

    friend class Solution;

    private:

            int R, L;
            int totalDistance;        

            std::vector < std::vector < int > > container;            
            std::vector < int > pickupTour;
            std::vector < int > deliveryTour;
            
            int computeDistanceHeuristicStrategy(AREA area);
            int computeDistanceDynamicProgramming(AREA area);            

            int getTraveledPickupDistanceHeuristicStrategy();
            int getTraveledDeliveryDistanceHeuristicStrategy();
            int getTraveledPickupDistanceDynamicProgramming();
            int getTraveledDeliveryDistanceDynamicProgramming();

    public:
        
            Vehicle(int _R, int _L);
            Vehicle(const Vehicle &other);
            Vehicle& operator=(const Vehicle &other);
            
            void getTraveledDistances(FITNESS_EVALUATION fitnessEvaluation);
};