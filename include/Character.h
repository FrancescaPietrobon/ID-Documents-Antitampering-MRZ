#ifndef POINTS_H
#define POINTS_H

#include <cmath>
#include <vector>
#include <string>
#include <unordered_map>
//#include "../src/Dictionary.cpp"

class Character{
    private:
        float x_center;
        float y_center;
        float box_width;
        float box_height;
        float labelIdx;
        std::string label;
        int countPts;
        int cluster;
    public:
        Character(float c_x, float c_y, float w, float h, float label, int cl);
        float getX();
        float getY();
        std::string getLabel();
        float getLabelIdx();
        int getCountPts();
        int getCluster();
        void setCountPts(int);
        void setCluster(int);
        float distance(Character);  
};

#endif