#pragma once

#include <memory>

#include "data.hpp"
#include "vehicle.hpp"

enum class NEIGHBORHOOD_STRUCTURE { IS, IC, SP, SS };

class Solution {

    private:
            
            std::vector < std::unique_ptr < Vehicle > > vehicles;            
            int totalRoutingCost;

            void randomNeighborIS(Solution &randomNeighbor, FITNESS_EVALUATION fitnessEvaluation);
            void randomNeighborIC(Solution &randomNeighbor, FITNESS_EVALUATION fitnessEvaluation);
            void randomNeighborSP(Solution &randomNeighbor, FITNESS_EVALUATION fitnessEvaluation);
            void randomNeighborSS(Solution &randomNeighbor, FITNESS_EVALUATION fitnessEvaluation);

            inline static std::vector < std::pair < int, std::pair < int, int > > > containerPositions; // [< k, < r, l >, ...]
                                
    public:
            
            Solution();
            Solution& operator=(const Solution &other);

            int getTotalRoutingCost() const;
            void evaluateSolution(FITNESS_EVALUATION fitnessEvaluation, int k1 = -1, int k2 = -1, int k3 = -1);
            void generateARandomSolution(FITNESS_EVALUATION fitnessEvaluation);
            void generateARandomNeighbor(Solution &randomNeighbor, NEIGHBORHOOD_STRUCTURE neighborhoodStructure, FITNESS_EVALUATION fitnessEvaluation);
            void applyLocalSearchIS(FITNESS_EVALUATION fitnessEvaluation);

            void saveSolution(const std::string& outputFileName);
};