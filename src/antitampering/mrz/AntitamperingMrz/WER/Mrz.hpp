#pragma once

#include "charactersClustering/CharactersClusteringApi.hpp"
#include <unordered_map>

enum MrzType
{
    td1 = 1,
    td2,
    td3,
    mrva,
    mrvb,
    NONE,
};
struct MrzFields
{
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
};

class Mrz
{
    public:
        Mrz() = default;
        MrzFields extractMrz(const Fields *fields, const size_t fieldsSize);
        virtual void printMrzFields(MrzFields mrzFields) = 0;

    protected:
        std::unordered_map<char, unsigned> digit_conversion;
        static Mrz* createMrz(MrzType type, std::vector<Fields>);
        virtual MrzFields extractMrzFields(std::vector<Fields> mrz) = 0;
        std::vector<Fields> findMrzLines(const Fields *fields, const size_t fieldsSize);
        MrzType findMrzType(std::vector<Fields> mrzLines);
        bool check(std::string field, std::string checkDigit);
        bool checkOverall(std::vector<Fields> mrz, std::string overallDigit); 
     
};