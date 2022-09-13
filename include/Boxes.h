#ifndef _BOXES_H_
#define _BOXES_H_

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
    private:
        Document document;
        cv::Mat pred;
        double xAlter;
        double yAlter;
        matrix2D predMatrix;
        matrix2D corners;
        matrix2D extractPredCVMat(cv::Mat);

    public:
        Boxes(Document, cv::Mat);
        cv::Mat getPred();
        matrix2D getPredMatrix();
        matrix2D getCorners();
        void setpredMatrix(matrix2D);
        void computeCorners(matrix2D);
        void printPredCVMat();

};

#endif