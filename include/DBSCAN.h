#ifndef _DBSCAN_H_
#define _DBSCAN_H_

#include <cmath>
#include <vector>

const int NOISE = -2;
const int NOT_CLASSIFIED = -1;

class Point{
    private:
        double x, y;
        int countPts, cluster;
    public:
        double distance(const Point &);
        int getCountPts();
        void setCountPts(int);
        int getCluster();
        void setCluster(int);

    };

class DBSCAN
{
    private:
        int n, minPts;
        double eps;
        
        const int size = points.size();
        std::vector<std::vector<int>> adjPoints;
        std::vector<bool> visited;
        std::vector<std::vector<int>> cluster;
        int clusterIdx;
    public:
        std::vector<Point> points;
        DBSCAN(int n, double eps, int minPts, std::vector<Point> points);
        void checkNearPoints();
        void run();
        void dfs (int, int);
        std::vector<std::vector<int> > getCluster();
        bool isCoreObject(int);


};

#endif