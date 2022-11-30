#ifndef _DOCUMENT_H_
#define _DOCUMENT_H_

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/highgui/highgui.hpp>

typedef std::vector<std::vector<float>> matrix2D;

class Document
{
    private:
        const std::string imagePath;
        const int width;
        const int height;
        const float denoiseParam;
        float xAlter;
        float yAlter;
        cv::Mat inputImage;
        cv::Mat imagePreprocessed;
        cv::Mat blob;

    public:
        Document(const std::string, const int, const int, const float);
        cv::Mat preprocessing();
        cv::Mat getDocument();
        cv::Mat getInputImage();
        float getXAlter();
        float getYAlter();
        int getWidth();
        int getHeight();
};

#endif