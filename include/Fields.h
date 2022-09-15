#ifndef FIELDS_H
#define FIELDS_H

#include <cmath>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <iostream>
#include "Points.h"

class Fields{
    private:
        std::vector<myPoint> originalCluster;
        int numClusters;
        std::map<int, std::multimap<float, myPoint>> fields;
        std::vector<int> clusterIdxs;
        std::string typeOfData;
        bool isMRZ;

    public:
        Fields(std::vector<myPoint>, int);
        void order();
        void fillFields();
        
};

#endif