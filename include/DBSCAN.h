#ifndef DBSCAN_H
#define DBSCAN_H

#include <cmath>
#include <vector>

const int NOISE = -2;
const int NOT_CLASSIFIED = -1;

class myPoint{
    private:
        float x_center;
        float y_center;
        float box_width;
        float box_height;
        float label;
        int countPts;
        int cluster;
    public:
        myPoint(float c_x, float c_y, float w, float h, float label);
        float getX();
        float getY();
        float getLabel();
        int getCountPts();
        int getCluster();
        void setCountPts(int);
        void setCluster(int);
        float distance(myPoint);
        
};


class myDBSCAN
{
    private:
        int n;
        int minPts;
        double eps;
        std::vector<myPoint> points;
        const int size = points.size(); 
        std::vector<std::vector<int>> adjPoints; //???
        int clusterIdx;
        std::vector<std::vector<int>> cluster;

    public:
        myDBSCAN(int n, double eps, int minPts, std::vector<myPoint> points);
        void run();
        void checkNearPoints();
        bool isCoreObject(int idx);
        void dfs(int now, int c);

};

#endif