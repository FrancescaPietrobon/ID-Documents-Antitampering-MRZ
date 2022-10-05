#include "../include/Fields.h"

Fields::Fields(std::vector<Character> chars, int n):
    originalCluster(chars), numClusters(n)
    {
        MRZ mrz;
        fillFields();  
    }

size_t Fields::getNumDoubtfulFields()
{
    return numDoubtfulFields;
}

bool Fields::getResult()
{
    return result;
}

float Fields::getFinalConf()
{
    return finalConf;
}
 
std::map<std::string, std::pair<std::pair<std::string,std::string>, float>> Fields::getFinalAssociations()
{
    return finalAssociation;
}


void Fields::fillFields()
{
    Field field(originalCluster);
    for(int i = 0; i <= numClusters; ++i)
    {
        for(int j = 0; j < originalCluster.size(); ++j)
        {
            if(originalCluster[j].getCluster() == i)
            {
                field.setField(originalCluster[j].getX(), originalCluster[j]);
                if(!field.getIsPartOfMRZ() && originalCluster[j].getLabel() == "<")
                {
                    field.setisPartOfMRZ(true);
                    ++numLineOfMRZ;
                }
            }
        }
        fields.push_back(field);
        field.clear();
    }
    
    // Consider single characters as field
    for(int j = 0; j < originalCluster.size(); ++j)
    {
        if(originalCluster[j].getCluster() == -2)
        {
            field.setField(originalCluster[j].getX(), originalCluster[j]);
            fields.push_back(field);
            field.clear();
            numClusters++;
        }
    }

    for(auto & field: fields)
    {
        field.computeMeanY();

        std::string data;
        for(auto & character: field.getField())
            data += character.second.getLabel();
        field.setData(data);
        std::cout << data << std::endl;
    }       
}


void Fields::checkMRZ()
{
    int count = 0;
    std::vector<std::vector<Character>> mrzChar;

    // Save in splittedMRZ all the mrz lines ordered wrt the mean y coordinate
    for(auto & field: fields)
    {
        if(count > numLineOfMRZ)
            break;
        if(field.getIsPartOfMRZ())
        {
            field.setTypeOfData("MRZ");
            splittedMRZ.emplace(field.getMeanY(), field);
            ++count;
        }
    }

    for(auto mrzLine: splittedMRZ)
        mrzChar.push_back(mrzLine.second.getCharacters());

    if(count > 1) //if MRZ is detected
    {
        std::string MRZType = findMRZType(mrzChar);

        std::cout << "MRZ type: "<< MRZType << std::endl;

        if(MRZType == "TD1")
        {
            TD1 mrz(mrzChar, numLineOfMRZ);
            mrz.extractFields();
            mrzGeneral = mrz;
        }
        else if(MRZType == "TD2")
        {
            TD2 mrz(mrzChar, numLineOfMRZ);
            mrz.extractFields();
            mrzGeneral = mrz;
        }
        else if(MRZType == "TD3")
        {
            TD3 mrz(mrzChar, numLineOfMRZ);
            mrz.extractFields();
            mrzGeneral = mrz;
        }
        else if(MRZType == "MRVA")
        {
            MRVA mrz(mrzChar, numLineOfMRZ);
            mrz.extractFields();
            mrzGeneral = mrz;
        }
        else if(MRZType == "MRVB")
        {
            MRVB mrz(mrzChar, numLineOfMRZ);
            mrz.extractFields();
            mrzGeneral = mrz;
        }
        else
            std::cerr << "MRZ type not found." << std::endl;
        
        std::cout << std::endl;
        std::cout << "MRZ:" << std::endl;
        mrzGeneral.printMRZ();
        std::cout << std::endl;
        mrzGeneral.printMRZFields();
    }
    else
        std::cerr << "MRZ not found." << std::endl;
}


