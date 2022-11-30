#ifndef _ANTITAMPERING_MRZ_EUCLDIST_H_
#define _ANTITAMPERING_MRZ_EUCLDIST_H_

#include "../AntitamperingMrz.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

#include <vector>
#include <numeric>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "pugixml.hpp"
#include "utilities.h"
#include "ModelBoxes.h"
#include "XMLBoxes.h"
#include "DBSCAN.h"
#include "Anchors.h"
#include "Dictionary.h"
#include "Date.h"
#include "Fields.h"
#include "Character.h"
#include "Response.h"

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
#define EPS 10 // it depends on the image

typedef std::vector<std::vector<float>> matrix2D;
typedef std::vector<std::vector<std::vector<float>>> matrix3D;
typedef std::vector<std::vector<std::vector<std::vector<float>>>> matrix4D;

class AntitamperingMrzEuclDist : public AntitamperingMrz {
    public:
        AntitamperingMrzEuclDist(const std::string& OCRModelFilename);
        virtual OcrMrzResponseResult check(const AntitamperingMrzData& data);
        std::pair<matrix2D, std::vector<float>> predictFromModel(Document &document, int numClasses, float thresholdIOU, float thresholdNMS);
    protected:
        std::string modelPath;
        std::pair<matrix2D, std::vector<float>> predictFromXML(Document &document, const char* XMLPath);
        
};

#endif /* _ANTITAMPERING_MRZ_EUCLDIST_H_ */