#include <iostream>
#include <vector>
#include <numeric>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "include/pugixml.hpp"
#include "include/utilities.h"
#include "include/ModelBoxes.h"
#include "include/XMLBoxes.h"
#include "include/DBSCAN.h"
#include "include/Anchors.h"
#include "src/Characters.cpp"

#define FEATURE_WIDTH 800
#define FEATURE_HEIGHT 800
#define NUM_CLASSES 64
#define THRESHOLD_IOU 0.08
#define THRESHOLD_NMS 0.01
#define DENOISE_PARAM 10

typedef std::vector<std::vector<float>> matrix2D;
typedef std::vector<std::vector<std::vector<float>>> matrix3D;
typedef std::vector<std::vector<std::vector<std::vector<float>>>> matrix4D;


std::pair<matrix2D, std::vector<float>> predictFromModel(std::string imagePath, std::string networkPath)
{
    Document document(imagePath);

    // Predict
    cv::dnn::Net network = cv::dnn::readNetFromTensorflow(networkPath);
    network.setInput(document.getBlob());
    cv::Mat prediction = network.forward();

    ModelBoxes boxes(document, prediction);

    // Compute anchors
    Anchors anchors;
    matrix2D anchorBoxes = anchors.anchorsGenerator();

    // Compute the right boxes
    matrix2D centers = computeCenters(boxes.getBoxPred(), anchorBoxes);
    boxes.computeBoxes(centers);
    boxes.computeNMS(THRESHOLD_IOU, THRESHOLD_NMS);

    boxes.reshapeBoxes();
    savePredictionImage(document.getInputImage(), boxes.getBoxes(), boxes.getClasses(), "../pred_model.jpg");

    std::pair<matrix2D, std::vector<float>> result(boxes.getBoxes(), boxes.getClasses());

    return result;
}


std::pair<matrix2D, std::vector<float>> predictFromXML(std::string imagePath, const char* XMLPath)
{
    Document document(imagePath);

    XMLBoxes xmlBoxes(document, XMLPath);

    xmlBoxes.extractBoxes();
    savePredictionImage(document.getInputImage(), xmlBoxes.getBoxes(), xmlBoxes.getClasses(), "../pred_xml.jpg");

    std::pair<matrix2D, std::vector<float>> result(xmlBoxes.getBoxes(), xmlBoxes.getClasses());

    return result;
}


int main()
{
    std::string networkPath = "/home/f_pietrobon/thesis/MRZ_Antitampering/models/Frozen_graph_prova.pb";
    std::string imagePath = "/home/f_pietrobon/thesis/MRZ_Antitampering/data/BGR_AO_02001_FRONT.jpeg";
    
    const char* XMLPath = "/home/f_pietrobon/thesis/MRZ_Antitampering/data/BGR_AO_02001_FRONT.xml";
    
    std::pair<matrix2D, std::vector<float>> XMLResult = predictFromXML(imagePath, XMLPath);

    std::pair<matrix2D, std::vector<float>> modelResult = predictFromModel(imagePath, networkPath);
    
    return 0;
}