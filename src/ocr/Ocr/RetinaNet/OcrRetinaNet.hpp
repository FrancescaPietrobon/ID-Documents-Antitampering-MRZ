#ifndef _OCR_RETINANET_HPP_
#define _OCR_RETINANET_HPP_

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
        OcrRetinaNet(const cv::dnn::Net model, cv::Size modelInputSize);
        virtual ~OcrRetinaNet(){};
        std::vector<Characters> detect(const cv::Mat image, const float confidenceThreshold) override;
    
    protected:
        cv::dnn::Net model;
        cv::Size modelInputSize;
        float xAlter;
        float yAlter;

    private:
        cv::Mat inference(const cv::Mat& image);
        cv::Mat imagePreprocessing(const cv::Mat& image);
        std::vector<Characters> nonMaximaSuppression(matrix2D boxesPreNMS, matrix2D classPred, float confidenceThreshold);
        std::pair<matrix2D, matrix2D> adjustModelPredictions(cv::Mat prediction);
        matrix2D computeBoxes(matrix2D modelBoxes, matrix2D anchorBoxes);
        std::unordered_map<unsigned, char> dictionary = {{0, '0',}, {1, '1',}, {2, '2',}, {3, '3',}, {4, '4',},
                                                         {5, '5',}, {6, '6',}, {7, '7',}, {8, '8',}, {9, '9',},
                                                         {10, 'A',}, {11, 'a',}, {12, 'B',}, {13, 'b',}, {14, 'C',},
                                                         {15, 'c',}, {16, 'D',}, {17, 'd',}, {18, 'E',}, {19, 'e',},
                                                         {20, 'F',}, {21, 'f',}, {22, 'G',}, {23, 'g',}, {24, 'H',},
                                                         {25, 'h',}, {26, 'I',}, {27, 'i',}, {28, 'J',}, {29, 'j',},
                                                         {30, 'K',}, {31, 'k',}, {32, 'L',}, {33, 'l',}, {34, 'M',},
                                                         {35, 'm',}, {36, 'N',}, {37, 'n',}, {38, 'O',}, {39, 'o',},
                                                         {40, 'P',}, {41, 'p',}, {42, 'Q',}, {43, 'q',}, {44, 'R',},
                                                         {45, 'r',}, {46, 'S',}, {47, 's',}, {48, 'T',}, {49, 't',},
                                                         {50, 'U',}, {51, 'u',}, {52, 'V',}, {53, 'v',}, {54, 'W',},
                                                         {55, 'w',}, {56, 'X',}, {57, 'x',}, {58, 'Y',}, {59, 'y',},
                                                         {60, 'Z',}, {61, 'z',}, {62, '<',}};
};

#endif