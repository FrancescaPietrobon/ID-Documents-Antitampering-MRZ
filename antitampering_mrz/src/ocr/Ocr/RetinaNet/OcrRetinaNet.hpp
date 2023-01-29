#ifndef _OCR_RETINANET_HPP_
#define _OCR_RETINANET_HPP_

#include "../Ocr.hpp"
#include "../../exceptions/Exceptions.hpp"
#include "Anchors.hpp"
#include "Dictionary.hpp"
#include "UtilitiesRetinaNet.hpp"

#include <opencv2/opencv.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/highgui/highgui.hpp>

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
        OcrRetinaNet(const cv::dnn::Net model, cv::Size modelInputSize);
        virtual ~OcrRetinaNet(){};
        std::vector<OcrData> detect(const cv::Mat &image, const float confidenceThreshold) override;
    
    protected:
        cv::dnn::Net model;
        cv::Size modelInputSize;
        float xAlter;
        float yAlter;

    private:
        cv::Mat inference(const cv::Mat& image);
        cv::Mat imagePreprocessing(const cv::Mat& image);
        std::vector<OcrData> nonMaximaSuppression(matrix2D boxesPreNMS, matrix2D classPred, float confidenceThreshold);
        std::pair<matrix2D, matrix2D> adjustModelPredictions(cv::Mat prediction);
        matrix2D computeBoxes(matrix2D modelBoxes, matrix2D anchorBoxes);
};

#endif