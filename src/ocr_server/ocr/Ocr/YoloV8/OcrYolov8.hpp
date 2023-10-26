#pragma once

#include "../Ocr.hpp"

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <onnxruntime_cxx_api.h>

template <typename T>
T clip(const T& n, const T& lower, const T& upper)
{
    return std::max(lower, std::min(n, upper));
}

class OcrYolov8: public Ocr
{
    public:
        OcrYolov8(std::string& modelPath, const cv::Size modelInputSize, const cv::Scalar meanValue, double confThreshold, double iouThreshold, bool binaryImg);
        ~OcrYolov8() = default;
        std::vector<Character> detect(const cv::Mat image, const float confidenceThreshold) override;
        std::vector<Character> resultImage(const cv::Mat frame);
        std::vector<Character> nonMaximaSuppression(std::vector<Character> detections, double confThreshold, double iouThreshold);

    private:
        Ort::Env env;
        Ort::Session session;

        const cv::Size modelInputSize;
        const cv::Scalar meanValue;
        double confThreshold;
        double iouThreshold;
        bool binaryImg;

        cv::Rect2f scaleCoords(const cv::Size& imageShape, cv::Rect2f coords, const cv::Size& imageOriginalShape, bool p_Clip);
        std::tuple<int, float> getBestClassInfo(const cv::Mat& p_Mat, const int& numClasses);
        std::vector<Character> postprocessing(const cv::Size& resizedImageShape, const cv::Size& originalImageShape, std::vector<Ort::Value>& outputTensors,
                                            double confThreshold, double iouThreshold);
        cv::Mat letterbox(const cv::Mat image, const cv::Size& modelInputSize, const cv::Scalar& meanValue, bool auto_, bool scaleFill, bool scaleUp, int stride);
        std::vector<float> blobImage(cv::Mat image);
};
