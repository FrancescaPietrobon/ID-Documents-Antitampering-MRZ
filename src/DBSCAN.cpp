#include "../include/DBSCAN.h"


myPoint::myPoint(float c_x, float c_y, float w, float h, float lab):
    x_center(c_x), y_center(c_y), box_width(w), box_height(h), label(lab) {}


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





myDBSCAN::myDBSCAN(int n_, double eps_, int minPts_, std::vector<myPoint> points_):
    n(n_), eps(eps_), minPts(minPts_), points(points_)
    {
        adjPoints.resize(size);
        clusterIdx = -1;
    }


void myDBSCAN::checkNearPoints()
{
    for(int i = 0; i < size; i++)
        for(int j = 0; j < size; j++)
        {
            if(i == j)
                continue;
            if(points[i].distance(points[j]) <= eps)
            {
                points[i].setCountPts(points[i].getCountPts() + 1);
                adjPoints[i].push_back(j);
            }
        }
}


bool myDBSCAN::isCoreObject(int idx) {
    return points[idx].getCountPts() >= minPts;
}


void myDBSCAN::dfs(int now, int c) {
    points[now].setCluster(c);
    if(!isCoreObject(now)) return;
    
    for(auto&next:adjPoints[now]) {
        if(points[next].getCluster() != NOT_CLASSIFIED) continue;
        dfs(next, c);
    }
}


void myDBSCAN::run()
{
    checkNearPoints();
    for(int i=0; i < size; i++)
    {
        if(points[i].getCluster() != NOT_CLASSIFIED)
            continue;
        if(isCoreObject(i))
            dfs(i, ++clusterIdx);
        else
            points[i].setCluster(NOISE);
    }
    
    cluster.resize(clusterIdx+1);
    for(int i = 0; i < size; i++)
        if(points[i].getCluster() != NOISE)
            cluster[points[i].getCluster()].push_back(i);
}