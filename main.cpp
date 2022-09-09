#include <iostream>
#include <vector>
#include <numeric>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "include/Document.h"
#include "include/Boxes.h"
#include "include/DBSCAN.h"
#include "include/Characters.h"
#include "include/Anchors.h"
#include "include/pugixml.hpp"

#define FEATURE_WIDTH 800
#define FEATURE_HEIGHT 800
#define NUM_CLASSES 64
#define THRESHOLD_IOU 0.08
#define THRESHOLD_NMS 0.01
#define DENOISE_PARAM 10

typedef std::vector<std::vector<float>> matrix2D;
typedef std::vector<std::vector<std::vector<float>>> matrix3D;
typedef std::vector<std::vector<std::vector<std::vector<float>>>> matrix4D;

matrix2D extractPredCVMat(cv::Mat boxPrediction)
{
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
            if(i < 20)
                std::cout << boxPrediction[i][j] << " ";
        if(i < 20)
            std::cout << " " << std::endl;
    }
}


void applySigmoid(matrix2D &A)
{
    for(size_t i = 0; i < A.size(); ++i)
        for(size_t j = 0; j < A[0].size(); ++j)
            A[i][j] = 1 / (1 + exp(- A[i][j]));
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


void extractImageWithPrediction(cv::Mat img, matrix2D boxes, matrix2D pred, float threshold, float threshold_nms)
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

    std::vector<int> nmsIndices;
    cv::dnn::dnn4_v20220524::NMSBoxes(boxes_rect, all_max, threshold, threshold_nms, nmsIndices);
    
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


void printXMLBoxes(const char* XMLPath)
{
    pugi::xml_document doc;
    doc.load_file(XMLPath);
    pugi::xml_node tools = doc.child("annotation");

    for (pugi::xml_node object: tools.children("object"))
    {
        std::cout << "Object " << object.child_value("name");
        pugi::xml_node box = object.child("bndbox");
        std::cout << " : xmin " << box.child_value("xmin");
        std::cout << ", ymin " << box.child_value("ymin");
        std::cout << ", xmax " << box.child_value("xmax");
        std::cout << ", ymax " << box.child_value("ymax") << std::endl;
    }
}


std::pair<matrix2D, std::vector<float>> extractBoxes(const char* XMLPath)
{
    matrix2D boxes;
    std::vector<float> single_box;
    std::vector<float> classes;
    float xmin, ymin, xmax, ymax;
    std::string label;
    unsigned label_idx;

    pugi::xml_document doc;
    doc.load_file(XMLPath);

    pugi::xml_node tools = doc.child("annotation");
    for (pugi::xml_node object: tools.children("object"))
    {
        label = std::string(object.child_value("name"));
        label_idx = inverse_dictionary[label];
        classes.push_back(label_idx);

        pugi::xml_node box = object.child("bndbox");
        xmin = std::atof(box.child_value("xmin"));
        ymin = std::atof(box.child_value("ymin"));
        xmax = std::atof(box.child_value("xmax"));
        ymax = std::atof(box.child_value("ymax"));
        single_box = {xmin, ymin, xmax, ymax};
        boxes.push_back(single_box);
    }

    std::pair<matrix2D, std::vector<float>> result(boxes, classes);

    return result;

}


void NEWpredictFromModel(std::string networkPath, std::string imagePath)
{
    cv::dnn::Net network = cv::dnn::readNetFromTensorflow(networkPath);

    Document document(imagePath);
    document.preprocessing();
    
    // Predict
    network.setInput(document.GetBlob());
    cv::Mat prediction = network.forward();
    
    // Split box and classes
    cv::Range boxRange(0, 4);
    cv::Range classRange(4, NUM_CLASSES + 5);
    cv::Range all(cv::Range::all());
    std::vector<cv::Range> boxRanges = {all, boxRange, all, all};
    std::vector<cv::Range> classRanges = {all, classRange, all, all};
    cv::Mat boxPrediction = prediction(boxRanges);
    cv::Mat classPrediction = prediction(classRanges);

    Boxes boxesPred(boxPrediction);

    // Save prediction for box and classes in the right format
    // matrix2D boxPred = extractPredCVMat(boxesPred.getPred());
    // boxesPred.setPredReshaped(boxPred);

    matrix2D classPred = extractPredCVMat(classPrediction);
    
    applySigmoid(classPred);

    // Compute anchors
    Anchors anchors;
    matrix2D anchorBoxes = anchors.anchorsGenerator();

    // Compute the right boxes
    matrix2D centers = computeCenters(boxesPred.getPredReshaped(), anchorBoxes);
    boxesPred.computeCorners(centers);
    //printPredCVMat(corners);

    // Save the image with the predicted boxes
    extractImageWithPrediction(document.GetDocument(), boxesPred.getCorners(), classPred, THRESHOLD_IOU, THRESHOLD_NMS); // FORSE DEVE ESSERE BLOB NON DOCUMENT
}


void predictFromModel(std::string networkPath, std::string imagePath)
{
    cv::dnn::Net network = cv::dnn::readNetFromTensorflow(networkPath);
    cv::Mat document = cv::imread(imagePath, cv::IMREAD_COLOR);
   
    // Preprocessing
    cv::fastNlMeansDenoising(document, document, DENOISE_PARAM);
    cv::resize(document, document, cv::Size(FEATURE_WIDTH, FEATURE_HEIGHT), 0, 0, cv::INTER_CUBIC);
    
    // Predict
    cv::Mat blob = cv::dnn::blobFromImage(document, 1.0, cv::Size(FEATURE_WIDTH, FEATURE_HEIGHT), cv::Scalar(103.939, 116.779, 123.68), true, false);
    network.setInput(blob);
    cv::Mat prediction = network.forward();
    
    // Split box and classes
    cv::Range boxRange(0, 4);
    cv::Range classRange(4, NUM_CLASSES + 5);
    cv::Range all(cv::Range::all());
    std::vector<cv::Range> boxRanges = {all, boxRange, all, all};
    std::vector<cv::Range> classRanges = {all, classRange, all, all};
    cv::Mat boxPrediction = prediction(boxRanges);
    cv::Mat classPrediction = prediction(classRanges);

    // Save prediction for box and classes in the right format
    matrix2D boxPred = extractPredCVMat(boxPrediction);
    matrix2D classPred = extractPredCVMat(classPrediction);
    
    applySigmoid(classPred);

    // Compute anchors
    Anchors anchors;
    matrix2D anchorBoxes = anchors.anchorsGenerator();

    // Compute the right boxes
    matrix2D centers = computeCenters(boxPred, anchorBoxes);
    matrix2D corners = computeCorners(centers);
    //printPredCVMat(corners);

    // Save the image with the predicted boxes
    extractImageWithPrediction(document, corners, classPred, THRESHOLD_IOU, THRESHOLD_NMS); // FORSE DEVE ESSERE BLOB NON DOCUMENT
}



void predictFromXML(const char* XMLPath, std::string imagePath)
{
    cv::Mat document_init = cv::imread(imagePath, cv::IMREAD_COLOR);
    cv::Mat document;

    // Preprocessing
    cv::fastNlMeansDenoising(document_init, document, DENOISE_PARAM);
    cv::resize(document, document, cv::Size(FEATURE_WIDTH, FEATURE_HEIGHT), 0, 0, cv::INTER_CUBIC);
    
    // Predict
    cv::Mat blob = cv::dnn::blobFromImage(document, 1.0, cv::Size(FEATURE_WIDTH, FEATURE_HEIGHT), cv::Scalar(103.939, 116.779, 123.68), true, false);

    std::pair<matrix2D, std::vector<float>> result = extractBoxes(XMLPath);



    savePredictionImage(document, result.first, result.second, "../pred_xml.jpg");
}





int main()
{
    std::string networkPath = "/home/f_pietrobon/thesis/MRZ_Antitampering/models/Frozen_graph_prova.pb";
    std::string imagePath = "/home/f_pietrobon/thesis/MRZ_Antitampering/data/BGR_AO_02001_FRONT.jpeg";
    
    const char* XMLPath = "/home/f_pietrobon/thesis/MRZ_Antitampering/data/BGR_AO_02001_FRONT.xml";

    printXMLBoxes(XMLPath);

    //predictFromXML(XMLPath, imagePath);

    predictFromModel(networkPath, imagePath);
    
    return 0;
}