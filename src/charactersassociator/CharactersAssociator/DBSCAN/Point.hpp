#pragma once

#include "../ocr/Ocr/Ocr.hpp"

#include <cmath>
#include <vector>
#include <string>

class Point
{
    private:
        Characters character;
        float xCenter;
        float yCenter;
        size_t cluster = 0;
        size_t countPts = 0;

    public:
        Point(Characters c, float x, float y, size_t cl);
        float distance(Point);
        float getX();
        float getY();
        size_t getCluster();
        void setCluster(size_t);
        void setCountPts(size_t count);
        size_t getCountPts();
        float getConfidence();
        char getLabel();
};