#ifndef MODEL_BOXES_H
#define MODEL_BOXES_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "Boxes.h"
#include "utilities.h"

typedef std::vector<std::vector<float>> matrix2D;

class ModelBoxes: public Boxes
{
    private:
        const cv::Mat predictions;
        const int numClasses;
        cv::Mat boxPredCVMat;
        cv::Mat classPredCVMat;
        matrix2D boxPred;
        matrix2D classPred;
        matrix2D boxesPreNMS;
        matrix2D extractPredCVMat(cv::Mat);
        

    public:
        ModelBoxes(Document doc, const cv::Mat pred, const int numClasses);
        matrix2D getBoxPred();
        matrix2D getBoxesPreNMS();
        matrix2D getBoxesReshaped();
        void computeBoxes(matrix2D);
        void computeNMS(float threshold, float threshold_nms);
        void printBoxPred();
        void printClassPred();
        void reshapeBoxes();
        void applySigmoid(matrix2D &);
        void multVariance(matrix2D &);
};

#endif