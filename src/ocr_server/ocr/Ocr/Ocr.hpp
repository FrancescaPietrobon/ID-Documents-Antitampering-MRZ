#pragma once

#include "common/utils/utils.hpp"

#include <spdlog/spdlog.h>
#include <opencv2/opencv.hpp>

struct Character {
    char *label;
    size_t labelIndex;
    Coordinates position;
    float confidence;
};

class Ocr {
    public:
        Ocr(){};
        virtual ~Ocr(){};
        virtual std::vector<Character> detect(const cv::Mat image, const float confidenceThreshold) = 0;
    protected:
        cv::Mat binarization(cv::Mat img);
};
