#ifndef POINTS_H
#define POINTS_H

#include <cmath>
#include <vector>
#include <string>

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
        myPoint(float c_x, float c_y, float w, float h, float label, int cl);
        float getX();
        float getY();
        float getLabel();
        int getCountPts();
        int getCluster();
        void setCountPts(int);
        void setCluster(int);
        float distance(myPoint);  
};

#endif