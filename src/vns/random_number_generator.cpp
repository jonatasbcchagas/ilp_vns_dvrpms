#include "random_number_generator.hpp"

std::default_random_engine& RandomNumberGenerator::getGenerator() {
    return generator;
}

void RandomNumberGenerator::setSeed(unsigned _seed) {
    seed = _seed;
    generator.seed(seed);
}

unsigned RandomNumberGenerator::getSeed() const {
    return seed;
}

float RandomNumberGenerator::generateRandomNumberUniformDistribution(float _min, float _max) {
    std::uniform_real_distribution < float > distribution(_min, _max);    
    return distribution(generator);
}

double RandomNumberGenerator::generateRandomNumberUniformDistribution(double _min, double _max) {
    std::uniform_real_distribution < double > distribution(_min, _max);    
    return distribution(generator);
}

int RandomNumberGenerator::generateRandomNumberUniformDistribution(int _min, int _max) {
    std::uniform_int_distribution < int > distribution(_min, _max);
    return distribution(generator);
}

unsigned RandomNumberGenerator::generateRandomNumberUniformDistribution(unsigned _min, unsigned _max) {
    std::uniform_int_distribution < unsigned > distribution(_min, _max);
    return distribution(generator);
}

bool RandomNumberGenerator::generateRandomBooleanBiasedCoin(double probability) {
    std::bernoulli_distribution distribution(probability);
    return distribution(generator);
}

double RandomNumberGenerator::generateRandomNumberNormalDistribution(double _min, double _max) {
    std::normal_distribution < double > distribution(_min, _max);    
    return distribution(generator);
}