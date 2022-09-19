#include "../include/Character.h"

Character::Character(float c_x, float c_y, float w, float h, float lab, int cl):
    x_center(c_x), y_center(c_y), box_width(w), box_height(h), labelIdx(lab), cluster(cl)
    {
        extern std::unordered_map<unsigned, std::string> dictionary;
        label = dictionary[int(labelIdx)];
    }


float Character::getX()
{
    return x_center;
}

float Character::getY()
{
    return y_center;
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
    return std::sqrt((x_center - point.getX()) * (x_center - point.getX()) +
                     (y_center - point.getY()) * (y_center - point.getY()));
}
