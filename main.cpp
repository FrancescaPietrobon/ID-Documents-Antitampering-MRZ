#include <iostream>
#include <vector>
#include <numeric>
#include <math.h>
#include <chrono>
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

#define NUM_CLASSES 63
#define CONF_THRESHOLD 0.8

// Image preprocessing params:
#define FEATURE_WIDTH 800
#define FEATURE_HEIGHT 800
#define DENOISE_PARAM 10

// NMS params:
#define THRESHOLD_IOU 0.3     //0.06
#define THRESHOLD_NMS 0.005    //0.005

// DBSCAN params:
#define EPS 27 // it depends on the image
#define MIN_PTS 1

typedef std::vector<std::vector<float>> matrix2D;

int main()
{
    // To monitor time taken by the application
    auto start = std::chrono::high_resolution_clock::now();

    std::string imagePath = "../data/AFG_AO_01001_FRONT.JPG"; //TD3 eps = 27   IOU = 0.3  NMS = 0.005 P
    //std::string imagePath = "../data/JPN_AO_02003_FRONT.jpg"; //TD3 eps = 27  IOU = 0.06  NMS = 0.005 NP
    //std::string imagePath = "../data/IMG-20220930-WA0002.jpg"; //TD3 eps = 30  IOU = 0.05  NMS = 0.01 NF
    Document document(imagePath, FEATURE_WIDTH, FEATURE_HEIGHT, DENOISE_PARAM);

    // Choose the metric type (pairs or WER)
    metricsType metric = WER;

    // Predict from XML boxes
    //const char* XMLPath = "../data/AFG_AO_01001_FRONT.xml"; //TD3 eps = 27
    //std::pair<matrix2D, std::vector<float>> XMLResult = predictFromXML(document, XMLPath);
    //savePredictionImage(document.getInputImage(), XMLResult.first, XMLResult.second, "../pred_xml.jpg");

    // Predict from model

    std::string networkPath = "../models/Frozen_graph_lnorm_5e6_156img.pb";
    
    std::pair<matrix2D, std::vector<float>> modelResult = predictFromModel(document, networkPath, NUM_CLASSES, THRESHOLD_IOU, THRESHOLD_NMS);
    savePredictionImage(document.getInputImage(), modelResult.first, modelResult.second, "../pred_model.jpg");

    // Aggregate boxes using DBSCAN
    DBSCAN dbscan(EPS, MIN_PTS, modelResult);
    dbscan.run();

    saveCentersPredictionImage(document.getInputImage(), dbscan.getPoints(), "../DBSCAN.jpg");

    // Find fields based on DBSCAN predictions
    Fields fields(dbscan.getPoints(), dbscan.getClusterIdx(), CONF_THRESHOLD);

    if(fields.findMRZ())
    {
        fields.printOrderedFields();

        fields.compareMRZFields(metric);

        fields.printAssociations();
        fields.printDoubtfulFields();

        // Compact output
        OcrMrzResponseResult res = fillResponse(imagePath, fields, CONF_THRESHOLD);
        printResponse(res);
    }
    
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "\nTime taken by the complete application: " << duration.count() << " milliseconds" << std::endl;

    return 0;
}