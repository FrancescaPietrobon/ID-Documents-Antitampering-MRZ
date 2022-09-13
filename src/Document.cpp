#include "../include/Document.h"

Document::Document(const std::string path):
    imagePath(path)
    {
        inputImage = cv::imread(imagePath, cv::IMREAD_COLOR);
        img_w = inputImage.size[1];
        img_h = inputImage.size[0];
        xAlter = FEATURE_WIDTH / img_w;
        yAlter = FEATURE_HEIGHT / img_h;

        preprocessing();
        blob = cv::dnn::blobFromImage(imagePreprocessed, 1.0, cv::Size(FEATURE_WIDTH, FEATURE_HEIGHT), cv::Scalar(103.939, 116.779, 123.68), true, false);
    
    }

void Document::preprocessing()
{
    cv::fastNlMeansDenoising(inputImage, imagePreprocessed, DENOISE_PARAM);
    cv::resize(imagePreprocessed, imagePreprocessed, cv::Size(FEATURE_WIDTH, FEATURE_HEIGHT), 0, 0, cv::INTER_CUBIC);
    blob = cv::dnn::blobFromImage(imagePreprocessed, 1.0, cv::Size(FEATURE_WIDTH, FEATURE_HEIGHT), cv::Scalar(103.939, 116.779, 123.68), true, false);
}


cv::Mat Document::getBlob()
{
    return blob;
}

cv::Mat Document::getDocument()
{
    return imagePreprocessed;
}

float Document::getXAlter()
{
    return xAlter;
}

float Document::getYAlter()
{
    return yAlter;
}