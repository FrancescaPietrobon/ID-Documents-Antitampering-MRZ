#include "Document.h"

Document::Document(const std::string path, const int w, const int h, const float dp):
    imagePath(path), width(w), height(h), denoiseParam(dp){}


cv::Mat Document::preprocessing()
{
    inputImage = cv::imread(imagePath);
    float imgWidth = inputImage.size[1];
    float imgHeight = inputImage.size[0];
    xAlter = width / imgWidth;
    yAlter = height / imgHeight;

    cv::fastNlMeansDenoising(inputImage, imagePreprocessed, denoiseParam);
    cv::resize(imagePreprocessed, imagePreprocessed, cv::Size(width, height), 0, 0, cv::INTER_CUBIC);

    return imagePreprocessed;
}

cv::Mat Document::getDocument()
{
    return imagePreprocessed;
}

cv::Mat Document::getInputImage()
{
    return inputImage;
}

float Document::getXAlter()
{
    return xAlter;
}

float Document::getYAlter()
{
    return yAlter;
}

int Document::getWidth()
{
    return width;
}

int Document::getHeight()
{
    return height;
}