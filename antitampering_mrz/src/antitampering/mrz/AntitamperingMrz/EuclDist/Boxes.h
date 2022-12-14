#ifndef _BOXES_H_
#define _BOXES_H_

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "Document.h"

typedef std::vector<std::vector<float>> matrix2D;

class Boxes
{
    protected:
        Document document;
        matrix2D boxes;
        std::vector<float> classes;

    public:
        Boxes(Document);
        matrix2D getBoxes();
        std::vector<float> getClasses();
};

#endif