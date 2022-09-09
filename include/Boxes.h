#ifndef _BOXES_H_
#define _BOXES_H_

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/highgui/highgui.hpp>

#define FEATURE_WIDTH 800
#define FEATURE_HEIGHT 800

typedef std::vector<std::vector<float>> matrix2D;
typedef std::vector<std::vector<std::vector<float>>> matrix3D;
typedef std::vector<std::vector<std::vector<std::vector<float>>>> matrix4D;

class Boxes
{
    public:
        Boxes(cv::Mat);
        cv::Mat getPred();
        matrix2D getPredReshaped();
        matrix2D getCorners();
        matrix2D setPredReshaped(matrix2D);
        void computeCorners(matrix2D);

    private:
        cv::Mat pred;
        matrix2D predReshaped;
        matrix2D corners;
        matrix2D extractPredCVMat(cv::Mat);


};

#endif