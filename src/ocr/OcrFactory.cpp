#include "OcrFactory.hpp"


std::map<std::string, AlgorithmType> mapAlgorithmType = {
    {"RetinaNet", RetinaNet},
    //{"Yolov7", Yolov7}
};

std::shared_ptr<Ocr> OcrFactory::createOCR(const std::string& algorithmType)
{
    SPDLOG_INFO("Creating OCR");
    AlgorithmType type = mapAlgorithmType.find(algorithmType)->second;
    switch (type)
    {
        case RetinaNet:
        {
            SPDLOG_INFO("Loading OCR");
            std::string modelsFolder = std::string(MODELS_FOLDER);
            std::string modelFilename = modelsFolder + std::string("Frozen_graph_lnorm_5e6_156img.pb");

            cv::dnn::Net model = cv::dnn::readNetFromTensorflow(modelFilename);
            cv::Size netInputSize = cv::Size(800, 800);
            return std::shared_ptr<Ocr>(std::make_shared<OcrRetinaNet>(model, netInputSize));
        }
        default:
        {
            SPDLOG_ERROR("FATAL: Algorithm Type NOT FOUND,  {}", algorithmType);
            throw OcrException(OcrErrorCode::OCR_TYPE_NOT_FOUND, std::string("Algorithm Type not handled"));
        }
    }
}