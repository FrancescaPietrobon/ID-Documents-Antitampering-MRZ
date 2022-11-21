#include "../include/DBSCAN.h"


DBSCAN::DBSCAN(float eps_, int minPts_, std::pair<matrix2D, std::vector<float>> pred):
    eps(eps_), minPts(minPts_)
    {
        clusterIdx = -1;
        computePoints(pred);
        size = points.size();
        adjPoints.resize(size);
    }

std::vector<Character> DBSCAN::getPoints()
{
    return points;
}

int DBSCAN::getClusterIdx()
{
    return clusterIdx;
}


void DBSCAN::computePoints(std::pair<matrix2D, std::vector<float>> boxesLabels)
{
    matrix2D boxes = boxesLabels.first;
    std::vector<float> labels = boxesLabels.second;

    float h, w, xCenter, yCenter;
    for(size_t i = 0; i < boxes.size(); ++i)
    {
        w = boxes[i][2] - boxes[i][0];
        h = boxes[i][3] - boxes[i][1];

        xCenter = boxes[i][0] + w / 2;
        yCenter = boxes[i][1] + h / 2;
 
        Character point(xCenter, yCenter, labels[i], NOT_CLASSIFIED);
        points.push_back(point);
    }
}


void DBSCAN::run()
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


void DBSCAN::checkNearPoints()
{
    for(int i = 0; i < size; i++)
        for(int j = 0; j < size; j++)
        {
            if(i == j)
                continue;
            //std::cout << points[i].distance(points[j]) << points[i].getLabelIdx() << std::endl;
            if(points[i].distance(points[j]) <= eps)
            {
                points[i].setCountPts(points[i].getCountPts() + 1);
                adjPoints[i].push_back(j);
            }
        }
}


bool DBSCAN::isCoreObject(int idx) {
    return points[idx].getCountPts() >= minPts;
}


void DBSCAN::dfs(int now, int c) {
    points[now].setCluster(c);
    if(!isCoreObject(now)) return;
    
    for(auto & next: adjPoints[now]) {
        if(points[next].getCluster() != NOT_CLASSIFIED) continue;
        dfs(next, c);
    }
}