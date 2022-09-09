#include "../include/Document.h"

Document::Document(std::string path):
    imagePath(path)
    {
        inputImage = cv::imread(imagePath, cv::IMREAD_COLOR);
    }

void Document::preprocessing()
{
    cv::fastNlMeansDenoising(inputImage, imagePreprocessed, DENOISE_PARAM);
    cv::resize(imagePreprocessed, imagePreprocessed, cv::Size(FEATURE_WIDTH, FEATURE_HEIGHT), 0, 0, cv::INTER_CUBIC);
    blob = cv::dnn::blobFromImage(imagePreprocessed, 1.0, cv::Size(FEATURE_WIDTH, FEATURE_HEIGHT), cv::Scalar(103.939, 116.779, 123.68), true, false);
}


cv::Mat Document::GetBlob()
{
    return blob;
}

cv::Mat Document::GetDocument()
{
    return imagePreprocessed;
}
