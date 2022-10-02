#ifndef MRZ_H
#define MRZ_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include "Character.h"


class MRZ
{
    protected:
        std::vector<std::vector<Character>> mrz;
        std::multimap<std::string, std::string> allFields;
        std::multimap<std::string, std::string> allFieldsInv;
        int numLines;
        std::string MRZType;
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
        void assignMRZ(std::vector<std::vector<Character>>, int);
        void printMRZ();
        void printMRZFields();
        void computeFullMRZ();
        std::multimap<std::string, std::string> getAllFieldsInv();
        std::multimap<std::string, std::string> getAllFields();
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
        //std::unordered_map<std::string, std::string> allFieldsInv;
        
        
};


class TD1: public MRZ
{
    private:
        std::string secondOptionalData;
        std::string checkFirstTwoLines;
        std::string checkOptionalData;
    public:
        TD1(std::vector<std::vector<Character>>, int);
        void extractFields();
};

class TD2: public MRZ
{
    private:

    public:
        TD2(std::vector<std::vector<Character>>, int);
        void extractFields();
        std::string checkOptionalData;
        std::string checkOverall;

};

class TD3: public MRZ
{
    private:
        std::string getCheckOptionalData();
        std::string checkOverall;
        std::string checkOptionalData;

    public:
        TD3(std::vector<std::vector<Character>>, int);
        void extractFields();
        void printMRZFields();
};

class MRVA: public MRZ
{
    private:

    public:
        MRVA(std::vector<std::vector<Character>>, int);
        void extractFields();
        void printMRZFields();
};

class MRVB: public MRZ
{
    private:

    public:
        MRVB(std::vector<std::vector<Character>>, int);
        void extractFields();
};

#endif