#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/highgui/highgui.hpp>

typedef std::vector<std::vector<float>> matrix2D;

class Document
{
    public:
        Document(const std::string, int width, int height, float denoiseParam);
        void preprocessing();
        cv::Mat getBlob();
        cv::Mat getDocument();
        cv::Mat getInputImage();
        float getXAlter();
        float getYAlter();
        
    private:
        cv::Mat inputImage;
        cv::Mat imagePreprocessed;
        cv::Mat blob;
        float denoiseParam;
        int width;
        int height;
        float img_w;
        float img_h;
        float xAlter;
        float yAlter;
        std::string imagePath;
};

#endif