std::string Fields::findMRZType(std::vector<std::vector<Character>> mrz)
{
    std::string MRZType;
    if(numLineOfMRZ == 3)
    {
        MRZType = "TD1";
    }
    else if(mrz[0].size() >= 32 && mrz[0].size() <= 40) // if(mrz[0].size() == 36) restrictive case
    {
        if(mrz[0][0].getLabel() == "P")
            MRZType = "TD2";
        else
            MRZType = "MRVB";
    }
    else if(mrz[0].size() > 40 && mrz[0].size() <= 48) // if(mrz[0].size() == 44) restrictive case
    {
        if(mrz[0][0].getLabel() == "P")
            MRZType = "TD3";
        else
            MRZType = "MRVA";
    }
    else
        MRZType = "NULL";

    return MRZType;
}


void Fields::printOrderedFields()
{
    for(auto & field: fields)
        std::cout << field.getMeanY() << " " << field.getData() << " is part of MRZ: " << field.getIsPartOfMRZ() << std::endl;
}


void Fields::checkAlphanumDate(Field & field)
{
    Date date(field.getData());
    
    // REQUIRED DATE DETECTED CORRECTLY
    // If it is a date, it is refactorized in format yymmdd.
    if(date.isDate())
        field.setData(date.getNewDate());
}


void Fields::compareMRZFields(metricsType metricType)
{
    //OCCHIO CHE SE HO SPECIMEN sia per nome che cognome mi dirà entrambe le volte SPECIMEN è cognome (primo che vede)
    // Then check for identical field
    for(auto & field: fields)
    {
        if(field.getData().size() >= 6 && field.getData().size() <= 12)
            checkAlphanumDate(field);
        auto search = mrzGeneral.getAllFieldsInv().find(field.getData());
        std::cout << "\nField: " << field.getData() << std::endl;
        //std::cout << " Find: " << search->first << std::endl;
        if(search != mrzGeneral.getAllFieldsInv().end()) //NON VA!!!
        {
            if((field.getData() == search->first) && (field.getTypeOfData() == ""))
            {
                std::cout << "Field: " << field.getData() << " ";
                std::cout << " Find: " << search->first << " " << search->second << std::endl;
                // GESTISCI ATTRIBUZIONI MULTIPLE DELLO STESSO TIPO IN setTypeOfData
                field.setTypeOfData(search->second);
                finalAssociation.emplace(search->second, std::make_pair(std::make_pair(field.getData(), search->first), 1.));
                std::cout << "Associated" << std::endl;
            }
            mostCompatible(field, metricType);
        }      
    }
    std::cout << std::endl;
    
    for(auto &association: finalAssociation)
    {
        if(association.second.second != 1)
            numDoubtfulFields++;
    }

    if(numDoubtfulFields != 0)
        result = false;

    computeFinalConf();
    
}


