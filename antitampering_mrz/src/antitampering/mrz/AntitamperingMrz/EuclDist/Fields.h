#ifndef _FIELDS_H_
#define _FIELDS_H_

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
#include "TD1.h"
#include "TD2.h"
#include "TD3.h"
#include "MRVA.h"
#include "MRVB.h"
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
        std::string MRZtype = "";
        std::vector<association> finAss;
        std::vector<association> doubtfulAss;
        size_t numDoubtfulFields = 0;
        bool result = true;
        float confFinal = 1;
        void fillFields();
        MRZType findMRZType(std::vector<std::vector<Character>>);
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