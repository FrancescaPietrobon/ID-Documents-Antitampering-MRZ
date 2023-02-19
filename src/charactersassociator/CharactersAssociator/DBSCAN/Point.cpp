#include "Point.hpp"

Point::Point(Characters c, float x, float y, size_t cl):
    character(c), xCenter(x), yCenter(y), cluster(cl){}

float Point::getX()
{
    return xCenter;
}

float Point::getY()
{
    return yCenter;
}

size_t Point::getCluster()
{
    return cluster;
}

void Point::setCluster(size_t cl)
{
    cluster = cl;
}

void Point::setCountPts(size_t count)
{
    countPts = count;
}

size_t Point::getCountPts()
{
    return countPts;
}

float Point::getConfidence()
{
    return character.confidence;
}

char Point::getLabel()
{
    return character.label;
}

Coordinates Point::getPosition()
{
    return character.position;
}

float Point::distance(Point point)
{
    return std::sqrt(0.05 * (xCenter - point.getX()) * (xCenter - point.getX()) +
                     (yCenter - point.getY()) * (yCenter - point.getY()));
    //return std::sqrt((xCenter - point.getX()) * (xCenter - point.getX()) +
    //                 (yCenter - point.getY()) * (yCenter - point.getY()));
}
