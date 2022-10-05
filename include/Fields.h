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


class Fields
{
    private:
        std::vector<Character> originalCluster;
        int numClusters;
        std::list<Field> fields;
        std::map<std::string, std::pair<std::pair<std::string,std::string>, float>> finalAssociation;
        int numLineOfMRZ = 0;
        std::map<float, Field> splittedMRZ;
        MRZ mrzGeneral;
        size_t numDoubtfulFields = 0;
        bool result = true;
        float finalConf = 1;
        void mostCompatible(Field & field, metricsType metricType);
        void checkAlphanumDate(Field & field);
        void computeFinalConf();
        std::string findMRZType(std::vector<std::vector<Character>>);

    public:
        Fields(std::vector<Character>, int);
        void fillFields();
        void printOrderedFields();
        size_t getNumDoubtfulFields();
        bool getResult();
        float getFinalConf();
        void checkMRZ();
        void compareMRZFields(metricsType);
        void printNotFilledAndFilledFields();
        void printAssociations();
        void printDoubtfulFields();
        std::map<std::string, std::pair<std::pair<std::string,std::string>, float>> getFinalAssociations();
};

#endif