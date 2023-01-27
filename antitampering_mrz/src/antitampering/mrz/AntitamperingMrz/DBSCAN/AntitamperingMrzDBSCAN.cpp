#include "AntitamperingMrzDBSCAN.h"
#include "src/antitampering/mrz/exceptions/Exceptions.hpp"

#include "opencv2/highgui/highgui_c.h"

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>


AntitamperingMrzDBSCAN::AntitamperingMrzDBSCAN(const std::string& OCRModelFilename): AntitamperingMrz()
{
    SPDLOG_INFO("Creating AntitamperingMrz Object");
    modelPath = OCRModelFilename;
}


std::pair<matrix2D, std::vector<float>> AntitamperingMrzDBSCAN::predictFromXML(Document & document, const char* XMLPath)
{
    XMLBoxes xmlBoxes(document, XMLPath);
    xmlBoxes.extractBoxes();
    std::pair<matrix2D, std::vector<float>> result(xmlBoxes.getBoxes(), xmlBoxes.getClasses());
    return result;
}


std::pair<matrix2D, std::vector<float>> AntitamperingMrzDBSCAN::predictFromModel(Document & document, int numClasses, float thresholdIOU, float thresholdNMS)
{
    SPDLOG_INFO("Load network");
    cv::dnn::Net network = cv::dnn::readNetFromTensorflow(modelPath);
    cv::Mat blob = cv::dnn::blobFromImage(document.getDocument(), 1.0, cv::Size(document.getWidth(), document.getHeight()), cv::Scalar(103.939, 116.779, 123.68), true, false);
    network.setInput(blob);

    SPDLOG_INFO("Make model's predictions");
    cv::Mat prediction = network.forward();

    SPDLOG_INFO("Construct anchors");
    Anchors *anchors = new Anchors(document.getWidth(), document.getHeight());
    ModelBoxes modelBoxes(anchors, document, prediction, numClasses, thresholdIOU, thresholdNMS);

    SPDLOG_INFO("Make correct predictions");
    std::pair<matrix2D, std::vector<float>> result = modelBoxes.predict();

    return result;
}


OcrMrzResultDetail AntitamperingMrzDBSCAN::check(const AntitamperingMrzData& data)
{
    SPDLOG_INFO("Check MRZ starts");

    std::string imagePath = data.imageName;

    Document document(imagePath, FEATURE_WIDTH, FEATURE_HEIGHT, DENOISE_PARAM);
    cv::Mat preprocessedImage = document.preprocessing();

    // Choose the metric type (pairs or WER)
    metricsType metric = WER;
    
    // Predict from XML boxes
    //const char* XMLPath = "../data/AFG_AO_01001_FRONT.xml"; //TD3 eps = 27
    //std::pair<matrix2D, std::vector<float>> XMLResult = predictFromXML(document, XMLPath);
    //savePredictionImage(document.getInputImage(), XMLResult.first, XMLResult.second, "../pred_xml.jpg");

    // Predict from model  
    std::pair<matrix2D, std::vector<float>> modelResult = predictFromModel(document, NUM_CLASSES, THRESHOLD_IOU, THRESHOLD_NMS);

    // Aggregate boxes using DBSCAN
    SPDLOG_INFO("Run DBSCAN");
    DBSCAN cluster(EPS, modelResult);
    std::vector<Field> fields = cluster.extractFields();

    SPDLOG_INFO("Compute fields");
    cluster.printOrderedFields(fields);

    SPDLOG_INFO("Save image with DBSCAN's predictions");
    saveCentersPredictionImage(document.getInputImage(), cluster.getPoints(), "../cluster_xml.jpg");    

    //Fields fields(cluster.getPoints(), cluster.getClusterIdx(), CONF_THRESHOLD);
    /*
    if(fields.findMRZ())
    {
        fields.printOrderedFields();

        fields.compareMRZFields(metric);

        fields.printAssociations();
        fields.printDoubtfulFields();

        // Compact output
        OcrMrzResultDetail res = fillResponse(imagePath, fields, CONF_THRESHOLD);
        printResponse(res);
        SPDLOG_INFO("Check MRZ  ends");
        return res;
    }
    */
    OcrMrzResultDetail res;
    SPDLOG_INFO("Check MRZ ends");
    return res;
}