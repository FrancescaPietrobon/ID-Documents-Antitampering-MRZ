#include "../include/DBSCAN.h"


myDBSCAN::myDBSCAN(double eps_, int minPts_, std::vector<myPoint> points_):
    eps(eps_), minPts(minPts_), points(points_)
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
            //std::cout << points[i].distance(points[j]) << points[i].getLabel() << std::endl;
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
    for(int i = 0; i < size; i++)
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


std::vector<myPoint> myDBSCAN::getPoints()
{
    return points;
}

int myDBSCAN::getClusterIdx()
{
    return clusterIdx;
}