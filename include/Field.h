#ifndef FIELD_H
#define FIELD_H

#include <cmath>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <iostream>
#include "Character.h"

class Field
{
    protected:
        std::vector<Character> originalCluster;
        std::vector<Character> orderedField;
        std::multimap<float, Character> field;
        int clusterIdx;
        std::string typeOfData;
        bool partOfMRZ;
        float meanY;

    public:
        Field(std::vector<Character>);
        void setField(float x, Character point);
        std::multimap<float, Character> getField();
        std::vector<Character> getCharacters();
        std::vector<Character> getOriginalCluster();
        void print();
        void clear();
        void isPartOfMRZ();
        bool getIsPartOfMRZ();
        void setTypeOfData(std::string);
        void computeField();
        void computeMeanY();
        float getMeanY();
};

#endif