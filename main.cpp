#include <iostream>
#include <vector>
#include <numeric>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "include/DBSCAN.h"
#include "include/Characters.h"
#include "include/Anchors.h"

#define FEATURE_WIDTH 800
#define FEATURE_HEIGHT 800
#define NUM_CLASSES 64

typedef std::vector<std::vector<float>> matrix2D;
typedef std::vector<std::vector<std::vector<float>>> matrix3D;
typedef std::vector<std::vector<std::vector<std::vector<float>>>> matrix4D;

/*
matrix2D extractPredCVMatWrongOrder(cv::Mat boxPrediction)
{
    matrix2D boxPred;
    std::vector<float> box;
    cv::Vec<int,4> idx;

    for(int i = 0; i < boxPrediction.size[3]; ++i)
    {
        for(int j = 0; j < boxPrediction.size[1]; ++j)
        {
            idx = {0, j, 0, i};
            box.push_back(boxPrediction.at<float>(idx));
        }
        boxPred.push_back(box);
        box.clear();
    }
    return boxPred;
}
*/


matrix2D extractPredCVMat(cv::Mat boxPrediction)
{
    // To extract the values in the right order (like in Python)

    matrix2D boxPred;
    std::vector<float> box;
    cv::Vec<int,4> idx;
    int j = 0;
    int i = 0;
    int count = 0;

    while(count < boxPrediction.size[1] * boxPrediction.size[3])
    {
        if(j >= boxPrediction.size[3])
        {
            i += 1;
            j = 0;
        }
        idx = {0, i, 0, j};
        box.push_back(boxPrediction.at<float>(idx));
        count += 1;
        j += 1;
        if(count % boxPrediction.size[1] == 0)
        {
            boxPred.push_back(box);
            box.clear();
        }
    }
    return boxPred;
}


void printPredCVMat(matrix2D boxPrediction)
{
    for(size_t i = 0; i < boxPrediction.size(); ++i)
    {
        for(size_t j = 0; j < boxPrediction.begin()->size(); ++j)
        {
            //if(i < 20)
            //if(i > 200140)
            //if(i == 200135)
            if(i < 20)
                std::cout << boxPrediction[i][j] << " ";
        }
        //if(i < 20)
        //if(i > 200140)
        //if(i == 200135)
        if(i < 20)
            std::cout << " " << std::endl;
    }
}


void applySigmoid(matrix2D &A)
{
    for(size_t i = 0; i < A.size(); ++i)
    {
        for(size_t j = 0; j < A[0].size(); ++j)
        {
            //std::cout << A[i][j] << "\t\t";
            A[i][j] = 1 / (1 + exp(- A[i][j]));
            //A[i][j] = (float)1 / ( 1 + exp(- A[i][j]));
            //std::cout << A[i][j] << std::endl;
        }
    }
}


matrix2D computeCenters(matrix2D boxes, matrix2D anchorBoxes)
{
    matrix2D center = boxes;
    for(size_t i = 0; i < boxes.size(); ++i)
    {
        center[i][0] = boxes[i][0] * anchorBoxes[i][2] + anchorBoxes[i][0];
        center[i][1] = boxes[i][1] * anchorBoxes[i][3] + anchorBoxes[i][1];
        center[i][2] = std::exp(boxes[i][2]) *  anchorBoxes[i][2];
        center[i][3] = std::exp(boxes[i][3]) *  anchorBoxes[i][3];
        //if(i <= 5)
        //    std::cout << boxes[i][0] * anchorBoxes[i][2] + anchorBoxes[i][0] << " " << boxes[i][1] * anchorBoxes[i][3] + anchorBoxes[i][1] << " " << std::exp(boxes[i][2]) *  anchorBoxes[i][2] << " " << std::exp(boxes[i][3]) *  anchorBoxes[i][3] << std::endl;
            //std::cout << anchorBoxes[i][0] << " " << anchorBoxes[i][1] << " " << anchorBoxes[i][2] << " " << anchorBoxes[i][3] << " " << std::endl;
            //std::cout << boxes[i][0] << " " << boxes[i][1] << " " << boxes[i][2] << " " << boxes[i][3] << " " << std::endl;
            //std::cout << center[i][0] << " " << center[i][1] << " " << center[i][2] << " " << center[i][3] << " " << std::endl;
            
    }
    return center;
}


