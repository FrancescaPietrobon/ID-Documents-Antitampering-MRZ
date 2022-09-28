#include "../include/Field.h"


Field::Field(std::vector<Character> chars):
    originalCluster(chars)
    {
        partOfMRZ = false;
        typeOfData = "";
    }


void Field::setField(float x, Character point)
{
    field.emplace(x, point);
}

std::multimap<float, Character> Field::getField()
{
    return field;
}

std::vector<Character> Field::getOriginalCluster()
{
    return originalCluster;
}

std::vector<Character> Field::getCharacters()
{
    for(auto & character: field)
    {
        orderedField.push_back(character.second);
    }
    return orderedField;
}

void Field::clear()
{
    field.clear();
    partOfMRZ = false;
}

void Field::isPartOfMRZ()
{
    partOfMRZ = true;
}

bool Field::getIsPartOfMRZ()
{
    return partOfMRZ;
}

void Field::setTypeOfData(std::string t)
{
    typeOfData = t;
}

std::string Field::getTypeOfData()
{
    return typeOfData;
}

void Field::setData(std::string d)
{
    data = d;
}

std::string Field::getData()
{
    return data;
}

void Field::computeMeanY()
{
    float sum = 0;
    for(auto & character: field)
    {
        sum += character.second.getY();
    }
    meanY = sum / field.size();
}

float Field::getMeanY()
{
    return meanY;
}

void Field::print()
{
    for(auto it = field.begin(); it != field.end(); ++it)
    {
        std::cout << ((*it).second).getLabel();
    }
}


void Field::computeField()
{
    for(auto it = field.begin(); it != field.end(); ++it)
    {
        //std::cout << ((*it).second).getLabel();
    }
}