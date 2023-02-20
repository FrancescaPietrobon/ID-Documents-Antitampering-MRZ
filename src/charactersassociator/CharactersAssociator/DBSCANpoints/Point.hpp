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

    public:
        Point(Characters c, float x, float y, size_t cl);
        float distance(Point);
        float getX();
        float getY();
        size_t getCluster();
        void setCluster(size_t);
        float getConfidence();
        char getLabel();
        Coordinates getPosition();
};