#pragma once

#include "data.hpp"
#include "solution.hpp"

class VNS {

    private:

            Solution s; // best solution found

    public:
            
            VNS();           
            void run(int numberIterWithoutImprovement, const std::string& outputFileName);
};