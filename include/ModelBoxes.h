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
        cv::Mat predictions;
        cv::Mat boxPredCVMat;
        cv::Mat classPredCVMat;
        matrix2D boxPred;
        matrix2D classPred;
        matrix2D boxesPreNMS;
        int numClasses;
        matrix2D extractPredCVMat(cv::Mat);
        

    public:
        ModelBoxes(Document doc, cv::Mat pred, int numClasses);
        matrix2D getBoxPred();
        matrix2D getClassPred();
        cv::Mat getBoxPredCVMat();
        cv::Mat getClassPredCVMat();
        matrix2D getBoxesPreNMS();
        matrix2D getBoxesReshaped();
        void computeBoxes(matrix2D);
        void computeNMS(float threshold, float threshold_nms);
        void printBoxPred();
        void printClassPred();
        void reshapeBoxes();
        void applySigmoid(matrix2D &);
        void multBias(matrix2D &);

};

#endif