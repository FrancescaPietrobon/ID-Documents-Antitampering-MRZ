#include "../include/Character.h"

Character::Character(float c_x, float c_y,float lab, int cl):
    xCenter(c_x), yCenter(c_y), labelIdx(lab), cluster(cl)
    {
        extern std::unordered_map<unsigned, std::string> dictionary;
        label = dictionary[int(labelIdx)];
    }


float Character::getX()
{
    return xCenter;
}

float Character::getY()
{
    return yCenter;
}

float Character::getLabelIdx()
{
    return labelIdx;
}

std::string Character::getLabel()
{
    return label;
}

int Character::getCountPts()
{
    return countPts;
}

int Character::getCluster()
{
    return cluster;
}

void Character::setCountPts(int count)
{
    countPts = count;
}

void Character::setCluster(int cl)
{
    cluster = cl;
}


float Character::distance(Character point)
{
    return std::sqrt(0.05 * (xCenter - point.getX()) * (xCenter - point.getX()) +
                     (yCenter - point.getY()) * (yCenter - point.getY()));
    //return std::sqrt((xCenter - point.getX()) * (xCenter - point.getX()) +
    //                 (yCenter - point.getY()) * (yCenter - point.getY()));
}
