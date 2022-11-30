#include "MRZ.h"
#include "TD1.h"
#include "TD2.h"
#include "TD3.h"
#include "MRVA.h"
#include "MRVB.h"


MRZ* MRZ::createMRZ(MRZType type, std::vector<std::vector<Character>> chars) { 
    if (type == td1) 
        return new TD1(chars); 
    else if (type == td2) 
        return new TD2(chars); 
    else if (type == td3) 
        return new TD3(chars);
    else if (type == mrva) 
        return new MRVA(chars); 
    else if (type == mrvb) 
        return new MRVB(chars);
    else return NULL; 
} 


MRZ::MRZ(std::vector<std::vector<Character>> characters):
    mrz(characters){}

std::vector<detection> MRZ::getAllFields()
{
    return allFields;
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


void MRZ::printMRZFields()
{
    std::cout << "\nMRZ fields detected:" << std::endl;
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
    int sum = 0;
    size_t i = 0;
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


bool MRZ::checkOverall(std::string overallDigit)
{
    std::string stringForCheckOverall;
    for(int i = 0; i < 10; ++i)
        stringForCheckOverall += mrz[0][i].getLabel();
    
    for(int i = 13; i < 20; ++i)
        stringForCheckOverall += mrz[0][i].getLabel();

    for(int i = 21; i < 43; ++i)
        stringForCheckOverall += mrz[1][i].getLabel();
    
    return check(stringForCheckOverall, overallDigit);
}