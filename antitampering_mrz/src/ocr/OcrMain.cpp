#include "OcrFactory.hpp"

int main()
{
    //std::string imagePath = "/home/f_pietrobon/thesis/MRZ_Antitampering/antitampering_mrz/data/AFG_AO_01001_FRONT.JPG"; //TD3 eps = 27   IOU = 0.3  NMS = 0.005 P
    //std::string imagePath = "/home/f_pietrobon/thesis/MRZ_Antitampering/antitampering_mrz/data/JPN_AO_02003_FRONT.jpg"; //TD3 eps = 27  IOU = 0.06  NMS = 0.005 NP
    //std::string imagePath = "/home/f_pietrobon/thesis/MRZ_Antitampering/antitampering_mrz/data/IMG-20220930-WA0002.jpg"; //TD3 eps = 30  IOU = 0.05  NMS = 0.01 NF
    std::string imagePath = "/home/f_pietrobon/thesis/MRZ_Antitampering/antitampering_mrz/data/DNK_AO_04002_FRONT_2.jpeg";
    //std::string imagePath = "/home/f_pietrobon/thesis/MRZ_Antitampering/antitampering_mrz/data/dimMiste.jpg";

    const float confidenceThreshold = 0.3;
    cv::Mat inputImage = cv::imread(imagePath);

    OcrFactory ocrFactory;
    std::shared_ptr<Ocr> ocr = ocrFactory.createOCR("RetinaNet");
    std::vector<Characters> result = ocr->detect(inputImage, confidenceThreshold);
    return 0;
}