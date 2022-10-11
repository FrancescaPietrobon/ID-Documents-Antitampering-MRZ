#include "../include/DBSCAN.h"


myDBSCAN::myDBSCAN(float eps_, int minPts_, std::pair<matrix2D, std::vector<float>> pred):
    eps(eps_), minPts(minPts_)
    {
        clusterIdx = -1;
        computePoints(pred);
        size = points.size();
        adjPoints.resize(size);
    }


void myDBSCAN::computePoints(std::pair<matrix2D, std::vector<float>> boxes_labels)
{
    matrix2D boxes = boxes_labels.first;
    std::vector<float> labels = boxes_labels.second;

    float h, w, c_x, c_y;
    for(size_t i = 0; i < boxes.size(); ++i)
    {
        w = boxes[i][2] - boxes[i][0];
        h = boxes[i][3] - boxes[i][1];

        c_x = boxes[i][0] + w / 2;
        c_y = boxes[i][1] + h / 2;
 
        Character point(c_x, c_y, w, h, labels[i], NOT_CLASSIFIED);
        points.push_back(point);
    }
}


void myDBSCAN::checkNearPoints()
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


bool myDBSCAN::isCoreObject(int idx) {
    return points[idx].getCountPts() >= minPts;
}


void myDBSCAN::dfs(int now, int c) {
    points[now].setCluster(c);
    if(!isCoreObject(now)) return;
    
    for(auto & next: adjPoints[now]) {
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


std::vector<Character> myDBSCAN::getPoints()
{
    return points;
}

int myDBSCAN::getClusterIdx()
{
    return clusterIdx;
}