#pragma once

#include "ocr_server/ocr/OcrApi.hpp"
#include "common/utils/utils.hpp"

#include <spdlog/spdlog.h>
#include <opencv2/opencv.hpp>

class CharactersClustering {
    public:
        CharactersClustering(){};
        virtual ~CharactersClustering(){};
        virtual std::vector<Field> clusterCharacters(const Characters *characters, const size_t charactersSize) = 0;
};
