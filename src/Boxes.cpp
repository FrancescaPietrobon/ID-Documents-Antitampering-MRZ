#include "../include/Boxes.h"

Boxes::Boxes(Document doc):
    document(doc){}


matrix2D Boxes::getBoxes()
{
    return boxes;
}

std::vector<float> Boxes::getClasses()
{
    return classes;
}