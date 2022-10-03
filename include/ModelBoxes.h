#ifndef MODEL_BOXES_H
#define MODEL_BOXES_H

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "Boxes.h"
#include "utilities.h"

#define FEATURE_WIDTH 800
#define FEATURE_HEIGHT 800
#define NUM_CLASSES 64

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
        matrix2D extractPredCVMat(cv::Mat);
        void applySigmoid(matrix2D &);

    public:
        ModelBoxes(Document doc, cv::Mat pred);
        matrix2D getBoxPred();
        matrix2D getClassPred();
        matrix2D getBoxesPreNMS();
        matrix2D getBoxesReshaped();
        void computeBoxes(matrix2D);
        void computeNMS(float threshold, float threshold_nms);
        void printPredCVMat();
        void reshapeBoxes();

};

#endif