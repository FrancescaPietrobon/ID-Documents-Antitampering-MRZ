#ifndef DBSCAN_H
#define DBSCAN_H

#include <iostream>
#include <cmath>
#include <vector>
#include "../include/Character.h"

const int NOISE = -2;
const int NOT_CLASSIFIED = -1;

typedef std::vector<std::vector<float>> matrix2D;

class DBSCAN
{
    private:
        const float eps;
        const int minPts;
        std::vector<Character> points;
        int size; 
        std::vector<std::vector<int>> adjPoints;
        int clusterIdx;
        std::vector<std::vector<int>> cluster;
        void computePoints(std::pair<matrix2D, std::vector<float>> boxesLabels);

    public:
        DBSCAN(float, int, std::pair<matrix2D, std::vector<float>>);
        void run();
        void checkNearPoints();
        bool isCoreObject(int idx);
        void dfs(int now, int c);
        std::vector<Character> getPoints();
        int getClusterIdx();
};

#endif