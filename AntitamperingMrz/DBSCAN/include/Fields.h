#ifndef FIELDS_H
#define FIELDS_H

#include <cmath>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include "Character.h"
#include "MRZ/TD1.h"
#include "MRZ/TD2.h"
#include "MRZ/TD3.h"
#include "MRZ/MRVA.h"
#include "MRZ/MRVB.h"
#include "Field.h"
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
        float confThreshold;
        std::vector<Field> fields;
        int numLineOfMRZ = 0;
        std::map<float, Field> splittedMRZ;
        std::string MRZType = "";
        MRZ mrzGeneral;
        std::vector<association> finAss;
        std::vector<association> doubtfulAss;
        size_t numDoubtfulFields = 0;
        bool result = true;
        float confFinal = 1;
        void fillFields();
        std::string findMRZType(std::vector<std::vector<Character>>);
        void checkDate(Field & field);
        bool findField(std::string dataField, std::string& mrzDataField, std::string& fieldType, float confidence);
        void addAssociation(std::string dataField, std::string& mrzDataField, std::string& fieldType, float confidence);
        void computeConfFinal();

    public:
        Fields(std::vector<Character>, int, float);
        bool findMRZ();
        void compareMRZFields(metricsType);
        void printOrderedFields();
        void printAssociations();
        void printDoubtfulFields();
        size_t getNumDoubtfulFields();
        bool getResult();
        float getConfFinal();
        std::vector<association> getDoubtfulFields();
};

#endif