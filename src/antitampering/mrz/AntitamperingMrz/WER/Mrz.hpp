#pragma once

#include "charactersClustering/CharactersClusteringApi.hpp"

enum MrzType
{
    td1 = 1,
    td2,
    td3,
    mrva,
    mrvb,
    NONE,
};

class Mrz
{
    public:
        Mrz(std::vector<Fields> mrz);
        static Mrz* createMrz(MrzType type, std::vector<Fields>);
        Mrz() = default;
        virtual void extractFields(std::vector<Fields> mrz) = 0;

    protected:
        std::vector<Fields> mrz;

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
        bool checkOverall(std::string overallDigit); 
     
};