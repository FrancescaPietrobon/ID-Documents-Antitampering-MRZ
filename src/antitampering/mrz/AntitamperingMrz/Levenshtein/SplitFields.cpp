#include "SplitFields.hpp"

#include "MRZ/TD1.hpp"
#include "MRZ/TD2.hpp"
#include "MRZ/TD3.hpp"
#include "MRZ/MRVA.hpp"
#include "MRZ/MRVB.hpp"

std::vector<Field> SplitFields::findMrzLines(const Field *fields, const size_t fieldsSize)
{
    std::map<float, Field> orderedMrz;
    size_t countSymbolLower;
    for(size_t i = 0; i < fieldsSize; ++i)
    {
        countSymbolLower = 0;
        for(size_t j = 0; j < fields[i].labelSize; ++j)
        {
            if(fields[i].label[j] == '<')
                countSymbolLower += 1;
        }
        if(countSymbolLower > 3)
            orderedMrz.emplace(fields[i].position.topLeftY, fields[i]);     
    }

    std::vector<Field> mrzLines;
    for(auto & line: orderedMrz)
        mrzLines.push_back(line.second);

    return mrzLines;
}

std::vector<MrzField> SplitFields::extractMrzFields(std::vector<Field> mrzLines)
{
    MrzType mrzType = this->findMrzType(mrzLines);
    std::vector<MrzField> mrzFields;
    if(mrzType != NONE)
    {
        Mrz* mrz = this->createMrz(mrzType, mrzLines);
        mrzFields = mrz->extractMrzFields(mrzLines);
        checkDigitsResult = mrz->checkDigits(mrzLines, mrzFields);
        mrz->printMrzFields(mrzFields);
    }
    else
    {
        SPDLOG_ERROR("FATAL: Mrz Type NOT FOUND");
        throw Exception(ErrorCode::GENERAL_ERROR, std::string("Mrz Type not handled"));
    }
    return mrzFields;
}

MrzType SplitFields::findMrzType(std::vector<Field> mrzLines)
{
    MrzType mrzType = NONE;

    if(this->isTD1(mrzLines))
        mrzType = td1;
    else if(this->isTD2(mrzLines))
        mrzType = td2;
    else if(this->isTD3(mrzLines))
        mrzType = td3;
    else if(this->isMRVA(mrzLines))
        mrzType = mrva;
    else if(this->isMRVB(mrzLines))
        mrzType = mrvb;

    return mrzType;
}

bool SplitFields::isTD1(std::vector<Field> mrzLines)
{
    return ((mrzLines.size() == 3) &&
            (mrzLines[0].labelSize == 36) && (mrzLines[1].labelSize == 36) && (mrzLines[2].labelSize == 36));
}

bool SplitFields::isTD2(std::vector<Field> mrzLines)
{
    return ((mrzLines.size() == 2) &&
            (mrzLines[0].labelSize == 36) && (mrzLines[1].labelSize == 36) &&
            (mrzLines[0].label[0] == 'P'));
}

bool SplitFields::isTD3(std::vector<Field> mrzLines)
{
    return ((mrzLines.size() == 2) &&
            (mrzLines[0].labelSize == 44) && (mrzLines[1].labelSize == 44) &&
            (mrzLines[0].label[0] == 'P'));
}

bool SplitFields::isMRVB(std::vector<Field> mrzLines)
{
    return ((mrzLines.size() == 2) &&
            (mrzLines[0].labelSize == 36) && (mrzLines[1].labelSize == 36) &&
            (mrzLines[0].label[0] != 'P'));
}

bool SplitFields::isMRVA(std::vector<Field> mrzLines)
{
    return ((mrzLines.size() == 2) &&
            (mrzLines[0].labelSize == 44) && (mrzLines[1].labelSize == 44) &&
            (mrzLines[0].label[0] != 'P'));
}

Mrz* SplitFields::createMrz(MrzType mrzType, std::vector<Field> chars)
{ 
    if (mrzType == td1) 
        return new TD1; 
    else if (mrzType == td2) 
        return new TD2; 
    else if (mrzType == td3) 
        return new TD3;
    else if (mrzType == mrva) 
        return new MRVA; 
    else if (mrzType == mrvb) 
        return new MRVB;
    else return NULL; 
}

std::vector<Field> SplitFields::extractFieldsWithoutMrz(const Field *allFields, const size_t fieldsSize, std::vector<Field> mrzLines)
{
    std::vector<Field> fields;
    for(size_t i = 0; i < fieldsSize; ++i)
        fields.push_back(allFields[i]);

    for(Field line: mrzLines)
        fields.erase(std::remove(fields.begin(), fields.end(), line), fields.end());
    
    return fields;
}

bool SplitFields::getCheckDigitsResult()
{
    return checkDigitsResult;
}
