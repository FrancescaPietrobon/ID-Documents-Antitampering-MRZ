#include <iostream>
#include <vector>
#include <numeric>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "include/Document.h"
#include "include/Boxes.h"
#include "include/XMLBoxes.h"
#include "include/DBSCAN.h"
#include "include/Anchors.h"
#include "include/utilities.h"
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


void predictFromModel(Document document, std::string networkPath)
{
    cv::dnn::Net network = cv::dnn::readNetFromTensorflow(networkPath);
    
    // Predict
    network.setInput(document.getBlob());
    cv::Mat prediction = network.forward();
    
    // Split box and classes
    cv::Range boxRange(0, 4);
    cv::Range classRange(4, NUM_CLASSES + 5);
    cv::Range all(cv::Range::all());
    std::vector<cv::Range> boxRanges = {all, boxRange, all, all};
    std::vector<cv::Range> classRanges = {all, classRange, all, all};
    cv::Mat boxPrediction = prediction(boxRanges);
    cv::Mat classPrediction = prediction(classRanges);

    Boxes boxesPred(document, boxPrediction);

    // Save prediction for box and classes in the right format
    // matrix2D boxPred = extractPredCVMat(boxesPred.getPred());
    // boxesPred.setpredMatrix(boxPred);

    matrix2D classPred = extractPredCVMat(classPrediction);
    
    applySigmoid(classPred);

    // Compute anchors
    Anchors anchors;
    matrix2D anchorBoxes = anchors.anchorsGenerator();

    // Compute the right boxes
    matrix2D centers = computeCenters(boxesPred.getPredMatrix(), anchorBoxes);
    boxesPred.computeCorners(centers);
    //printPredCVMat(corners);

    // Save the image with the predicted boxes
    extractImageWithPrediction(document.getDocument(), boxesPred.getCorners(), classPred, THRESHOLD_IOU, THRESHOLD_NMS); // FORSE DEVE ESSERE BLOB NON DOCUMENT
}



int main()
{
    std::string networkPath = "/home/f_pietrobon/thesis/MRZ_Antitampering/models/Frozen_graph_prova.pb";
    std::string imagePath = "/home/f_pietrobon/thesis/MRZ_Antitampering/data/BGR_AO_02001_FRONT.jpeg";
    
    const char* XMLPath = "/home/f_pietrobon/thesis/MRZ_Antitampering/data/BGR_AO_02001_FRONT.xml";

    Document document(imagePath);

    XMLBoxes xmlBoxes(document, XMLPath);

    xmlBoxes.predictFromXML();
    //xmlBoxes.printXMLBoxes();

    //predictFromModel(document, networkPath);
    
    return 0;
}