matrix2D computeCorners(matrix2D boxes)
{
    matrix2D corners = boxes;
    for(size_t i = 0; i < boxes.size(); ++i)
    {
        corners[i][0] = boxes[i][0] - boxes[i][2] / 2;
        corners[i][1] = boxes[i][1] - boxes[i][3] / 2;
        corners[i][2] = boxes[i][0] + boxes[i][2] / 2;
        corners[i][3] = boxes[i][1] + boxes[i][3] / 2;
        //if(i < 5)
            //std::cout << boxes[i][0] - boxes[i][2] / 2 << " " << boxes[i][1] - boxes[i][3] / 2 << " " << boxes[i][0] + boxes[i][2] / 2 << " " << boxes[i][1] + boxes[i][3] / 2 << std::endl;
            //std::cout << anchorBoxes[i][0] << " " << anchorBoxes[i][1] << " " << anchorBoxes[i][2] << " " << anchorBoxes[i][3] << " " << std::endl;
            //std::cout << boxes[i][0] << " " << boxes[i][1] << " " << boxes[i][2] << " " << boxes[i][3] << " " << std::endl;
            //std::cout << center[i][0] << " " << center[i][1] << " " << center[i][2] << " " << center[i][3] << " " << std::endl;
            
    }
    return corners;
}

void savePredictionImage(cv::Mat img, matrix2D boxes, std::vector<float> classes, std::string img_name)
{
    cv::Mat new_image = img;
    int fontFace = cv::FONT_HERSHEY_SIMPLEX;
    double fontScale = 0.4;
    int thickness_pred = 1;
    int thickness_rect = 1;
    int lineType = cv::LINE_8;

    for(size_t i = 0; i < classes.size(); ++i)
    {
        cv::rectangle(new_image, cv::Point(boxes[i][0], boxes[i][1]), cv::Point(boxes[i][2], boxes[i][3]), cv::Scalar(255,0,0),thickness_rect, lineType);
        cv::putText(new_image, dictionary[classes[i]],  cv::Point((boxes[i][2] + boxes[i][0])/2 - 4 , boxes[i][1] - 4), fontFace, fontScale, cv::Scalar(0, 0, 0), thickness_pred);

    }
    cv::imwrite(img_name, new_image);
}

void extractImageWithPrediction(cv::Mat img, matrix2D boxes, matrix2D pred, float threshold)
{
    std::vector<float> indices_max;
    std::vector<float> all_max;
    std::vector<float>::iterator max_it;
    float index_max;
    float max;
    matrix2D boxes_new;
    std::vector<cv::Rect2d> boxes_rect;
    
    for(size_t i = 0; i < pred.size(); ++i)
    {
        max = *std::max_element(pred[i].begin(), pred[i].end());
        max_it = std::max_element(pred[i].begin(), pred[i].end());
        index_max = std::distance(pred[i].begin(), max_it);

        if(max > threshold)
        {
            all_max.push_back(max);
            indices_max.push_back(index_max);
            boxes_new.push_back(boxes[i]);
            cv::Rect2d rect(boxes[i][0], boxes[i][1], boxes[i][2] - boxes[i][0], boxes[i][3] - boxes[i][1]);
            boxes_rect.push_back(rect);
        }
    }

    //savePredictionImage(img, boxes_new, indices_max, "../pred_pre_nms.jpg");

    std::vector<int> nmsIndices;
    cv::dnn::dnn4_v20220524::NMSBoxes(boxes_rect, all_max, 0.5, 0.1, nmsIndices, 1., 0);
    
    matrix2D boxes_nms;
    std::vector<float> indices_max_nms;
    std::vector<float> all_max_nms;
    //std::cout << "Number of chars after NMS: " << nmsIndices.size() << std::endl;

    for(size_t i = 0; i < nmsIndices.size(); ++i)
    {
        boxes_nms.push_back(boxes_new[nmsIndices[i]]);
        indices_max_nms.push_back(indices_max[nmsIndices[i]]);
        all_max_nms.push_back(all_max[nmsIndices[i]]);
    }

    savePredictionImage(img, boxes_nms, indices_max_nms, "../pred_post_nms.jpg");
    
}




