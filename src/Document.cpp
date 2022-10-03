#include "../include/Document.h"

Document::Document(const std::string path, int w, int h, float dp):
    imagePath(path), width(w), height(h), denoiseParam(dp)
    {
        inputImage = cv::imread(imagePath, cv::IMREAD_COLOR);
        img_w = inputImage.size[1];
        img_h = inputImage.size[0];
        xAlter = width / img_w;
        yAlter = height / img_h;

        preprocessing();
        blob = cv::dnn::blobFromImage(imagePreprocessed, 1.0, cv::Size(width, height), cv::Scalar(103.939, 116.779, 123.68), true, false);
    }


void Document::preprocessing()
{
    cv::fastNlMeansDenoising(inputImage, imagePreprocessed, denoiseParam);
    cv::resize(imagePreprocessed, imagePreprocessed, cv::Size(width, height), 0, 0, cv::INTER_CUBIC);
    blob = cv::dnn::blobFromImage(imagePreprocessed, 1.0, cv::Size(width, height), cv::Scalar(103.939, 116.779, 123.68), true, false);
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