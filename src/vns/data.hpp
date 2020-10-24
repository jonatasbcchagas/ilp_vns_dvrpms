#pragma once

#include <vector>
#include <string>

const int INF = 987654321;

class Data {

    public:

            static Data& getInstance() {
                static Data instance;                                        
                return instance;
            }
            
            Data(Data const&)            = delete;
            void operator=(Data const&)  = delete;

            int numPoints;
            int numItems;
            int capacityOfFleet;
            int **pickupDistances, **deliveryDistances;
            int DEPOT;
            int numVehicles;
            std::vector < std::pair < int, int > > dimContainers; // < R_1, L_1 >,  R_2, L_2 >, ...,  R_k, L_k >
            
            void readData(const std::string& pickupRegionFileName, const std::string& deliveryRegionFileName, const std::string& containerConfigsFileName, int _numItems);

    private:

            Data() {};
};
