#ifndef BOXES_H
#define BOXES_H

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "Document.h"

#define FEATURE_WIDTH 800
#define FEATURE_HEIGHT 800

typedef std::vector<std::vector<float>> matrix2D;
typedef std::vector<std::vector<std::vector<float>>> matrix3D;
typedef std::vector<std::vector<std::vector<std::vector<float>>>> matrix4D;

class Boxes
{
    protected:
        Document document;
        

    public:
        explicit Boxes(Document);

};

#endif