#ifndef _DOCUMENT_H_
#define _DOCUMENT_H_

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
        Document(std::string);
        void preprocessing();
        cv::Mat GetBlob();
        cv::Mat GetDocument();
        

    private:
        cv::Mat inputImage;
        cv::Mat imagePreprocessed;
        cv::Mat blob;
        std::string imagePath;



};

#endif