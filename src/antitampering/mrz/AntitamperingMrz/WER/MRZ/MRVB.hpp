#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "../Mrz.hpp"

class MRVB: public Mrz
{
    private:
        bool checkDigits(std::vector<Field> mrz, std::vector<MrzField> mrzFields);
        std::vector<MrzField> extractDocType(std::vector<Field> mrz, std::vector<MrzField> &mrzFields);
        std::vector<MrzField> extractState(std::vector<Field> mrz, std::vector<MrzField> &mrzFields);
        std::vector<MrzField> extractSurnameAndName(std::vector<Field> mrz, std::vector<MrzField> &mrzFields);
        std::vector<MrzField> extractDocNumber(std::vector<Field> mrz, std::vector<MrzField> &mrzFields);
        std::vector<MrzField> extractNationality(std::vector<Field> mrz, std::vector<MrzField> &mrzFields);
        std::vector<MrzField> extractDateBirth(std::vector<Field> mrz, std::vector<MrzField> &mrzFields);
        std::vector<MrzField> extractSex(std::vector<Field> mrz, std::vector<MrzField> &mrzFields);
        std::vector<MrzField> extractDateExpireDoc(std::vector<Field> mrz, std::vector<MrzField> &mrzFields);
        
    public:
        std::vector<MrzField> extractMrzFields(std::vector<Field> mrz);
};