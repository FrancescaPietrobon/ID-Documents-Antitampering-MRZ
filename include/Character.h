#ifndef POINTS_H
#define POINTS_H

#include <cmath>
#include <vector>
#include <string>
#include <unordered_map>


class Character
{
    private:
        float xCenter;
        float yCenter;
        float labelIdx;
        std::string label;
        int countPts = 0;
        int cluster = 0;

    public:
        Character(float c_x, float c_y, float label, int cl);
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