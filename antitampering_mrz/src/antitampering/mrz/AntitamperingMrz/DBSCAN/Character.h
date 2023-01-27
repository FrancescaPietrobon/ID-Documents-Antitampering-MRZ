#ifndef _CHARACTER_H_
#define _CHARACTER_H_

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
        int cluster = 0;
        int countPts = 0;

    public:
        Character(float c_x, float c_y, float label, int cl);
        float distance(Character);
        float getX();
        float getY();
        std::string getLabel();
        float getLabelIdx();
        int getCluster();
        void setCluster(int);
        void setCountPts(int count);
        int getCountPts();
};

#endif