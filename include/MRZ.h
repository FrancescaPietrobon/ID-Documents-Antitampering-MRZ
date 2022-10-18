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
        std::multimap<std::string, std::string> allFieldsInv;
        std::vector<detection> allFieldsStruct;

        std::multimap<std::string, std::string> allFields;
        std::string docType;
        std::string passportType;
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
        std::multimap<std::string, std::string> getAllFieldsInv();
        void removeField(std::multimap<std::string, std::string>::iterator);
        void removeFieldInv(std::multimap<std::string, std::string>::iterator);
        void removeAllFieldsStruct(std::vector<detection>::iterator);

        void assignMRZ(std::vector<std::vector<Character>>, int);
        void computeFullMRZ();
        std::multimap<std::string, std::string> getAllFields();
        std::vector<detection> getAllFieldsStruct();
};


#endif