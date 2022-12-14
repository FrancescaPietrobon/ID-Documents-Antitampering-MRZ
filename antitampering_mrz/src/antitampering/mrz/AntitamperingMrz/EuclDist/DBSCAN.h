#ifndef _CLUSTER_H_
#define _CLUSTER_H_

#include <iostream>
#include <cmath>
#include <vector>
#include "Character.h"
#include "Field.h"

const int NOT_CLASSIFIED = -1;

typedef std::vector<std::vector<float>> matrix2D;

class DBSCAN
{
    private:
        const float eps;
        std::vector<Character> points;
        int size; 
        std::vector<std::vector<int>> adjPoints;
        int clusterIdx = -1;
        std::vector<std::vector<int>> cluster;
        std::vector<Character> computePointsForCharacters(std::pair<matrix2D, std::vector<float>> boxesLabels);
        std::vector<std::vector<int>> assignCluster(std::vector<Character> points);

    public:
        DBSCAN(float, std::pair<matrix2D, std::vector<float>>);
        std::vector<Field> computeFields(std::vector<Character> points, std::vector<std::vector<int>> adjPoints);
        std::vector<Field> extractFields();
        std::vector<std::vector<int>> findNearPoints(std::vector<Character> points);
        bool isCoreObject(int idx);
        std::vector<Character> dfs(int now, int c, std::vector<Character> &points, std::vector<std::vector<int>> &adjPoints);
        std::vector<Character> getPoints();
        int getClusterIdx();
        std::vector<std::vector<int>> getAdjPoints();
        void printOrderedFields(std::vector<Field> fields);
};

#endif