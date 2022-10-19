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
    private:
        std::multimap<float, Character> field; //it could be deleted after computeOrderedCharacters
        std::vector<Character> orderedCharacters;
        float meanY = 0;
        std::string data = "";
        bool partOfMRZ = false;

    public:
        void computeMeanY();
        void computeOrderedCharacters();
        void clear();
        std::vector<Character> getCharacters();
        float getMeanY();
        std::string getData(); 
        bool getIsPartOfMRZ();
        void setField(float x, Character point);
        void setIsPartOfMRZ(bool);
        void setData(std::string);
};

#endif