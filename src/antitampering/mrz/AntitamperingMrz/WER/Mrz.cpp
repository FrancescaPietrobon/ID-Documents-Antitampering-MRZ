#include "Mrz.hpp"

static std::unordered_map<char, unsigned> digitsConversion = {
    {'<', 0,}, {'0', 0,}, {'1', 1,}, {'2', 2,}, {'3', 3,}, {'4', 4,}, {'5', 5,}, {'6', 6,}, {'7', 7,}, {'8', 8,}, {'9', 9,},
    {'A', 10,}, {'B', 11,}, {'C', 12,}, {'D', 13,}, {'E', 14,}, {'F', 15,}, {'G', 16,}, {'H', 17,}, {'I', 18,}, {'J', 19,},
    {'K', 20,}, {'L', 21,}, {'M', 22,}, {'N', 23,}, {'O', 24,}, {'P', 25,}, {'Q', 26,}, {'R', 27,}, {'S', 28,}, {'T', 29,},
    {'U', 30,}, {'V', 31,}, {'W', 32,}, {'X', 33,}, {'Y', 34,}, {'Z', 35,}
};

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
        sum += digitsConversion[field[i]] * weight[i];

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