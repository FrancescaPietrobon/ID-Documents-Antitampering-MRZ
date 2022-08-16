#include "DBSCAN.h"

double Point::distance(const Point & point)
{
    return std::sqrt((x - point.x) * (x - point.x) +
                     (y - point.y) * (y - point.y));
}

int Point::getCountPts()
{
    return countPts;
}


void Point::setCountPts(int count)
{
    countPts = count;
}


int Point::getCluster()
{
    return cluster;
}


void Point::setCluster(int cl)
{
    cluster = cl;
}


DBSCAN::DBSCAN(int n_, double eps_, int minPts_, std::vector<Point> points_):
    n(n_), eps(eps_), minPts(minPts_), points(points_)
    {
        adjPoints.resize(size);
        clusterIdx = -1;
    }

    
void DBSCAN::run()
{
    checkNearPoints();
    for(size_t i=0; i < size; i++)
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


void DBSCAN::checkNearPoints()
{
    for(size_t i = 0; i < size; i++)
        for(size_t j = 0; j < size; j++)
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


void DBSCAN::dfs (int now, int c) {
    points[now].setCluster(c);
    if(!isCoreObject(now)) return;
    
    for(auto&next:adjPoints[now]) {
        if(points[next].getCluster() != NOT_CLASSIFIED) continue;
        dfs(next, c);
    }
}


bool DBSCAN::isCoreObject(int idx) {
    return points[idx].getCountPts() >= minPts;
}


std::vector<std::vector<int> > DBSCAN::getCluster() {
    return cluster;
}