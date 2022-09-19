#ifndef FIELDS_H
#define FIELDS_H

#include <cmath>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <iostream>
#include "Character.h"
#include "MRZ.h"
#include "Field.h"


class Fields
{
    private:
        std::vector<Character> originalCluster;
        int numClusters;
        std::map<int, Field> fields;
        std::vector<int> clusterIdxs;
        int numLineOfMRZ = 0;
        std::map<float, Field> splittedMRZ;

    public:
        Fields(std::vector<Character>, int);
        void fillFields();
        void printOrderedFields();
        void fillLabels();
        int getNumLineOfMRZ();
        void checkMRZ();
        std::string findMRZType(std::vector<std::vector<Character>>);
        
};

#endif