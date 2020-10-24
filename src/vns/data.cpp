#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstdio>

#include "data.hpp"
    
void Data::readData(const std::string& pickupRegionFileName, const std::string& deliveryRegionFileName, const std::string& containerConfigsFileName, int _numItems) {

    numItems = _numItems;

    std::vector < std::pair < long double, long double> > points;
    int id;
    long double x, y;
    DEPOT = 0;
    
    std::string line;
    std::ifstream fin(pickupRegionFileName.c_str());

    if(!fin) { std::clog << "ERROR! File \"" << pickupRegionFileName << "\" not found." << std::endl; std::abort(); }

    for(int i = 0; i < 6; ++i) {
        getline(fin, line);
        if(line.find("DIMENSION") != std::string::npos) {
            for(int j = 0; j < (int)line.length(); ++j) {
                if(line[j] < '0' || line[j] > '9') line[j] = ' ';
            }
            std::stringstream ss;
            ss << line;
            ss >> numPoints;
        }
    }

    pickupDistances = new int*[numPoints];
    for(int i = 0; i < numPoints; ++i) {
        pickupDistances[i] = new int[numPoints];
    }

    while(fin >> id >> x >> y) {
        points.push_back(std::make_pair(x, y));
    }

    for(int i = 0; i < (int)points.size(); ++i) {
        for(int j = i; j <(int)points.size(); ++j) {
            pickupDistances[i][j] = pickupDistances[j][i] = (int) (0.5 +
                                                                       sqrt(
                                                                            ((points[i].first-points[j].first)*(points[i].first-points[j].first)) +
                                                                            ((points[i].second-points[j].second)*(points[i].second-points[j].second))
                                                                       ));
        }
    }
    fin.close();

    fin.open(deliveryRegionFileName.c_str());

    if(!fin) { std::clog << "ERROR! File \"" << deliveryRegionFileName << "\" not found." << std::endl; std::abort(); }

    for(int i = 0; i < 6; ++i) { getline(fin, line); }

    points.clear();

    while(fin >> id >> x >> y) {
        points.push_back(std::make_pair(x, y));
    }

    deliveryDistances = new int*[numPoints];
    for(int i = 0; i < numPoints; ++i) {
        deliveryDistances[i] = new int[numPoints];
    }

    for(int i = 0; i < (int)points.size(); ++i) {
        for(int j = i; j <(int)points.size(); ++j) {
            deliveryDistances[i][j] = deliveryDistances[j][i] = (int) (0.5 +
                                                                       sqrt(
                                                                            ((points[i].first-points[j].first)*(points[i].first-points[j].first)) +
                                                                            ((points[i].second-points[j].second)*(points[i].second-points[j].second))
                                                                       ));
        }
    }
    fin.close();

    fin.open(containerConfigsFileName.c_str());

    if(!fin) { std::clog << "ERROR! File \"" << containerConfigsFileName << "\" not found." << std::endl; std::abort(); }

    fin >> numVehicles;
    int r, l;

    capacityOfFleet = 0;
    while(fin >> r >> l) {
        capacityOfFleet += r * l;
        dimContainers.push_back(std::make_pair(r, l));
    }
    
    if(numItems > capacityOfFleet) { std::clog << "ERROR! Number of items must be less than the total capacity of the vehicles." << std::endl; std::abort(); }

    fin.close();
}
