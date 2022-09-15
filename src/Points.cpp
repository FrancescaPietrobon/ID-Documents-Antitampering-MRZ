#include "../include/Points.h"

myPoint::myPoint(float c_x, float c_y, float w, float h, float lab, int cl):
    x_center(c_x), y_center(c_y), box_width(w), box_height(h), label(lab), cluster(cl) {}


float myPoint::getX()
{
    return x_center;
}

float myPoint::getY()
{
    return y_center;
}

float myPoint::getLabel()
{
    return label;
}

int myPoint::getCountPts()
{
    return countPts;
}

int myPoint::getCluster()
{
    return cluster;
}

void myPoint::setCountPts(int count)
{
    countPts = count;
}

void myPoint::setCluster(int cl)
{
    cluster = cl;
}


float myPoint::distance(myPoint point)
{
    return std::sqrt((x_center - point.getX()) * (x_center - point.getX()) +
                     (y_center - point.getY()) * (y_center - point.getY()));
}
