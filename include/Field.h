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
        std::string data = "";
        std::string typeOfData = "";
        bool partOfMRZ = false;
        float meanY;

    public:
        Field(std::vector<Character>);
        void setField(float x, Character point);
        void setData(std::string);
        void setTypeOfData(std::string);
        std::multimap<float, Character> getField();
        std::vector<Character> getCharacters();
        std::vector<Character> getOriginalCluster();
        float getMeanY();
        std::string getData();
        void setisPartOfMRZ(bool);
        bool getIsPartOfMRZ();
        std::string getTypeOfData();
        void print();
        void clear();
        void computeMeanY();
};

#endif