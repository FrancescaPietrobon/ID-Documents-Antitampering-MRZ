#include "ocr/OcrApi.hpp"
#include "charactersClustering/CharactersClusteringApi.hpp"
#include "antitampering/mrz/AntitamperingMrzApi.hpp"
#include "base64/base64.h"

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
    //std::string imagePath = "/home/f_pietrobon/thesis/MRZ_Antitampering/data/JPN_AO_02003_FRONT.jpg"; //TD3 eps = 27  IOU = 0.06  NMS = 0.005 NP
    //std::string imagePath = "/home/f_pietrobon/thesis/MRZ_Antitampering/data/DNK_AO_04002_FRONT_2.jpeg";
    //std::string imagePath = "/home/f_pietrobon/thesis/MRZ_Antitampering/data/IMG-20220930-WA0002.jpg"; //TD3 eps = 30  IOU = 0.05  NMS = 0.01 NF
    std::string imagePath = "testData/images/ocr/AFG_AO_01001_FRONT.JPG";

    char **images = new char *[1];
    images[0] = utils::convertStringtoCharPtr("images0");
    char **contentType = new char *[1];
    contentType[0] = utils::convertStringtoCharPtr("image/jpeg");
    char **contentBase64 = new char *[1];
    contentBase64[0] = utils::convertStringtoCharPtr(getBase64(cv::imread(imagePath)));
    Coordinates *coordinates = new Coordinates[1];
    coordinates[0] = Coordinates{0, 0, 2000, 2000};
    float *thresholds = new float[1];
    thresholds[0] = 0.3;
    char *algoTypeOcr = new char;
    algoTypeOcr = utils::convertStringtoCharPtr("RetinaNet");

    OcrResponse ocrResult = process(images, contentType, contentBase64, coordinates, thresholds, 1, algoTypeOcr);

    printOCRResult(cv::imread(imagePath), ocrResult.resultDetails->characters, ocrResult.resultDetails->charactersSize);

    char *algoTypeClustering = new char;
    //algoTypeClustering = utils::convertStringtoCharPtr("DbscanPoints");
    algoTypeClustering = utils::convertStringtoCharPtr("DbscanBoxes");

    ClusteringResponse clusteringResult = cluster(ocrResult, algoTypeClustering);

    std::cout << "\nClustering result with DBSCAN:\n" << std::endl;
    for(size_t i = 0; i < clusteringResult.resultDetailsSize; ++i)
    {
        std::cout << "Image: " << clusteringResult.resultDetails[i].image << "\n" << std::endl;
        for(size_t j = 0; j < clusteringResult.resultDetails[i].fieldsSize; ++j)
        {
            std::cout << "Label: " << clusteringResult.resultDetails[i].fields[j].label << std::endl;
            std::cout << "Confidence: " << clusteringResult.resultDetails[i].fields[j].confidence << "\n" << std::endl;
        }
    }

    printDbscanResult(cv::imread(imagePath), clusteringResult.resultDetails->fields, clusteringResult.resultDetails->fieldsSize);
    
    char *algoTypeAntitamperingMrz = new char;
    algoTypeAntitamperingMrz = utils::convertStringtoCharPtr("wer");

    AntitamperingMrzResponse antitamperingMrzResponse = associate(clusteringResult, algoTypeAntitamperingMrz);

    return 0;
}