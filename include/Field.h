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
        std::multimap<float, Character> field; //it could be deleted after computeOrderedCharacters
        std::vector<Character> orderedCharacters;
        float meanY = 0;
        std::string data = "";
        bool partOfMRZ = false;
        std::string typeOfData = "None";

        bool operator == (const Field & other) const;

    public:
        void setField(float x, Character point);
        bool getIsPartOfMRZ();
        void setIsPartOfMRZ(bool);
        void clear();
        void computeMeanY();
        void computeOrderedCharacters();
        void setData(std::string);
        void setTypeOfData(std::string);
        std::vector<Character> getCharacters();
        float getMeanY();
        std::string getTypeOfData();
        std::string getData();        
};

#endif