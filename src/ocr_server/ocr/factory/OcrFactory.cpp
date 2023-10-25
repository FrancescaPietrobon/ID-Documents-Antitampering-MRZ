#include "OcrFactory.hpp"

std::map<std::string, AlgorithmType> mapAlgorithmType = {
    {"RetinaNet", RetinaNet},
    {"YoloV8", YoloV8}
};

std::shared_ptr<Ocr> OcrFactory::createOCR(const std::string& algorithmType)
{
    SPDLOG_INFO("Creating OCR");
    AlgorithmType type = mapAlgorithmType.find(algorithmType)->second;
    switch (type)
    {
        case RetinaNet:
        {
            SPDLOG_INFO("Loading RetinaNet OCR");
            std::string modelsFolder = std::string(MODELS_FOLDER);
            std::string modelFilename = modelsFolder + std::string("ocr_5e6_156img.pb");

            cv::dnn::Net model = cv::dnn::readNetFromTensorflow(modelFilename);
            cv::Size netInputSize = cv::Size(800, 800);
            return std::shared_ptr<Ocr>(std::make_shared<OcrRetinaNet>(model, netInputSize));
        }
        case YoloV8:
        {
            SPDLOG_INFO("Loading RetinaNet OCR");
            std::string modelsFolder = std::string(MODELS_FOLDER);
            std::string modelFilename = modelsFolder + std::string("ocr-yolov8.onnx");
            const cv::Size modelInputSize = cv::Size(640, 640);
            const cv::Scalar meanValues = cv::Scalar(114, 114, 114);
            double confThreshold = 0.3;
            double iouThreshold = 0.5;
            return std::shared_ptr<Ocr>(std::make_shared<OcrYolov8>(modelFilename, modelInputSize, meanValues, confThreshold, iouThreshold));
        }
        default:
        {
            SPDLOG_ERROR("FATAL: Algorithm Type NOT FOUND,  {}", algorithmType);
            throw Exception(ErrorCode::DETECTOR_TYPE_NOT_FOUND, std::string("Algorithm Type not handled"));
        }
    }
}
