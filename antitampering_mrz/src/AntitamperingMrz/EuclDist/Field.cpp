#include "Field.h"


void Field::setField(float x, Character point)
{
    field.emplace(x, point);
}

std::vector<Character> Field::getCharacters()
{
    return orderedCharacters;
}

void Field::setIsPartOfMRZ(bool isPart)
{
    partOfMRZ = isPart;
}

bool Field::getIsPartOfMRZ()
{
    return partOfMRZ;
}

void Field::setData(std::string d)
{
    data = d;
}

std::string Field::getData()
{
    return data;
}

float Field::getMeanY()
{
    return meanY;
}


void Field::clear()
{
    field.clear();
    orderedCharacters.clear();
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


void Field::computeOrderedCharacters()
{
    for(auto & character: field)
        orderedCharacters.push_back(character.second);
}