/*
#include "antitamperingMrzFactory.hpp"
#include "antitamperingMrzApi.hpp"

int main()
{

    std::string networkPath = "/home/f_pietrobon/thesis/MRZ_Antitampering/antitampering_mrz/models/Frozen_graph_lnorm_5e6_156img.pb";
    std::string imagePath = "/home/f_pietrobon/thesis/MRZ_Antitampering/antitampering_mrz/data/AFG_AO_01001_FRONT.JPG"; //TD3 eps = 27   IOU = 0.3  NMS = 0.005 P
    //std::string imagePath = "../data/JPN_AO_02003_FRONT.jpg"; //TD3 eps = 27  IOU = 0.06  NMS = 0.005 NP
    //std::string imagePath = "../data/IMG-20220930-WA0002.jpg"; //TD3 eps = 30  IOU = 0.05  NMS = 0.01 NF
    
    float confidenceThreshold = 0.5;

    AntitamperingMrzData document;
    document.imageName = convertStringtoCharPtr2(imagePath);
    document.confidenceThreshold = confidenceThreshold;

    AntitamperingMrzFactory antitamperingMrzFactory;
    std::shared_ptr<AntitamperingMrz> antitamperingMrz = antitamperingMrzFactory.createAntitamperingMrz(networkPath);
    antitamperingMrz->check(document);
    return 0;
}
*/