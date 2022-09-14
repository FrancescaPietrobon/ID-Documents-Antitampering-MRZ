#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/highgui/highgui.hpp>

#define FEATURE_WIDTH 800
#define FEATURE_HEIGHT 800
#define DENOISE_PARAM 10

typedef std::vector<std::vector<float>> matrix2D;
typedef std::vector<std::vector<std::vector<float>>> matrix3D;
typedef std::vector<std::vector<std::vector<std::vector<float>>>> matrix4D;

class Document
{
    public:
        Document(const std::string);
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
        float img_w;
        float img_h;
        float xAlter;
        float yAlter;
        std::string imagePath;

};

#endif