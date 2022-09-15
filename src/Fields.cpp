#include "../include/Fields.h"

Fields::Fields(std::vector<myPoint> chars, int n):
    originalCluster(chars), numClusters(n)
    {
        fillFields();
        
    }


void Fields::fillFields()
{
    std::multimap<float, myPoint> field;
    for(int i = 0; i < numClusters; ++i)
    {
        for(int j = 0; j < originalCluster.size(); ++j)
        {
            if(originalCluster[j].getCluster() == i)
            {
                field.emplace(originalCluster[j].getX(), originalCluster[j]);
            }
        }
        fields.emplace(i, field);
        field.clear();
    }
    
    for(int j = 0; j < originalCluster.size(); ++j)
    {
        if(originalCluster[j].getCluster() == -2)
        {
            field.emplace(originalCluster[j].getX(), originalCluster[j]);
            fields.emplace(numClusters, field);
            field.clear();
            numClusters++;
        }
    }

    extern std::unordered_map<unsigned, std::string> dictionary;

    for(auto cluster: fields)
    {
        std::cout << cluster.first << " ";
        for(auto it = (cluster.second).begin(); it != (cluster.second).end(); ++it)
        {
            std::cout << dictionary[((*it).second).getLabel()];
        }
        std::cout << std::endl;
    }

}

