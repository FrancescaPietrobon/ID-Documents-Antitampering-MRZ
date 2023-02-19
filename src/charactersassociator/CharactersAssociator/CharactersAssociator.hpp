#pragma once

#include "ocr/OcrApi.hpp"
#include "common/utils/utils.hpp"

#include <spdlog/spdlog.h>
#include <opencv2/opencv.hpp>

struct Fields {
    char* label;
    size_t labelSize;
    Coordinates position;
    float confidence;
};

class CharactersAssociator {
    public:
        CharactersAssociator(){};
        virtual ~CharactersAssociator(){};
        virtual std::vector<Fields> associateCharacters(const Characters *characters, const size_t charactersSize) = 0;
};