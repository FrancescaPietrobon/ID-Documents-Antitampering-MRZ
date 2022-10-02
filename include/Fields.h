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
#include "MRZ.h"
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
        void searchField(std::string);
        MRZ mrzGeneral;
        size_t numDoubtfulFields = 0;
        bool result = true;
        float finalConf = 1;
        void mostCompatible(Field & field, metricsType metricType);
        void checkAlphanumDate(Field & field);
        //std::string checkMonth(std::string);
        std::pair<std::string, size_t> checkMonth(std::string);
        void computeFinalConf();

    public:
        Fields(std::vector<Character>, int);
        void fillFields();
        void printOrderedFields();
        void fillLabels();
        int getNumLineOfMRZ();
        size_t getNumDoubtfulFields();
        bool getResult();
        float getFinalConf();
        void checkMRZ();
        std::string findMRZType(std::vector<std::vector<Character>>);
        void compareMRZFields(metricsType);
        void printMRZAllFieldsInv();
        void printNotFilledAndFilledFields();
        void printAssociations();
        void printDoubtfulFields();
        std::map<std::string, std::pair<std::pair<std::string,std::string>, float>> getFinalAssociations();
        
};

#endif