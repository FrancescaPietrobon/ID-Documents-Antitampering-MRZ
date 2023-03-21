#pragma once

#include "common/utils/utils.hpp"

#include <spdlog/spdlog.h>
#include <opencv2/opencv.hpp>

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
