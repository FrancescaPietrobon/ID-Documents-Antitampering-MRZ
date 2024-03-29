#include "ocr_server/ocr/OcrApi.hpp"
#include "ocr_server/charactersClustering/CharactersClusteringApi.hpp"
#include "antitampering/mrz/AntitamperingMrzApi.hpp"
#include "base64/base64.h"
#include <spdlog/spdlog.h>

#include "print/PrintResult.hpp"

std::string getBase64(cv::Mat image)
{
    std::vector<uchar> buffer;
    cv::imencode(".png", image, buffer);
    std::string imageBase64 = base64_encode(buffer.data(), buffer.size());
    return imageBase64;
}

int main(int argc, char *argv[])
{
    std::string imagePath = "testData/images/ocr/AFG_AO_01001_FRONT.JPG";

    char **images = new char *[1];
    images[0] = utils::convertStringtoCharPtr("images0");
    char **contentBase64 = new char *[1];
    contentBase64[0] = utils::convertStringtoCharPtr(getBase64(cv::imread(imagePath)));
    Coordinates *coordinates = new Coordinates[1];
    coordinates[0] = Coordinates{0, 0, 2000, 2000};
    float *thresholdsOcr = new float[1];
    thresholdsOcr[0] = 0.25;
    char *algoTypeOcr = new char;
    algoTypeOcr = utils::convertStringtoCharPtr("RetinaNet");
    // algoTypeOcr = utils::convertStringtoCharPtr("RetinaNetBinary");
    // algoTypeOcr = utils::convertStringtoCharPtr("YoloV8");
    // algoTypeOcr = utils::convertStringtoCharPtr("YoloV8Binary");

    OcrResponse ocrResponse = process(images, contentBase64, coordinates, thresholdsOcr, 1, algoTypeOcr);
    delete[] contentBase64;
    delete[] coordinates;
    delete[] thresholdsOcr;
    delete algoTypeOcr;

    saveImgOcrResponse(cv::imread(imagePath), ocrResponse.resultDetails->character, ocrResponse.resultDetails->charactersSize);

    char *algoTypeClustering = new char;
    algoTypeClustering = utils::convertStringtoCharPtr("Dbscan");

    ClusteringResponse clusteringResponse = cluster(ocrResponse, algoTypeClustering);
    delete algoTypeClustering;

    printDbscanResponse(clusteringResponse);
    saveImgDbscanResponse(cv::imread(imagePath), clusteringResponse.resultDetails->field, clusteringResponse.resultDetails->fieldsSize);

    DocumentFields *documentFields = new DocumentFields[1];
    documentFields[0].fields = clusteringResponse.resultDetails->field;
    documentFields[0].fieldsSize = clusteringResponse.resultDetails->fieldsSize;

    char *algoTypeAntitamperingMrz = new char;
    algoTypeAntitamperingMrz = utils::convertStringtoCharPtr("levenshtein");

    AntitamperingMrzResponse antitamperingMrzResponse = associate(images, documentFields, 1, algoTypeAntitamperingMrz);
    delete algoTypeAntitamperingMrz;

    printAntitamperingMrzResponse(antitamperingMrzResponse);
    
    return 0;
}