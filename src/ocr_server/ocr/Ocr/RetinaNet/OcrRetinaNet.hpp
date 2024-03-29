#pragma once

#include "../Ocr.hpp"
#include "common/exceptions/Exceptions.hpp"
#include "Anchors.hpp"
#include "UtilitiesRetinaNet.hpp"

#include <opencv2/opencv.hpp>
#include <vector>
#include <fmt/core.h>

#define NUM_CLASSES 63
#define DENOISE_PARAM 10
#define THRESHOLD_NMS 0.005
#define VARIANCE_X 0.1
#define VARIANCE_Y 0.2

typedef std::vector<std::vector<float>> matrix2D;

class OcrRetinaNet : public Ocr 
{
    public:
        OcrRetinaNet(const cv::dnn::Net model, cv::Size modelInputSize, bool binaryImg);
        virtual ~OcrRetinaNet(){};
        std::vector<Character> detect(const cv::Mat image, const float confidenceThreshold) override;
    
    protected:
        cv::dnn::Net model;
        cv::Size modelInputSize;
        bool binaryImg;
        float xAlter;
        float yAlter;

    private:
        cv::Mat inference(const cv::Mat& image);
        cv::Mat imagePreprocessing(const cv::Mat& inputImage, bool binaryImg);
        std::vector<Character> nonMaximaSuppression(matrix2D boxesPreNMS, matrix2D classPred, float confidenceThreshold);
        std::pair<matrix2D, matrix2D> adjustModelPredictions(cv::Mat prediction);
        matrix2D computeBoxes(matrix2D modelBoxes, matrix2D anchorBoxes);
};
