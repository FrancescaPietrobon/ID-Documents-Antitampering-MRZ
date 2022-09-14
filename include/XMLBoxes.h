#ifndef XML_BOXES_H
#define XML_BOXES_H

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "pugixml.hpp"
#include "utilities.h"
#include "Boxes.h"

typedef std::vector<std::vector<float>> matrix2D;
typedef std::vector<std::vector<std::vector<float>>> matrix3D;
typedef std::vector<std::vector<std::vector<std::vector<float>>>> matrix4D;

class XMLBoxes: public Boxes
{
    private:
        const char* XMLPath;

    public:
        XMLBoxes(Document doc, const char* xml):
            Boxes(doc), XMLPath(xml){}
        void extractBoxes();
        void printXMLBoxes();
};

#endif