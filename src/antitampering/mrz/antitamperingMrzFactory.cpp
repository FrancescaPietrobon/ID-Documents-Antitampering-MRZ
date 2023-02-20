#include "antitamperingMrzFactory.hpp"

std::map<std::string, AlgorithmType> mapAlgorithmType = {
    {"wer", wer},
    //{"boh", boh}
};

std::shared_ptr<Ocr> OcrFactory::createOCR(const std::string& algorithmType)
{
    SPDLOG_INFO("Creating OCR");
    AlgorithmType type = mapAlgorithmType.find(algorithmType)->second;
    switch (type)
    {
        case wer:
            return std::shared_ptr<Ocr>(std::make_shared<OcrRetinaNet>(model, netInputSize));
        default:
        {
            SPDLOG_ERROR("FATAL: Algorithm Type NOT FOUND,  {}", algorithmType);
            throw PhygitalAIException(PhygitalAIErrorCode::DETECTOR_TYPE_NOT_FOUND, std::string("Algorithm Type not handled"));
        }
    }
}