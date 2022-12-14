#include "DBSCAN.h"

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>

DBSCAN::DBSCAN(float eps_, std::pair<matrix2D, std::vector<float>> pred):
    eps(eps_)
    {
        points = computePointsForCharacters(pred);
        size = points.size();
        adjPoints.resize(points.size());
    }

std::vector<Character> DBSCAN::getPoints()
{
    return points;
}

std::vector<std::vector<int>> DBSCAN::getAdjPoints()
{
    return adjPoints;
}

int DBSCAN::getClusterIdx()
{
    return clusterIdx;
}


std::vector<Character> DBSCAN::computePointsForCharacters(std::pair<matrix2D, std::vector<float>> boxesLabels)
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
 
        Character character(xCenter, yCenter, labels[i], NOT_CLASSIFIED);
        points.push_back(character);
    }
    return points;
}


std::vector<Field> DBSCAN::extractFields()
{
    SPDLOG_INFO("findNearPoints");
    adjPoints = findNearPoints(points);
    for(int i = 0; i < size; i++)
    {
        if(points[i].getCluster() != NOT_CLASSIFIED)
            continue;
        points = dfs(i, ++clusterIdx, points, adjPoints);
    }
    
    SPDLOG_INFO("assignCluster");
    cluster = assignCluster(points);

    SPDLOG_INFO("computeFields");
    std::vector<Field> fields = computeFields(points, adjPoints);

    return fields;
}


std::vector<std::vector<int>> DBSCAN::findNearPoints(std::vector<Character> points)
{
    for(int i = 0; i < points.size(); i++)
        for(int j = 0; j < points.size(); j++)
        {
            if(i == j)
                continue;
            if(points[i].distance(points[j]) <= eps)
                adjPoints[i].push_back(j);
        }
    return adjPoints;
}


std::vector<Character> DBSCAN::dfs(int now, int c, std::vector<Character> &points, std::vector<std::vector<int>> &adjPoints)
{
    points[now].setCluster(c);
    for(auto & next: adjPoints[now]) {
        if(points[next].getCluster() != NOT_CLASSIFIED) continue;
        dfs(next, c, points, adjPoints);
    }
    return points;
}


std::vector<std::vector<int>> DBSCAN::assignCluster(std::vector<Character> points)
{
    cluster.resize(clusterIdx+1);
    for(int i = 0; i < size; i++)
        cluster[points[i].getCluster()].push_back(i);

    return cluster;
}



std::vector<Field> DBSCAN::computeFields(std::vector<Character> points, std::vector<std::vector<int>> adjPoints)
{
    std::vector<Field> fields;
    Field field;
    for(int i = 0; i <= clusterIdx; ++i)
    {
        for(size_t j = 0; j < points.size(); ++j)
        {
            if(points[j].getCluster() == i)
                field.setField(points[j].getX(), points[j]);
        }
        field.computeOrderedCharacters();
        field.computeMeanY();
        fields.push_back(field);
        field.clear();
    }
    return fields;
}


void DBSCAN::printOrderedFields(std::vector<Field> fields)
{
    std::cout << "Fields detected:" << std::endl;
    for(auto field: fields)
    {
        std::cout << "Field: ";
        for(auto character: field.getCharacters())
            std::cout << character.getLabel();
        std::cout << std::endl;
    }
}