#include <iostream>
#include <vector>
#include <numeric>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "src/DBSCAN.h"
#include "src/Characters.h"
#include "src/Anchors.h"

#define FEATURE_WIDTH 800
#define FEATURE_HEIGHT 800
#define NUM_CLASSES 64

typedef std::vector<std::vector<double>> matrix2D;
typedef std::vector<std::vector<std::vector<double>>> matrix3D;
typedef std::vector<std::vector<std::vector<std::vector<double>>>> matrix4D;


matrix2D extractPredCVMat(cv::Mat boxPrediction)
{
    matrix2D boxPred;
    std::vector<double> box;
    cv::Vec<int,4> idx;
    double val;

    for(int i = 0; i < boxPrediction.size[3]; ++i)
    {
        for(int j = 0; j < boxPrediction.size[1]; ++j)
        {
            idx = {0, j, 0, i};
            val = boxPrediction.at<double>(idx);
            //std::cout << val << " ";
            box.push_back(val);
        }
        //std::cout << " " << std::endl;
        boxPred.push_back(box);
        box.clear();
        
    }
    return boxPred;
}

int main()
{
    // Load network
    std::string networkWeights = "/home/f_pietrobon/thesis/MRZ_Antitampering/models/Frozen_graph_test_0.25.pb";

    cv::dnn::Net network = cv::dnn::readNetFromTensorflow(networkWeights);

    cv::Mat document = cv::imread("/home/f_pietrobon/thesis/MRZ_Antitampering/AFG_AO_01001_FRONT.JPG", cv::IMREAD_COLOR);
    cv::resize(document, document, cv::Size(FEATURE_WIDTH, FEATURE_HEIGHT), 0, 0, cv::INTER_CUBIC); 

    // network prediction
    cv::Mat blob = cv::dnn::blobFromImage(document, 1.0, cv::Size(FEATURE_WIDTH, FEATURE_HEIGHT), cv::Scalar(0,0,0), true, false, CV_32F);

    network.setInput(blob);

    cv::Mat prediction = network.forward();
    cv::Range boxRange(0, 4);
    cv::Range classRange(4, NUM_CLASSES + 4);
    cv::Range all(cv::Range::all());
    std::vector<cv::Range> boxRanges = {all, boxRange, all, all};
    std::vector<cv::Range> classRanges = {all, classRange, all, all};

    cv::Mat boxPrediction = prediction(boxRanges);
    std::cout << "Box pred:\t";
    std::cout << boxPrediction.size[0] << " x " << boxPrediction.size[1] << " x " << boxPrediction.size[2] << " x " << boxPrediction.size[3] << std::endl;
    
    cv::Mat classPrediction = prediction(classRanges);
    std::cout << "Class pred:\t";
    std::cout << classPrediction.size[0] << " x " << classPrediction.size[1] << " x " << classPrediction.size[2] << " x " << classPrediction.size[3] << std::endl;

    matrix2D boxPred = extractPredCVMat(boxPrediction);
    std::cout << "New box pred:\t";
    std::cout << boxPred.size() << " x " << boxPred[0].size() << std::endl;

    matrix2D classPred = extractPredCVMat(classPrediction);
    std::cout << "New class pred:\t";
    std::cout << classPred.size() << " x " << classPred[0].size() << std::endl;
    
    //Anchors anchors;
    //anchors.anchorsGenerator();

    // NON MAX SUPPRESSION WITH cv::NMSBoxes()

    // Uncomment to see the time needed to load the network
    //auto start_network = std::chrono::high_resolution_clock::now();
    //cv::Mat detections = network.forward();
    //auto stop_network = std::chrono::high_resolution_clock::now();
    //auto duration_network = std::chrono::duration_cast<std::chrono::milliseconds>(stop_network - start_network);
    //std::cout << "Time taken to predict: " << duration_network.count() << " milliseconds" << std::endl;
    
    //std::cout << detections.size[0] << " " << detections.size[1] << " " << detections.size[2] << " " << detections.size[3] << std::endl;
    //cv::Mat output(detections.size[2], detections.size[3], CV_32F, detections.ptr<float>());
    //std::cout << output << std::endl;    

    return 0;
}