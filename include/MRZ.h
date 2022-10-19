#ifndef MRZ_H
#define MRZ_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <list>
#include "Character.h"


struct detection
{
    std::string typeFieldMRZ;
    std::string fieldMRZ;
};

class MRZ
{
    protected:
        std::vector<std::vector<Character>> mrz;
        int numLines;
        std::vector<detection> allFields;

        std::string docType;
        std::string state;
        std::string surname;
        std::string name;
        std::string docNumber;
        std::string checkDocNum;
        std::string nationality;
        std::string dateBirth;
        std::string checkDateBirth;
        std::string sex;
        std::string dateExpireDoc;
        std::string checkDateExpireDoc;
        std::string optionalData;
        bool check(std::string field, std::string checkDigit);
        bool CheckOverAll(std::string checkOverall); 

    public:
        MRZ(std::vector<std::vector<Character>>, int);
        MRZ() = default;
        void printMRZ();
        void printMRZFields();
        void removeAllFields(std::vector<detection>::iterator);

        void assignMRZ(std::vector<std::vector<Character>>, int);
        void computeFullMRZ();
        std::vector<detection> getAllFields();
};


#endif