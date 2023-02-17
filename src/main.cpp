#include "ocr/OcrApi.hpp"
#include "base64/base64.h"

//#include "characters_associator/CharactersAssociator.hpp"

std::string getBase64(cv::Mat image)
{
    std::vector<uchar> buffer;
    cv::imencode(".png", image, buffer);
    std::string imageBase64 = base64_encode(buffer.data(), buffer.size());
    return imageBase64;
}

int main(int argc, char *argv[])
{
    /*
    //std::string imagePath = "/home/f_pietrobon/thesis/MRZ_Antitampering/antitampering_mrz/data/AFG_AO_01001_FRONT.JPG"; //TD3 eps = 27   IOU = 0.3  NMS = 0.005 P
    //std::string imagePath = "/home/f_pietrobon/thesis/MRZ_Antitampering/antitampering_mrz/data/JPN_AO_02003_FRONT.jpg"; //TD3 eps = 27  IOU = 0.06  NMS = 0.005 NP
    //std::string imagePath = "/home/f_pietrobon/thesis/MRZ_Antitampering/antitampering_mrz/data/IMG-20220930-WA0002.jpg"; //TD3 eps = 30  IOU = 0.05  NMS = 0.01 NF
    std::string imagePath = "/home/f_pietrobon/thesis/MRZ_Antitampering/data/DNK_AO_04002_FRONT_2.jpeg";
    //std::string imagePath = "/home/f_pietrobon/thesis/MRZ_Antitampering/antitampering_mrz/data/dimMiste.jpg";

    const float confidenceThreshold = 0.3;
    cv::Mat inputImage = cv::imread(imagePath);

    OcrFactory ocrFactory;
    std::shared_ptr<Ocr> ocr = ocrFactory.createOCR("RetinaNet");
    std::vector<Characters> result = ocr->detect(inputImage, confidenceThreshold);
    */

    std::string imagePath = "/home/f_pietrobon/thesis/MRZ_Antitampering/data/DNK_AO_04002_FRONT_2.jpeg";

    char **images = new char *[1];
    images[0] = utils::convertStringtoCharPtr("images0");
    char **contentType = new char *[1];
    contentType[0] = utils::convertStringtoCharPtr("image/jpeg");
    char **contentBase64 = new char *[1];
    contentBase64[0] = utils::convertStringtoCharPtr(getBase64(cv::imread(imagePath)));
    Coordinates *coordinates = new Coordinates[1];
    coordinates[0] = Coordinates{0, 0, 800, 800};
    float *thresholds = new float[1];
    thresholds[0] = 0.3;
    char *algoType = new char;
    algoType = utils::convertStringtoCharPtr("RetinaNet");

    OcrResponse result = process(images, contentType, contentBase64, coordinates, thresholds, 1, algoType);

    return 0;
}