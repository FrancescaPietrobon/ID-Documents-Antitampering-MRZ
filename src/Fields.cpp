#include "../include/Fields.h"

Fields::Fields(std::vector<Character> chars, int n):
    originalCluster(chars), numClusters(n)
    {
        //fillFields();  
    }

int Fields::getNumLineOfMRZ()
{
    return numLineOfMRZ;
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
                if(!field.getIsPartOfMRZ() && originalCluster[j].getLabel() == "<")
                {
                    field.isPartOfMRZ();
                    ++numLineOfMRZ;
                }
                    
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

    for(auto field: fields)
        field.second.computeMeanY(); //NON LO SALVA
}

void Fields::printOrderedFields()
{
    for(auto field: fields)
    {
        field.second.computeMeanY();
        std::cout << field.first << " " << field.second.getMeanY() << " ";
        for(auto character: field.second.getField())
        {
            std::cout << character.second.getLabel();
        }

        //field.second.print();
        std::cout  << " is part of MRZ: " << (field.second).getIsPartOfMRZ() << std::endl;
    }
}

std::string Fields::findMRZType(std::vector<std::vector<Character>> mrz)
{
    std::string MRZType;
    if(numLineOfMRZ == 3)
    {
        MRZType = "TD1";
    }
    else if(mrz[0].size() == 36)
    {
        if(mrz[0][0].getLabel() == "P")
            MRZType = "TD2";
        else
            MRZType = "MRVB";
    }
    else if(mrz[0].size() == 44)
    {
        if(mrz[0][0].getLabel() == "P")
            MRZType = "TD3";
        else
            MRZType = "MRVA";
    }
    return MRZType;
}


void Fields::checkMRZ()
{
    int count = 0;
    //std::map<float, Field> splittedMRZ;
    std::vector<std::vector<Character>> mrzChar;
    // Save in splittedMRZ all the mrz lines ordered wrt the mean y coordinate
    for(auto field: fields)
    {
        field.second.computeMeanY();
        if(count > numLineOfMRZ)
            break;
        if(field.second.getIsPartOfMRZ())
        {
            splittedMRZ.emplace(field.second.getMeanY(), field.second);
            ++count;
        }
    }

    for(auto mrzLine: splittedMRZ)
        mrzChar.push_back(mrzLine.second.getCharacters());

    std::string MRZType = findMRZType(mrzChar);

    std::cout << "MRZ type: "<< MRZType << std::endl;

    if(MRZType == "TD3")
    {
        TD3 mrz(mrzChar, numLineOfMRZ);
        std::cout << "MRZ:" << std::endl;
        mrz.printMRZ();
        std::cout << std::endl;
        mrz.extractFields();
        mrz.printMRZFields();
    }

}

/*
void Fields::printMRZFields(MRZ & mrz)
{
    std::cout << "Document type: ";
    std::cout << mrz.getDocType() << std::endl;
    std::cout << "State: ";
    std::cout << mrz.getState() << std::endl;
    std::cout << "Surname: ";
    std::cout << mrz.getSurname() << std::endl;
    std::cout << "Name: ";
    std::cout << mrz.getName() << std::endl;
    std::cout << "Document number: ";
    std::cout << mrz.getDocNumber() << std::endl;
    std::cout << "Check document number: ";
    std::cout << mrz.getCheckDocNum() << std::endl;
    std::cout << "Nationality: ";
    std::cout << mrz.getNationality() << std::endl;
    std::cout << "Date of birth: ";
    std::cout << mrz.getDateBirth() << std::endl;
    std::cout << "Check date of birth: ";
    std::cout << mrz.getCheckDateBirth() << std::endl;
    std::cout << "Sex: ";
    std::cout << mrz.getSex() << std::endl;
    std::cout << "Date of expire: ";
    std::cout << mrz.getDateExpireDoc() << std::endl;
    std::cout << "Check date of expire: ";
    std::cout << mrz.getCheckDateExpireDoc() << std::endl;
    std::cout << "Optional data: ";
    std::cout << mrz.getOptionalData() << std::endl;
    std::cout << "Check optional data: ";
    std::cout << mrz.getCheckOptionalData() << std::endl;
    std::cout << "Check overall: ";
    std::cout << mrz.getCheckOverall() << std::endl;
}
*/
    /*

            if(mrz.size() == 0)
                mrz.push_back(field.second);
            else
            {
                if(mrz[0].getMeanY() > field.second.getMeanY())
                {
                    mrz[1] = mrz[0];
                    mrz[0] = field.second;
                }
                else
                    mrz.push_back(field.second);

            }
            for(auto character: field.second.getField())
            {
                splittedMRZ.emplace(character.second.getY(), field.second);
                std::cout << character.second.getLabel();
            }
            std::cout << std::endl;
            ++count;
        }
    }
    */
   /*
    for(auto mrz: splittedMRZ)
    {
        std::cout << mrz.second.getField().size() << std::endl;
    }
    */


/*
void Fields::fillLabels()
{
    for(auto field: fields)
    {
        if(field.second.getIsPartOfMRZ())
        {
            field.second.setTypeOfData("MRZ");
            std::cout << numLineOfMRZ << std::endl;
        }
    }

    std::cout << numLineOfMRZ << std::endl;
    if(numLineOfMRZ == 3)
        field.second.setTypeOfData("MRZ"); //FAI UN DICTIONARY con i possibili campi
        MRZType = "TD1";
}
*/

