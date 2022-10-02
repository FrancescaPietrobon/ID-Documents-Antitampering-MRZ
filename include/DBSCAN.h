#ifndef DBSCAN_H
#define DBSCAN_H

#include <iostream>
#include <cmath>
#include <vector>
#include "../include/Character.h"

const int NOISE = -2;
const int NOT_CLASSIFIED = -1;

class myDBSCAN
{
    private:
        int minPts;
        double eps;
        std::vector<Character> points;
        const int size = points.size(); 
        std::vector<std::vector<int>> adjPoints; //???
        int clusterIdx;
        std::vector<std::vector<int>> cluster;

    public:
        myDBSCAN(double eps, int minPts, std::vector<Character> points);
        void run();
        void checkNearPoints();
        bool isCoreObject(int idx);
        void dfs(int now, int c);
        std::vector<Character> getPoints();
        int getClusterIdx();

};

#endif