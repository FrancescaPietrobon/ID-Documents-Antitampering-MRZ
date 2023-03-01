#pragma once

#include "ocr_server/ocr/OcrApi.hpp"
#include "common/utils/utils.hpp"

#include <spdlog/spdlog.h>
#include <opencv2/opencv.hpp>

struct Fields {
    char* label;
    size_t labelSize;
    Coordinates position;
    float confidence;
    
    bool operator==(Fields const & rhs) const {
        return (this->label == rhs.label && this->labelSize == rhs.labelSize && this->confidence == rhs.confidence);
    }
};

class CharactersClustering {
    public:
        CharactersClustering(){};
        virtual ~CharactersClustering(){};
        virtual std::vector<Fields> clusterCharacters(const Characters *characters, const size_t charactersSize) = 0;
};