/*
float IoU(std::vector<float> boxA, std::vector<float> boxB)
{
    // https://pyimagesearch.com/2016/11/07/intersection-over-union-iou-for-object-detection/
    // determine the (x, y)-coordinates of the intersection rectangle
    float xA = std::max(boxA[0], boxB[0]);
	float yA = std::max(boxA[1], boxB[1]);
	float xB = std::min(boxA[2], boxB[2]);
	float yB = std::min(boxA[3], boxB[3]);
    
    // compute the area of intersection rectangle
    float interArea = std::max(static_cast<float>(0.), xB - xA + 1) * std::max(static_cast<float>(0.), yB - yA + 1);

    // compute the area of both the prediction and ground-truth rectangles
	float boxAArea = (boxA[2] - boxA[0] + 1) * (boxA[3] - boxA[1] + 1);
	float boxBArea = (boxB[2] - boxB[0] + 1) * (boxB[3] - boxB[1] + 1);

    // compute the intersection over union by taking the intersection
	// area and dividing it by the sum of prediction + ground-truth
	// areas - the interesection area
	float iou = interArea / (boxAArea + boxBArea - interArea);

    return iou;
}
*/

/*
void NonMaxSuppression(matrix2D boxes, float threshold, matrix2D)
{
    std::vector<bool> nms;
    bool discard;
    for(size_t i = 0; i < boxes.size(); ++i)
    {
        discard = false;
        for(size_t j = 0; j < boxes.size(); ++j)
        {
            if(IoU(boxes[i], boxes[j]) > threshold)
            {
                //if(score)
            }
        }  
    }
}
*/


int main()
{
    // Load network 
    std::string networkWeights = "/home/f_pietrobon/thesis/MRZ_Antitampering/models/Frozen_graph_test_lr_5_4_ep_3_nms.pb";
    cv::dnn::Net network = cv::dnn::readNetFromTensorflow(networkWeights);
    
    //cv::Mat document = cv::imread("/home/f_pietrobon/thesis/MRZ_Antitampering/AFG_AO_01001_FRONT.JPG", cv::IMREAD_COLOR);
    cv::Mat document = cv::imread("/home/f_pietrobon/thesis/MRZ_Antitampering/BGR_AO_02001_FRONT.jpeg", cv::IMREAD_COLOR);
   
    cv::fastNlMeansDenoising(document, document, 10);
    cv::resize(document, document, cv::Size(800, 800), 0, 0, cv::INTER_CUBIC);
    
    // network prediction
    cv::Mat blob = cv::dnn::blobFromImage(document, 1.0, cv::Size(800, 800), cv::Scalar(0,0,0), true, false);
    network.setInput(blob);
    cv::Mat prediction = network.forward();
    
    std::cout << "Predictions:\t";
    std::cout << prediction.size[0] << " x " << prediction.size[1] << " x " << prediction.size[2] << " x " << prediction.size[3] << std::endl;
    
    cv::Range boxRange(0, 4);
    cv::Range classRange(4, NUM_CLASSES + 5);
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
    //printPredCVMat(boxPred);
    std::cout << "New box pred:\t";
    std::cout << boxPred.size() << " x " << boxPred.begin()->size() << std::endl;

    matrix2D classPred = extractPredCVMat(classPrediction);
    //printPredCVMat(classPred);
    std::cout << "New class pred:\t";
    std::cout << classPred.size() << " x " << classPred.begin()->size() << std::endl;
    
    applySigmoid(classPred);
    //printPredCVMat(classPred);
    std::cout << "Sig class pred:\t";
    std::cout << classPred.size() << " x " << classPred.begin()->size() << std::endl;

    Anchors anchors;
    matrix2D anchorBoxes = anchors.anchorsGenerator();
    std::cout << "All anchors: \t";
    std::cout << anchorBoxes.size() << " x " << anchorBoxes[0].size() << std::endl;

    matrix2D centers = computeCenters(boxPred, anchorBoxes);
    std::cout << "Centers: \t";
    std::cout << centers.size() << " x " << centers[0].size() << std::endl;
    //printPredCVMat(centers);

    matrix2D corners = computeCorners(centers);
    std::cout << "Corners: \t";
    std::cout << corners.size() << " x " << corners[0].size() << std::endl;
    //printPredCVMat(corners);

    extractImageWithPrediction(document, corners, classPred, 0.5);


    //std::cout << "Box predicted:" << std::endl;
    //printPredCVMat(boxPred);

    //std::cout << "Class predicted post sigmoid:" << std::endl;
    //printPredCVMat(classPred);

    // OK, SAME AS PYTHON!
    //std::cout << "Anchors:" << std::endl;
    //printPredCVMat(anchorBoxes);

    


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