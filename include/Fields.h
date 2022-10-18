#ifndef FIELDS_H
#define FIELDS_H

#include <cmath>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <iterator>
#include "Character.h"
#include "MRZ/TD1.h"
#include "MRZ/TD2.h"
#include "MRZ/TD3.h"
#include "MRZ/MRVA.h"
#include "MRZ/MRVB.h"
#include "Field.h"
#include "utilities.h"
#include "Date.h"
#include "Metrics.h"

struct association
{
    std::string fieldType;
    std::string dataField;
    std::string mrzDataField;
    float confidence;
};


class Fields
{
    private:
        std::vector<Character> originalCluster;
        int numClusters;
        std::vector<Field> fields;
        int numLineOfMRZ = 0;
        std::map<float, Field> splittedMRZ;
        std::string MRZType = "";
        MRZ mrzGeneral;
        std::string findMRZType(std::vector<std::vector<Character>>);
        void checkDate(Field & field);

        float confThreshold;
        
        std::map<std::string, std::pair<std::pair<std::string,std::string>, float>> finalAssociation;
        size_t numDoubtfulFields = 0;
        bool result = true;
        float confFinal = 1;
        void mostCompatible(Field & field, metricsType metricType);
        void computeConfFinal();
        std::vector<association> finAss;
        std::vector<association> doubtfulAss;

    public:
        Fields(std::vector<Character>, int, float);
        void fillFields();
        bool findMRZ();
        void printOrderedFields();
        void compareMRZFields(metricsType);
        void compare();
        void printFinAss();
        void printDoubtfulAss();

        size_t getNumDoubtfulFields();
        bool getResult();
        float getConfFinal();
        MRZ getMRZ();
        void printNotFilledAndFilledFields();
        void printAssociations();
        void printDoubtfulFields();
        std::map<std::string, std::pair<std::pair<std::string,std::string>, float>> getFinalAssociations();
};

#endif