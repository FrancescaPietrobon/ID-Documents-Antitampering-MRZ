#ifndef MRZ_H
#define MRZ_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include "Character.h"


class MRZ
{
    protected:
        std::vector<std::vector<Character>> mrz;
        int numLines;
        std::string MRZType;
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
        std::string checkOptionalData;

    public:
        MRZ(std::vector<std::vector<Character>>, int);
        void printMRZ();
        void findMRZType();
        void extractFieldsTD2();
        void extractFieldsMRVA();
        void extractFieldsMRVB();
        void computeFullMRZ();
        std::string getMRZType();
        std::string getDocType();
        std::string getState();
        std::string getSurname();
        std::string getName();
        std::string getDocNumber();
        std::string getCheckDocNum();
        std::string getNationality();
        std::string getDateBirth();
        std::string getCheckDateBirth();
        std::string getSex();
        std::string getDateExpireDoc();
        std::string getCheckDateExpireDoc();
        std::string getOptionalData();
        std::string getCheckOptionalData();
        
};


class TD1: public MRZ
{
    private:
        std::string secondOptionalData;
        std::string checkFirstTwoLines;
    public:
        TD1(std::vector<std::vector<Character>>, int);
        void extractFields();
};

class TD3: public MRZ
{
    private:
        std::string checkOverall;
    public:
        TD3(std::vector<std::vector<Character>>, int);
        void extractFields();
        void printMRZFields();
};

#endif