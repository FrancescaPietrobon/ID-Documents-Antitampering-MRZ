#ifndef _OCR_HPP_
#define _OCR_HPP_

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/highgui/highgui.hpp>

struct Coordinates {
    float topLeftX;
    float topLeftY;
    float bottomRightX;
    float bottomRightY;
};

struct OcrData {
    char label;
    size_t labelIndex;
    Coordinates position;
    float confidence;
};

class Ocr {
    public:
        Ocr(){};
        virtual ~Ocr(){};
        virtual std::vector<OcrData> detect(const cv::Mat image, const float confidenceThreshold) = 0;
};



#endif /* _OCR_HPP_ */