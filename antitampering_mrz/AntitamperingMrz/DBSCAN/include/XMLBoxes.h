#ifndef XMLBOXES_H
#define XMLBOXES_H

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "pugixml.hpp"
#include "Boxes.h"

typedef std::vector<std::vector<float>> matrix2D;

class XMLBoxes: public Boxes
{
    private:
        const char* XMLPath;

    public:
        XMLBoxes(Document doc, const char* xml);
        void extractBoxes();
        void printXMLBoxes();
};

#endif