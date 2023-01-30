#ifndef _OCR_FACTORY_HPP_
#define _OCR_FACTORY_HPP_

#include "Ocr/Ocr.hpp"
#include "Ocr/RetinaNet/OcrRetinaNet.hpp"
#include "exceptions/Exceptions.hpp"

#include <spdlog/spdlog.h>
#include <map>

#define MODELS_FOLDER "/home/f_pietrobon/thesis/MRZ_Antitampering/antitampering_mrz/models/"

enum AlgorithmType
{
    RetinaNet,
    //Yolov7
};



class OcrFactory {
    public: 
        static std::shared_ptr<Ocr> createOCR(const std::string& algorithmType);
};

#endif