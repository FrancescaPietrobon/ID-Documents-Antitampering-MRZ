#include "../include/XMLBoxes.h"
#include "../include/utilities.h"
#include "../include/pugixml.hpp"


XMLBoxes::XMLBoxes(Document doc, const char* xml):
    document(doc), XMLPath(xml){}


void XMLBoxes::predictFromXML()
{

    std::pair<matrix2D, std::vector<float>> result = extractBoxes();

    savePredictionImage(document.getDocument(), result.first, result.second, "../pred_xml.jpg");
}


std::pair<matrix2D, std::vector<float>> XMLBoxes::extractBoxes()
{
    std::vector<float> singleBox, singleBoxReshaped;
    float xmin, ymin, xmax, ymax;
    std::string label;
    unsigned labelIdx;

    pugi::xml_document doc;
    doc.load_file(XMLPath);

    extern std::unordered_map<std::string, unsigned> inverse_dictionary; // = inverse_map(dictionary);
    //extern std::unordered_map<unsigned, std::string> dictionary;

    pugi::xml_node tools = doc.child("annotation");
    for (pugi::xml_node object: tools.children("object"))
    {
        label = std::string(object.child_value("name"));
        labelIdx = inverse_dictionary[label];
        classes.push_back(labelIdx);

        pugi::xml_node box = object.child("bndbox");
        xmin = std::atof(box.child_value("xmin"));
        ymin = std::atof(box.child_value("ymin"));
        xmax = std::atof(box.child_value("xmax"));
        ymax = std::atof(box.child_value("ymax"));
        singleBox = {xmin, ymin, xmax, ymax};
        singleBoxReshaped = {xmin * document.getXAlter(), ymin * document.getYAlter(),
                             xmax * document.getXAlter(), ymax * document.getYAlter()};
        boxes.push_back(singleBox);
        boxesReshaped.push_back(singleBoxReshaped);
    }
    
    std::pair<matrix2D, std::vector<float>> result(boxesReshaped, classes);

    return result;
}


void XMLBoxes::printXMLBoxes()
{
    pugi::xml_document doc;
    doc.load_file(XMLPath);
    pugi::xml_node tools = doc.child("annotation");

    for (pugi::xml_node object: tools.children("object"))
    {
        std::cout << "Object " << object.child_value("name");
        pugi::xml_node box = object.child("bndbox");
        std::cout << " : xmin " << box.child_value("xmin");
        std::cout << ", ymin " << box.child_value("ymin");
        std::cout << ", xmax " << box.child_value("xmax");
        std::cout << ", ymax " << box.child_value("ymax") << std::endl;
    }
}






