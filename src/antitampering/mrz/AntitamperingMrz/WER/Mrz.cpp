#include "Mrz.hpp"

#include "MRZ/TD1.hpp"
#include "MRZ/TD2.hpp"
#include "MRZ/TD3.hpp"
#include "MRZ/MRVA.hpp"
#include "MRZ/MRVB.hpp"

std::vector<Fields> Mrz::findMrzLines(const Fields *fields, const size_t fieldsSize)
{
    std::map<float, Fields> orderedMrz;
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

    std::vector<Fields> mrzLines;
    for(auto & line: orderedMrz)
        mrzLines.push_back(line.second);

    return mrzLines;
}

std::vector<MrzFields> Mrz::extractMrz(std::vector<Fields> mrzLines)
{
    MrzType mrzType = findMrzType(mrzLines);
    Mrz* mrz;
    std::vector<MrzFields> mrzFields;
    if(mrzType != NONE)
    {
        mrz = createMrz(mrzType, mrzLines);
        mrzFields = mrz->extractMrzFields(mrzLines);
        mrz->printMrzFields(mrzFields);
    }
    else
    {
        SPDLOG_ERROR("FATAL: Mrz Type NOT FOUND");
        throw Exception(ErrorCode::GENERAL_ERROR, std::string("Mrz Type not handled"));
    }
    return mrzFields;
}

MrzType Mrz::findMrzType(std::vector<Fields> mrzLines)
{
    MrzType mrzType = NONE;
    if((mrzLines.size() == 3) && (mrzLines[0].labelSize == 36) && (mrzLines[1].labelSize == 36) && (mrzLines[2].labelSize == 36))
        mrzType = td1;
    else if((mrzLines[0].labelSize == 36) && (mrzLines[1].labelSize == 36))
    {
        if(mrzLines[0].label[0] == 'P')
            mrzType = td2;
        else
            mrzType = mrvb;
    }
    else if((mrzLines[0].labelSize == 44) && (mrzLines[1].labelSize == 44))
    {
        if(mrzLines[0].label[0] == 'P')
            mrzType = td3;
        else
            mrzType = mrva;
    }
    else
        mrzType = NONE;

    return mrzType;
}

Mrz* Mrz::createMrz(MrzType mrzType, std::vector<Fields> chars)
{ 
    /*
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
    */
    return new TD3;

} 

bool Mrz::check(std::string field, std::string checkDigit)
{
    int sum = 0;
    size_t i = 0;

    std::vector<int> weight;
    std::vector<int> vals {7, 3, 1};

    while(i < field.size())
    {
        for(int j: vals)
        {
            weight.push_back(j);
            ++i;
        }
    }
    
    for(size_t i = 0; i < field.size(); ++i)
        sum += digit_conversion[field[i]] * weight[i];

    return (checkDigit == std::to_string(sum % 10));
}

bool Mrz::checkOverall(std::vector<Fields> mrz, std::string overallDigit)
{
    std::string stringForCheckOverall;
    for(int i = 0; i < 10; ++i)
        stringForCheckOverall += mrz[0].label[i];
    
    for(int i = 13; i < 20; ++i)
        stringForCheckOverall += mrz[0].label[i];

    for(int i = 21; i < 43; ++i)
        stringForCheckOverall += mrz[1].label[i];
    
    return check(stringForCheckOverall, overallDigit);
}

void Mrz::printMrzFields(std::vector<MrzFields> mrzFields)
{
    std::cout << "\nMRZ fields detected:" << std::endl;
    for(MrzFields field: mrzFields)
        std::cout << field.fieldType << ": " << field.mrzDataField << std::endl;
}