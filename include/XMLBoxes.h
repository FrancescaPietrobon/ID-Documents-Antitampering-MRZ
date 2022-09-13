#ifndef _XML_BOXES_H_
#define _XML_BOXES_H_

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "Document.h"

typedef std::vector<std::vector<float>> matrix2D;
typedef std::vector<std::vector<std::vector<float>>> matrix3D;
typedef std::vector<std::vector<std::vector<std::vector<float>>>> matrix4D;

class XMLBoxes
{
    private:
        Document document;
        const char* XMLPath;
        matrix2D boxes;
        matrix2D boxesReshaped;
        std::vector<float> classes;
        float xmin, ymin, xmax, ymax;
        std::string label;
        unsigned label_idx;

    public:
        XMLBoxes(Document, const char*);
        std::pair<matrix2D, std::vector<float>> extractBoxes();
        void predictFromXML();
        void printXMLBoxes();
};

#endif