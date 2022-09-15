#include "../include/Fields.h"


Field::Field(std::vector<myPoint> chars):
    originalCluster(chars)
    {
        partOfMRZ = false;
    }


void Field::setField(float x, myPoint point)
{
    field.emplace(x, point);
}

std::multimap<float, myPoint> Field::getField()
{
    return field;
}

void Field::clear()
{
    field.clear();
    partOfMRZ = false;
}

void Field::isMRZ()
{
    partOfMRZ = true;
}

bool Field::getPartOfMRZ()
{
    return partOfMRZ;
}

void Field::print()
{
    extern std::unordered_map<unsigned, std::string> dictionary;
    for(auto it = field.begin(); it != field.end(); ++it)
    {
        std::cout << dictionary[((*it).second).getLabel()];
    }
}


Fields::Fields(std::vector<myPoint> chars, int n):
    originalCluster(chars), numClusters(n)
    {
        fillFields();  
    }


void Fields::fillFields()
{
    Field field(originalCluster);
    for(int i = 0; i < numClusters; ++i)
    {
        for(int j = 0; j < originalCluster.size(); ++j)
        {
            if(originalCluster[j].getCluster() == i)
            {
                field.setField(originalCluster[j].getX(), originalCluster[j]);
                if(originalCluster[j].getLabel() == 64)
                    field.isMRZ();
            }
        }
        fields.emplace(i, field);
        field.clear();
    }
    
    for(int j = 0; j < originalCluster.size(); ++j)
    {
        if(originalCluster[j].getCluster() == -2)
        {
            field.setField(originalCluster[j].getX(), originalCluster[j]);
            fields.emplace(numClusters, field);
            field.clear();
            numClusters++;
        }
    }

    

    for(auto cluster: fields)
    {
        std::cout << cluster.first << " ";

        cluster.second.print();
        /*
        for(auto it = ((cluster.second).getField()).begin(); it != ((cluster.second).getField()).end(); ++it)
        {
            std::cout << dictionary[((*it).second).getLabel()];
        }
        */
        std::cout  << " is part of MRZ: " << (cluster.second).getPartOfMRZ() << std::endl;
    }

}

