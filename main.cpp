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
#include "include/Fields.h"

#define NUM_CLASSES 64

// Image preprocessing params:
#define FEATURE_WIDTH 800
#define FEATURE_HEIGHT 800
#define DENOISE_PARAM 10

// NMS params:
#define THRESHOLD_IOU 0.08
#define THRESHOLD_NMS 0.01

// DBSCAN params:
#define EPS 15 //top 15 o 11
#define MIN_PTS 1

typedef std::vector<std::vector<float>> matrix2D;
typedef std::vector<std::vector<std::vector<float>>> matrix3D;
typedef std::vector<std::vector<std::vector<std::vector<float>>>> matrix4D;


void printAggregatedPoints(Document document, myDBSCAN dbscan)
{
    Fields fields(dbscan.getPoints(), dbscan.getClusterIdx());

}



std::vector<myPoint> computePoints(std::pair<matrix2D, std::vector<float>> boxes_labels)
{
    matrix2D boxes = boxes_labels.first;
    std::vector<float> labels = boxes_labels.second;

    std::vector<myPoint> points;
    float h, w, c_x, c_y;
    for(size_t i = 0; i < boxes.size(); ++i)
    {
        w = boxes[i][2] - boxes[i][0];
        h = boxes[i][3] - boxes[i][1];

        c_x = boxes[i][0] + w / 2;
        c_y = boxes[i][1] + h / 2;

        myPoint point(c_x, c_y, w, h, labels[i], NOT_CLASSIFIED);
        points.push_back(point);
    }

    return points;
}


std::pair<matrix2D, std::vector<float>> predictFromModel(Document document, std::string networkPath)
{
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
    
    std::pair<matrix2D, std::vector<float>> result(boxes.getBoxes(), boxes.getClasses());

    return result;
}


std::pair<matrix2D, std::vector<float>> predictFromXML(Document document, const char* XMLPath)
{
    XMLBoxes xmlBoxes(document, XMLPath);

    xmlBoxes.extractBoxes();

    std::pair<matrix2D, std::vector<float>> result(xmlBoxes.getBoxes(), xmlBoxes.getClasses());

    return result;
}


int main()
{
    //std::string imagePath = "/home/f_pietrobon/thesis/MRZ_Antitampering/data/BGR_AO_02001_FRONT.jpeg";
    std::string imagePath = "/home/f_pietrobon/thesis/MRZ_Antitampering/data/AFG_AO_01001_FRONT.JPG";

    Document document(imagePath);
    
    // Predict from model
    //std::string networkPath = "/home/f_pietrobon/thesis/MRZ_Antitampering/models/Frozen_graph_prova.pb";
    //std::pair<matrix2D, std::vector<float>> modelResult = predictFromModel(document, networkPath);
    //savePredictionImage(document.getInputImage(), modelResult.first, modelResult.second, "../pred_model.jpg");

    // Predict from XML boxes
    //const char* XMLPath = "/home/f_pietrobon/thesis/MRZ_Antitampering/data/BGR_AO_02001_FRONT.xml";
    const char* XMLPath = "/home/f_pietrobon/thesis/MRZ_Antitampering/data/AFG_AO_01001_FRONT.xml";
    std::pair<matrix2D, std::vector<float>> XMLResult = predictFromXML(document, XMLPath);
    //savePredictionImage(document.getInputImage(), XMLResult.first, XMLResult.second, "../pred_xml.jpg");
    
    std::vector<myPoint> points = computePoints(XMLResult);
    //saveCentersPredictionImage(document.getInputImage(), points, "../centers_xml.jpg");

    // Aggregate boxes using DBSCAN
    myDBSCAN dbScan(EPS, MIN_PTS, points);
    dbScan.run();

    //std::vector<myPoint> newPoints = dbScan.getPoints();
    saveCentersPredictionImage(document.getInputImage(), dbScan.getPoints(), "../DBSCAN_xml.jpg");
    printAggregatedPoints(document, dbScan);

    return 0;
}