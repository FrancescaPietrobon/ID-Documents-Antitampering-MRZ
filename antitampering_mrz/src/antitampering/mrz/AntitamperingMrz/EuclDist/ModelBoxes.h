#ifndef _MODEL_BOXES_H_
#define _MODEL_BOXES_H_

#include <iostream>
#include <vector>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "Boxes.h"
#include "Anchors.h"
#include "utilities.h"

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>

typedef std::vector<std::vector<float>> matrix2D;

class ModelBoxes: public Boxes
{
    private:
        Anchors *anchors = nullptr;
        const cv::Mat predictions;
        const int numClasses;
        float thresholdIOU;
        float thresholdNMS;
        matrix2D CVMatToMatrix2D(cv::Mat);
        matrix2D multVariance(matrix2D);
        matrix2D applySigmoid(matrix2D);
        matrix2D computeCenters(matrix2D boxes, matrix2D anchorBoxes);
        matrix2D computeBoxes(matrix2D);
        std::pair<matrix2D, std::vector<float>> computeNMS(matrix2D boxesPreNMS, matrix2D classPred, float threshold, float threshold_nms);
        matrix2D reshapeBoxes(matrix2D);

    public:
        ModelBoxes(Anchors *anchors, Document doc, const cv::Mat pred, const int numClasses, float thresholdIOU, float thresholdNMS);
        void printBoxPred(matrix2D);
        void printClassPred(matrix2D);        
        std::pair<matrix2D, std::vector<float>> predict();
};

#endif