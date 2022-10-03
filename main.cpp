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
#include "include/Dictionary.h"
#include "include/Date.h"
#include "include/Fields.h"
#include "include/Character.h"
#include "include/Response.h"
#include "include/Metrics.h"

#define NUM_CLASSES 64
#define CONF_THRESHOLD 0.75

// Image preprocessing params:
#define FEATURE_WIDTH 800
#define FEATURE_HEIGHT 800
#define DENOISE_PARAM 10

// NMS params:
#define THRESHOLD_IOU 0.08
#define THRESHOLD_NMS 0.01

// DBSCAN params:
#define EPS 30 // it depends on the image
#define MIN_PTS 1

// Meshgrid
#define RX_Y_PLUS 0.5


typedef std::vector<std::vector<float>> matrix2D;

// Predictions form Model and from given XML
std::pair<matrix2D, std::vector<float>> predictFromModel(Document document, std::string networkPath)
{
    // Predict
    cv::dnn::Net network = cv::dnn::readNetFromTensorflow(networkPath);
    network.setInput(document.getBlob());
    cv::Mat prediction = network.forward();

    ModelBoxes boxes(document, prediction, NUM_CLASSES);

    // Compute anchors
    Anchors anchors(FEATURE_WIDTH, FEATURE_HEIGHT, RX_Y_PLUS);
    matrix2D anchorBoxes = anchors.anchorsGenerator();

    // Compute the right boxes
    matrix2D centers = computeCenters(boxes.getBoxPred(), anchorBoxes);
    boxes.computeBoxes(centers);
    boxes.computeNMS(THRESHOLD_IOU, THRESHOLD_NMS);

    boxes.reshapeBoxes();
    
    std::pair<matrix2D, std::vector<float>> result(boxes.getBoxes(), boxes.getClasses());

    return result;
}

std::pair<matrix2D, std::vector<float>> predictFromXML(Document &document, const char* XMLPath)
{
    XMLBoxes xmlBoxes(document, XMLPath);
    xmlBoxes.extractBoxes();
    std::pair<matrix2D, std::vector<float>> result(xmlBoxes.getBoxes(), xmlBoxes.getClasses());
    return result;
}



int main()
{
    //std::string imagePath = "../data/BGR_AO_02001_FRONT.jpeg";
    //std::string imagePath = "../data/AFG_AO_01001_FRONT.JPG"; //TD3 eps = 27
    //std::string imagePath = "../data/ZWE_AO_01002_FRONT.JPG"; //MRVA eps = 20
    //std::string imagePath = "../data/AFG_AO_01001_FRONT_3.JPG"; //TD3 eps = 11
    std::string imagePath = "../data/IMG-20220930-WA0002.jpg"; //TD3 eps = 30
    Document document(imagePath, FEATURE_WIDTH, FEATURE_HEIGHT, DENOISE_PARAM);
    
    // Predict from model
    //std::string networkPath = "../models/Frozen_graph_prova.pb";
    //std::pair<matrix2D, std::vector<float>> modelResult = predictFromModel(document, networkPath);
    //savePredictionImage(document.getInputImage(), modelResult.first, modelResult.second, "../pred_model.jpg");

    // Predict from XML boxes
    //const char* XMLPath = "../data/BGR_AO_02001_FRONT.xml";
    //const char* XMLPath = "../data/AFG_AO_01001_FRONT.xml"; //TD3 eps = 27
    //const char* XMLPath = "../data/ZWE_AO_01002_FRONT.xml"; //MRVA eps = 20
    //const char* XMLPath = "../data/AFG_AO_01001_FRONT_3.xml"; //TD3 eps = 11
    const char* XMLPath = "../data/IMG-20220930-WA0002.xml"; //TD3  eps = 30

    // Choose the metric type
    //metricsType metric = pairs;
    metricsType metric = distLev;
    
    std::pair<matrix2D, std::vector<float>> XMLResult = predictFromXML(document, XMLPath);
    //savePredictionImage(document.getInputImage(), XMLResult.first, XMLResult.second, "../pred_xml.jpg");
    
    // Aggregate boxes using DBSCAN
    myDBSCAN dbscan(EPS, MIN_PTS, XMLResult);
    dbscan.run();

    saveCentersPredictionImage(document.getInputImage(), dbscan.getPoints(), "../DBSCAN_xml.jpg");

    // Find fields based on DBSCAN predictions
    Fields fields(dbscan.getPoints(), dbscan.getClusterIdx());

    fields.checkMRZ();
    std::cout << std::endl;

    fields.printOrderedFields();
    std::cout << std::endl;

    fields.compareMRZFields(metric);
    std::cout << std::endl;

    fields.printNotFilledAndFilledFields();
    std::cout << std::endl;

    fields.printAssociations();


    // Useful outputs

    std::cout << "\n\nFINAL OUTPUT" <<std::endl;

    std::cout << "\nImage name: " << imagePath << std::endl;

    std::cout << "\nResult: " << std::boolalpha << fields.getResult() << std::endl;
    std::cout << "\nConfidence threshold: " << CONF_THRESHOLD << std::endl;
    //std::cout << "\nConfidence: " << finalConf << std::endl;
    std::cout << "\nConfidence: " << fields.getFinalConf() << std::endl;
    fields.printDoubtfulFields();
   
    std::cout << "\nNumber of doubtful fields: " << fields.getNumDoubtfulFields() << std::endl;


    // Compact output
    //OcrMrzResponseResult res = fillResponse(imagePath, fields, CONF_THRESHOLD);

    return 0;
}