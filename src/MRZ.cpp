#include "../include/MRZ.h"

MRZ::MRZ(std::vector<std::vector<Character>> characters, int nl):
    mrz(characters), numLines(nl){}

std::multimap<std::string, std::string> MRZ::getAllFieldsInv()
{
    return allFieldsInv;
}

std::multimap<std::string, std::string> MRZ::getAllFields()
{
    return allFields;
}

std::vector<detection> MRZ::getAllFieldsStruct()
{
    return allFieldsStruct;
}

void MRZ::removeFieldInv(std::multimap<std::string, std::string>::iterator it)
{
    allFieldsInv.erase(it->first);
    allFields.erase(it->second);
}

void MRZ::removeField(std::multimap<std::string, std::string>::iterator it)
{
    allFieldsInv.erase(it->second);
    allFields.erase(it->first);
}

void MRZ::removeAllFieldsStruct(std::vector<detection>::iterator i)
{
    std::cout << "erase " << (*i).fieldMRZ << " " << (*i).typeFieldMRZ << std::endl;
    allFieldsStruct.erase(i);
    std::cout << allFieldsStruct.size() << std::endl;
}


void MRZ::printMRZ()
{
    for(size_t i = 0; i < mrz.size(); ++i)
    {
        for(size_t j = 0; j < mrz[0].size(); ++j)
        {
            std::cout << mrz[i][j].getLabel();
        }
        std::cout << std::endl;
    }
}


void MRZ::assignMRZ(std::vector<std::vector<Character>> mrzChar, int numLineOfMRZ)
{
    mrz = mrzChar;
    numLines = numLineOfMRZ;
}


void MRZ::printMRZFields()
{
    std::cout << "MRZ fields detected: " << std::endl;
    std::cout << "Document type: " << docType << std::endl;
    std::cout << "State: " << state << std::endl;
    std::cout << "Surname: " << surname << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Document number: " << docNumber << std::endl;
    std::cout << "Nationality: " << nationality << std::endl;
    std::cout << "Date of birth: " << dateBirth << std::endl;
    std::cout << "Sex: " << sex << std::endl;
    std::cout << "Date of expire: " << dateExpireDoc << std::endl;
}


bool MRZ::check(std::string field, std::string checkDigit)
{
    int sum = 0, i = 0;
    extern std::unordered_map<char, unsigned> digit_conversion;

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


bool MRZ::CheckOverAll(std::string checkOverall)
{
    std::string stringForCheckOverall;
    for(int i = 0; i < 10; ++i)
        stringForCheckOverall += mrz[0][i].getLabel();
    
    for(int i = 13; i < 20; ++i)
        stringForCheckOverall += mrz[0][i].getLabel();

    for(int i = 21; i < 43; ++i)
        stringForCheckOverall += mrz[1][i].getLabel();
        
    if(!check(stringForCheckOverall, checkOverall))
        std::cout << "Check overall faild." << std::endl;
    else
        std::cout << "Check overall OK." << std::endl;
    
    return check(stringForCheckOverall, checkOverall);
}