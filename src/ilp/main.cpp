#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdio>

#include "../vns/data.hpp"
#include "ilp.hpp"

inline void usage() { 
    std::clog << "\nUsage ./ilp [arguments]" << std::endl << std::endl;
    std::clog << "arguments:" << std::endl;
    std::clog << "           --pickuparea <pickup_area_file_name>" << std::endl;
    std::clog << "           --deliveryarea <delivery_area_file_name>" << std::endl;
    std::clog << "           --container <container_file_name>" << std::endl;
    std::clog << "           --n <number_of_items>" << std::endl;
    std::clog << "           --outputsolution <solution_file_name> " << std::endl;          
    exit(0);
}

int main(int argc, char** argv) {

    if(argc < 11) usage();

    int numItems, numberIterWithoutImprovement, seed;
    char parameterStr[1000];
    std::string pickupAreaFileName, deliveryAreaFileName, containerConfigsFileName, outputSolutionFileName;    
    
    int check_parameters = 0;
    for(int i = 1; i < argc; i += 2) { 
        if(strcmp(argv[i], "--pickuparea") == 0) { pickupAreaFileName = std::string(argv[i+1]); check_parameters += 1; }
        else if(strcmp(argv[i], "--deliveryarea") == 0) { deliveryAreaFileName = std::string(argv[i+1]); check_parameters += 1; }
        else if(strcmp(argv[i], "--container") == 0) { containerConfigsFileName = std::string(argv[i+1]); check_parameters += 1; }
        else if(strcmp(argv[i], "--n") == 0) { sscanf(argv[i+1],"%d", &numItems); check_parameters += 1; }
        else if(strcmp(argv[i], "--outputsolution") == 0) { outputSolutionFileName = std::string(argv[i+1]); check_parameters += 1; }
        else check_parameters = -INF;        
    }
    
    if(check_parameters != 5) usage();

    Data::getInstance().readData(pickupAreaFileName, deliveryAreaFileName, containerConfigsFileName, numItems);
    
    solveILPModel(outputSolutionFileName);
    
    return 0;
}
