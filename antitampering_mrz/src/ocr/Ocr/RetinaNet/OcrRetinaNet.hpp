#ifndef _OCR_RETINANET_HPP_
#define _OCR_RETINANET_HPP_

#include "../Ocr.hpp"
#include "../../exceptions/Exceptions.hpp"
#include "Anchors.hpp"
#include "Dictionary.hpp"

#include <opencv2/opencv.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <vector>
#include <fmt/core.h>

#define NUM_CLASSES 63
#define DENOISE_PARAM 10
#define THRESHOLD_NMS 0.005

typedef std::vector<std::vector<float>> matrix2D;

class OcrRetinaNet : public Ocr 
{
    public:
        OcrRetinaNet(const cv::dnn::Net model, cv::Size modelInputSize);
        virtual ~OcrRetinaNet(){};
        std::vector<OcrData> detect(const cv::Mat image, const float confidenceThreshold) override;
    
    protected:
        cv::dnn::Net model;
        cv::Size modelInputSize;
        float xAlter;
        float yAlter;

    private:
        cv::Mat inference(const cv::Mat& image);
        std::pair<matrix2D, matrix2D> adjustPredictions(cv::Mat prediction);
        matrix2D CVMatToMatrix2D(cv::Mat cvMat);
        matrix2D multVariance(matrix2D box);
        matrix2D applySigmoid(matrix2D A);
        matrix2D computeCenters(matrix2D boxes, matrix2D anchorBoxes);
        matrix2D computeBoxes(matrix2D);
        Coordinates reshapeBox(std::vector<float>);
        cv::Mat imagePreprocessing(const cv::Mat& image);
        std::vector<OcrData> nonMaximaSuppression(matrix2D boxesPreNMS, matrix2D classPred, float confidenceThreshold);

        void printPredictions(matrix2D pred);
        void savePredictionImage(cv::Mat img, std::vector<OcrData> characters);
};

#endif