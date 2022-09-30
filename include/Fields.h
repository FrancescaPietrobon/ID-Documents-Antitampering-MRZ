#ifndef FIELDS_H
#define FIELDS_H

#include <cmath>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include "Character.h"
#include "MRZ.h"
#include "Field.h"
#include "utilities.h"
#include "Date.h"


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
        void mostCompatible(Field & field);
        void checkAlphanumDate(Field & field);
        std::string checkMonth(std::string);

    public:
        Fields(std::vector<Character>, int);
        void fillFields();
        void printOrderedFields();
        void fillLabels();
        int getNumLineOfMRZ();
        void checkMRZ();
        std::string findMRZType(std::vector<std::vector<Character>>);
        void compareMRZFields();
        void printMRZAllFieldsInv();
        void printNotFilledAndFilledFields();
        void printAssociations();
        float computeFinalConf();
        void printDoubtfulFields();
        
};

#endif