void Fields::mostCompatible(Field & field, metricsType metricType)
{
    float currentComp = 0;
    std::string bestField = mrzGeneral.getAllFieldsInv().begin()->second;
    //std::cout << "first field: " << bestField << std::endl;
    if(metricType == pairs)
    {
        float maxComp = 0;
        for(auto mrzField: mrzGeneral.getAllFieldsInv())
        {
            std::cout << "Field: " << field.getData() << "   MRZ Field: " << mrzField.first;
            //currentComp = countPairs(field.getData(), mrzField.first);
            currentComp = countPairs(field.getData(), mrzField.first);
            std::cout << "   Comp: " << currentComp << std::endl;
            if(currentComp > maxComp)
            {
                maxComp = currentComp;
                bestField = mrzField.second;
            }
        }
        float conf = static_cast<float>(maxComp/(field.getData().size()));

        if(finalAssociation.find(bestField) == finalAssociation.end())
        {
            //std::map<std::string, std::pair<std::pair<std::string,std::string>, float>> finalAssociationNEW;
            finalAssociation.emplace(bestField, std::make_pair(std::make_pair(field.getData(), mrzGeneral.getAllFields().find(bestField)->second), conf));
            field.setTypeOfData(bestField);
            std::cout << "Associated" << std::endl;
        }
        else if(finalAssociation.find(bestField)->second.second < conf)
        {
            std::cout << "Prev conf: "<< finalAssociation.find(bestField)->second.second << "\t curr conf: " << conf << std::endl;
            finalAssociation.find(bestField)->second = std::make_pair(std::make_pair(field.getData(), mrzGeneral.getAllFields().find(bestField)->second), conf);
            std::cout << "Associated" << std::endl;
        }
    }
    else if(metricType == distLev)
    {
        float minComp = distanceLevenshtein(field.getData(), (*mrzGeneral.getAllFieldsInv().begin()).first);
        for(auto mrzField: mrzGeneral.getAllFieldsInv())
        {
            std::cout << "Field: " << field.getData() << "   MRZ Field: " << mrzField.first;
            //currentComp = countPairs(field.getData(), mrzField.first);
            currentComp = distanceLevenshtein(field.getData(), mrzField.first);
            std::cout << "   Comp: " << currentComp << std::endl;
            if(currentComp < minComp)
            {
                minComp = currentComp;
                bestField = mrzField.second;
            }
        }
        float conf = 1 - static_cast<float>(minComp/(field.getData().size()));
        if(mrzGeneral.getAllFields().find(bestField) != mrzGeneral.getAllFields().end()) //!!!!
        {
            if(finalAssociation.find(bestField) == finalAssociation.end())
            {
                finalAssociation.emplace(bestField, std::make_pair(std::make_pair(field.getData(), mrzGeneral.getAllFields().find(bestField)->second), conf));
                field.setTypeOfData(bestField);
                std::cout << "Associated" << std::endl;
            }
            else if(finalAssociation.find(bestField)->second.second < conf)
            {
                std::cout << "Prev conf: "<< finalAssociation.find(bestField)->second.second << "\t curr conf: " << conf << std::endl;
                finalAssociation.find(bestField)->second = std::make_pair(std::make_pair(field.getData(), mrzGeneral.getAllFields().find(bestField)->second), conf);
                std::cout << "Associated" << std::endl;
            }
        }
        
    }
}


void Fields::printNotFilledAndFilledFields()
{
    std::cout << "Not filled fields:" << std::endl;
    for(auto & field: fields)
        if(field.getTypeOfData() == "")
            std::cout << "None: " << field.getData() << std::endl;

    std::cout << std::endl;        
    
    std::cout << "Filled fields:" << std::endl;
    for(auto & field: fields)
        if(field.getTypeOfData() != "")
            std::cout << field.getTypeOfData() << ": " << field.getData() << std::endl;
}


void Fields::printAssociations()
{
    std::cout << "\nAssociated fields:" << std::endl;
    for(auto & association: finalAssociation)
    {
        std::cout << "\nField: " << association.first << std::endl;
        std::cout << "data-field: " << association.second.first.first << std::endl;
        std::cout << "mrz-data-field: " << association.second.first.second << std::endl;
        std::cout << "confidence: " << association.second.second << std::endl;
    }
}


void Fields::printDoubtfulFields()
{
    std::cout << "\nDoubtful fields:" << std::endl;
    bool existDubField = false;
    for(auto & association: finalAssociation)
    {
        if(association.second.second != 1.)
        {
            std::cout << "\nField: " << association.first << std::endl;
            std::cout << "data-field: " << association.second.first.first << std::endl;
            std::cout << "mrz-data-field: " << association.second.first.second << std::endl;
            std::cout << "confidence: " << association.second.second << std::endl;
            existDubField = true;
        }
    }
    
    if(!existDubField)
        std::cout << "None"<< std::endl;
}


void Fields::computeFinalConf()
{
    float sum = 0;
    for(auto & association: finalAssociation)
        sum += association.second.second;
    
    finalConf = sum / finalAssociation.size();
}