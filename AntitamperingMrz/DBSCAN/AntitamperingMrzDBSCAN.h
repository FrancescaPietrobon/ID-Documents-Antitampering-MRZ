#ifndef _ANTITAMPERING_MRZ_DBSCAN_H_
#define _ANTITAMPERING_MRZ_DBSCAN_H_

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
#define MIN_PTS 1

typedef std::vector<std::vector<float>> matrix2D;
typedef std::vector<std::vector<std::vector<float>>> matrix3D;
typedef std::vector<std::vector<std::vector<std::vector<float>>>> matrix4D;

class AntitamperingMrzDBSCAN : public AntitamperingMrz {
    public:
        AntitamperingMrzDBSCAN(const std::string& OCRModelFilename): AntitamperingMrz(){};
        virtual OcrMrzResponseResult check(const AntitamperingMrzData& data);
    protected:
        std::pair<matrix2D, std::vector<float>> predictFromXML(Document &document, const char* XMLPath);
        std::pair<matrix2D, std::vector<float>> predictFromModel(Document & document, std::string networkPath, int numClasses, float thresholdIOU, float thresholdNMS);
        matrix2D computeCenters(matrix2D boxes, matrix2D anchorBoxes);
};

#endif /* _ANTITAMPERING_MRZ_DBSCAN_H_ */