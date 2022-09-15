#ifndef FIELDS_H
#define FIELDS_H

#include <cmath>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <iostream>
#include "Points.h"


class Field
{
    private:
        std::vector<myPoint> originalCluster;
        std::multimap<float, myPoint> field;
        int clusterIdx;
        std::string typeOfData;
        bool partOfMRZ;

    public:
        Field(std::vector<myPoint>);
        void setField(float x, myPoint point);
        std::multimap<float, myPoint> getField();
        void print();
        void clear();
        void isMRZ();
        bool getPartOfMRZ();
};


class Fields
{
    private:
        std::vector<myPoint> originalCluster;
        int numClusters;
        std::map<int, Field> fields;
        std::vector<int> clusterIdxs;

    public:
        Fields(std::vector<myPoint>, int);
        void fillFields();
        
};

#endif