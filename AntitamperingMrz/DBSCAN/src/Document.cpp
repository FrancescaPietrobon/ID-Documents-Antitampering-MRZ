#include "../include/Document.h"

Document::Document(const std::string path, const int w, const int h, const float dp):
    imagePath(path), width(w), height(h), denoiseParam(dp)
    {
        inputImage = cv::imread(imagePath);
        float imgWidth = inputImage.size[1];
        float imgHeight = inputImage.size[0];
        xAlter = width / imgWidth;
        yAlter = height / imgHeight;

        preprocessing();
        blob = cv::dnn::blobFromImage(imagePreprocessed, 1.0, cv::Size(width, height), cv::Scalar(103.939, 116.779, 123.68), true, false);
    }


void Document::preprocessing()
{
    cv::fastNlMeansDenoising(inputImage, imagePreprocessed, denoiseParam);
    cv::resize(imagePreprocessed, imagePreprocessed, cv::Size(width, height), 0, 0, cv::INTER_CUBIC);
}

cv::Mat Document::getBlob()
{
    return blob;
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