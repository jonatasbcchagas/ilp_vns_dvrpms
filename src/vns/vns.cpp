#include <iostream>
#include <iomanip>
#include <fstream>
#include <chrono>

#include "vns.hpp"
#include "random_number_generator.hpp"

VNS::VNS() {}

void VNS::run(int numberIterWithoutImprovement, const std::string& outputFileName) {

    using namespace std::chrono;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    RandomNumberGenerator& randomNumberGenerator = RandomNumberGenerator::getInstance();

    std::vector < NEIGHBORHOOD_STRUCTURE > neighborhoodStructures { NEIGHBORHOOD_STRUCTURE::IS, 
                                                                    NEIGHBORHOOD_STRUCTURE::IC, 
                                                                    NEIGHBORHOOD_STRUCTURE::SP, 
                                                                    NEIGHBORHOOD_STRUCTURE::SS };

    int kMax = (int) neighborhoodStructures.size();

    std::ofstream fout((outputFileName + ".log").c_str());
    fout << "Total routing cost               Time(s)" << std::endl;

    Solution sLine;
    s.generateARandomSolution(FITNESS_EVALUATION::HEURISTIC);

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration < double > time_span = duration_cast < duration < double > >(t2 - t1);

    fout << std::fixed << std::setw(18) << std::setprecision(0) << s.getTotalRoutingCost() << " " << std::fixed << std::setw(21) << std::setprecision(3) << (double)time_span.count() << std::endl;
    
    int iter = 0;

    while(iter < numberIterWithoutImprovement) {
        int k = 1;
        while(1) {
            s.generateARandomNeighbor(sLine, neighborhoodStructures[k-1], FITNESS_EVALUATION::HEURISTIC);
            sLine.applyLocalSearchIS(FITNESS_EVALUATION::HEURISTIC);
            if(sLine.getTotalRoutingCost() < s.getTotalRoutingCost()) {
                s = sLine;
                k = 1;
                iter = 0;        
                t2 = high_resolution_clock::now();
                time_span = duration_cast<duration<double> >(t2 - t1);
                fout << std::fixed << std::setw(18) << std::setprecision(0) << s.getTotalRoutingCost() << " " << std::fixed << std::setw(21) << std::setprecision(3) << time_span.count() << std::endl;
            }
            else {
                k += 1;
                iter += 1;
            }
            if(k > kMax) break;
        }
    }

    t2 = high_resolution_clock::now();
    time_span = duration_cast < duration < double > >(t2 - t1);
    fout << std::fixed << std::setw(18) << std::setprecision(0) << s.getTotalRoutingCost() << " " << std::fixed << std::setw(21) << std::setprecision(3) << time_span.count() << " (FITNESS_EVALUATION::HEURISTIC)" << std::endl;

    s.evaluateSolution(FITNESS_EVALUATION::EXACT);
    
    t2 = high_resolution_clock::now();
    time_span = duration_cast < duration < double > >(t2 - t1);
    fout << std::fixed << std::setw(18) << std::setprecision(0) << s.getTotalRoutingCost() << " " << std::fixed << std::setw(21) << std::setprecision(3) << time_span.count() << " (FITNESS_EVALUATION::EXACT)" << std::endl;

    s.saveSolution(outputFileName);

    t2 = high_resolution_clock::now();
    time_span = duration_cast<duration<double> >(t2 - t1);

    fout << std::endl << "Total runtime: " << std::fixed << std::setw(21) << std::setprecision(3) << time_span.count() << " secs" << std::endl;

    fout.close();
}
