#pragma once

#include "../Ocr/RetinaNet/OcrRetinaNet.hpp"
#include "common/exceptions/Exceptions.hpp"

#include <spdlog/spdlog.h>
#include <map>

#define MODELS_FOLDER "/home/f_pietrobon/thesis/MRZ_Antitampering/models/"

enum AlgorithmType
{
    RetinaNet = 1,
    //Yolov7
};

class OcrFactory {
    public: 
        static std::shared_ptr<Ocr> createOCR(const std::string& algorithmType);
};
