#ifndef _OCR_HPP_
#define _OCR_HPP_

#include <spdlog/spdlog.h>
#include <opencv2/opencv.hpp>

struct Coordinates {
    float topLeftX;
    float topLeftY;
    float bottomRightX;
    float bottomRightY;
};

struct Characters {
    char label;
    size_t labelIndex;
    Coordinates position;
    float confidence;
};

class Ocr {
    public:
        Ocr(){};
        virtual ~Ocr(){};
        virtual std::vector<Characters> detect(const cv::Mat image, const float confidenceThreshold) = 0;
};

#endif /* _OCR_HPP_ */