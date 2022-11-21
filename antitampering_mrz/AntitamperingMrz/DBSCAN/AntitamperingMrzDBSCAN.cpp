#include "AntitamperingMrzDBSCAN.h"
//#include "facecomparator_utils/comparatorUtils.hpp"
#include "exceptions/Exceptions.hpp"

#include "opencv2/highgui/highgui_c.h"

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>


AntitamperingMrzDBSCAN::AntitamperingMrzDBSCAN(const std::string& OCRModelFilename): AntitamperingMrz()
{
    SPDLOG_INFO("Creating AntitamperingMrz Object");
    modelPath = OCRModelFilename;
}

matrix2D AntitamperingMrzDBSCAN::computeCenters(matrix2D boxes, matrix2D anchorBoxes)
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


std::pair<matrix2D, std::vector<float>> AntitamperingMrzDBSCAN::predictFromXML(Document &document, const char* XMLPath)
{
    XMLBoxes xmlBoxes(document, XMLPath);
    xmlBoxes.extractBoxes();
    std::pair<matrix2D, std::vector<float>> result(xmlBoxes.getBoxes(), xmlBoxes.getClasses());
    return result;
}


std::pair<matrix2D, std::vector<float>> AntitamperingMrzDBSCAN::predictFromModel(Document & document, int numClasses, float thresholdIOU, float thresholdNMS)
{
    // Predict
    SPDLOG_INFO("Load network");
    cv::dnn::Net network = cv::dnn::readNetFromTensorflow(modelPath);
    network.setInput(document.getBlob());

    SPDLOG_INFO("Make network prediction");
    cv::Mat prediction = network.forward();

    ModelBoxes boxes(document, prediction, numClasses);

    // Compute anchors
    Anchors anchors(document.getWidth(), document.getHeight());
    matrix2D anchorBoxes = anchors.anchorsGenerator();

    // Compute boxes
    matrix2D centers = computeCenters(boxes.getBoxPred(), anchorBoxes);
    boxes.computeBoxes(centers);
    boxes.computeNMS(thresholdIOU, thresholdNMS);
    boxes.reshapeBoxes();

    std::pair<matrix2D, std::vector<float>> result(boxes.getBoxes(), boxes.getClasses());

    return result;
}


OcrMrzResponseResult AntitamperingMrzDBSCAN::check(const AntitamperingMrzData& data)
{
    SPDLOG_INFO("Check MRZ starts");

    //std::string imagePath = "../data/AFG_AO_01001_FRONT.JPG"; //TD3 eps = 27   IOU = 0.3  NMS = 0.005 P
    //std::string imagePath = "../data/JPN_AO_02003_FRONT.jpg"; //TD3 eps = 27  IOU = 0.06  NMS = 0.005 NP
    //std::string imagePath = "../data/IMG-20220930-WA0002.jpg"; //TD3 eps = 30  IOU = 0.05  NMS = 0.01 NF
    std::string imagePath = data.imageName;

    Document document(imagePath, FEATURE_WIDTH, FEATURE_HEIGHT, DENOISE_PARAM);

    // Choose the metric type (pairs or WER)
    metricsType metric = WER;
    
    // Predict from XML boxes
    //const char* XMLPath = "../data/AFG_AO_01001_FRONT.xml"; //TD3 eps = 27
    //std::pair<matrix2D, std::vector<float>> XMLResult = predictFromXML(document, XMLPath);
    //savePredictionImage(document.getInputImage(), XMLResult.first, XMLResult.second, "../pred_xml.jpg");

    // Predict from model  
    std::pair<matrix2D, std::vector<float>> modelResult = predictFromModel(document, NUM_CLASSES, THRESHOLD_IOU, THRESHOLD_NMS);
    
    // Aggregate boxes using DBSCAN
    DBSCAN dbscan(EPS, MIN_PTS, modelResult);
    dbscan.run();

    saveCentersPredictionImage(document.getInputImage(), dbscan.getPoints(), "../DBSCAN_xml.jpg");

    Fields fields(dbscan.getPoints(), dbscan.getClusterIdx(), CONF_THRESHOLD);
    /*
    if(fields.findMRZ())
    {
        fields.printOrderedFields();

        fields.compareMRZFields(metric);

        fields.printAssociations();
        fields.printDoubtfulFields();

        // Compact output
        OcrMrzResponseResult res = fillResponse(imagePath, fields, CONF_THRESHOLD);
        printResponse(res);
        SPDLOG_INFO("Check MRZ  ends");
        return res;
    }
*/
    OcrMrzResponseResult res;
    SPDLOG_INFO("Check MRZ ends");
    return res;
}