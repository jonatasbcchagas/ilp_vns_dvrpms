#pragma once

#include <random>

class RandomNumberGenerator {

    private:
            std::default_random_engine generator;    
            unsigned seed;      

            RandomNumberGenerator() {};             

    public:

            static RandomNumberGenerator& getInstance() {
                static RandomNumberGenerator instance;                                        
                return instance;
            }
            
            RandomNumberGenerator(RandomNumberGenerator const&)            = delete;
            void operator=(RandomNumberGenerator const&)  = delete;

            ~RandomNumberGenerator() = default;
            std::default_random_engine& getGenerator();
            void setSeed(unsigned);
            unsigned getSeed() const;
            float generateRandomNumberUniformDistribution(float, float);
            double generateRandomNumberUniformDistribution(double, double);
            int generateRandomNumberUniformDistribution(int, int);            
            unsigned generateRandomNumberUniformDistribution(unsigned, unsigned); 
            bool generateRandomBooleanBiasedCoin(double);                  
            double generateRandomNumberNormalDistribution(double, double);     
};