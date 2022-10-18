#include "../include/Fields.h"

Fields::Fields(std::vector<Character> chars, int n, float conf):
    originalCluster(chars), numClusters(n), confThreshold(conf)
    {
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

float Fields::getConfFinal()
{
    return confFinal;
}
 
std::map<std::string, std::pair<std::pair<std::string,std::string>, float>> Fields::getFinalAssociations()
{
    return finalAssociation;
}

MRZ Fields::getMRZ()
{
    return mrzGeneral;
}


void Fields::fillFields()
{
    Field field;
    for(int i = 0; i <= numClusters; ++i)
    {
        for(size_t j = 0; j < originalCluster.size(); ++j)
        {
            if(originalCluster[j].getCluster() == i)
            {
                field.setField(originalCluster[j].getX(), originalCluster[j]);
                if(!field.getIsPartOfMRZ() && originalCluster[j].getLabel() == "<")
                {
                    field.setIsPartOfMRZ(true);
                    ++numLineOfMRZ;
                }
            }
        }
        fields.push_back(field);
        field.clear();
    }
    
    // Consider single characters as field
    for(size_t j = 0; j < originalCluster.size(); ++j)
    {
        if(originalCluster[j].getCluster() == -2)
        {
            field.setField(originalCluster[j].getX(), originalCluster[j]);
            fields.push_back(field);
            field.clear();
            numClusters++;
        }
    }

    std::cout << "\nFields detected by DBSCAN:" << std::endl;
    for(auto & field: fields)
    {
        field.computeMeanY();
        field.computeOrderedCharacters();

        std::string data;
        for(auto & character: field.getCharacters())
            data += character.getLabel();
        field.setData(data);
        std::cout << data << std::endl;
    }       
}


bool Fields::findMRZ()
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
        MRZType = findMRZType(mrzChar);
        std::cout << "\nMRZ type: "<< MRZType << std::endl;

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
        {
            std::cout << "MRZ type not found." << std::endl;
            return false;
        }

        std::cout << "\nMRZ:" << std::endl;
        mrzGeneral.printMRZ();
        std::cout << std::endl;
        mrzGeneral.printMRZFields();
    }
    else
    {
        std::cout << "MRZ not found." << std::endl;
        return false;
    }
    return true;   
}


std::string Fields::findMRZType(std::vector<std::vector<Character>> mrz)
{
    std::string MRZType;
    if((numLineOfMRZ == 3) && (mrz[0].size() == 36) && (mrz[1].size() == 36) && (mrz[2].size() == 36))
    {
        MRZType = "TD1";
    }
    else if((mrz[0].size() == 36) && (mrz[1].size() == 36)) // if(mrz[0].size() >= 32 && mrz[0].size() <= 40) less restrictive case
    {
        if(mrz[0][0].getLabel() == "P")
            MRZType = "TD2";
        else
            MRZType = "MRVB";
    }
    else if((mrz[0].size() == 44) && (mrz[1].size() == 44)) // if(mrz[0].size() > 40 && mrz[0].size() <= 48) less restrictive case
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
    std::cout << "\nFields detected:" << std::endl;
    for(auto & field: fields)
        std::cout << field.getMeanY() << " " << field.getData() << " is part of MRZ: " << field.getIsPartOfMRZ() << std::endl;
    std::cout << std::endl;
}


void Fields::compare()
{
    std::vector<detection> mrzFields = mrzGeneral.getAllFieldsStruct();
    float currentComp = 0;
    std::string bestTypeField, bestField;
    for(auto & field: fields)
    {
        if(!field.getIsPartOfMRZ())
        {
            if((field.getData().size() >= 6) && (field.getData().size() <= 12))
                checkDate(field);

            float minComp = distanceLevenshtein(field.getData(), (*mrzFields.begin()).fieldMRZ);

            for(size_t itFields = 0; itFields < mrzFields.size(); ++itFields)
            {
                std::cout << "Field: " << field.getData() << "   MRZ Field: " << mrzFields[itFields].fieldMRZ;
                std::cout << "   MRZ Field type: " << mrzFields[itFields].typeFieldMRZ;
                currentComp = distanceLevenshtein(field.getData(), mrzFields[itFields].fieldMRZ);
                std::cout << "   Comp: " << currentComp << std::endl;
                if(currentComp <= minComp)
                {
                    //std::cout << mrzFields[itFields].typeFieldMRZ << " new best " << std::endl;
                    minComp = currentComp;
                    bestTypeField = mrzFields[itFields].typeFieldMRZ;
                    bestField = mrzFields[itFields].fieldMRZ;
                }
                if(currentComp == 0)
                {
                    //std::cout << "Field to remove: " << field.getData() << "   MRZ Field: " << mrzFields[itFields].fieldMRZ << "   MRZ Field type: " << mrzGeneral.getAllFieldsStruct()[itFields].typeFieldMRZ << std::endl;
                    mrzFields.erase(mrzFields.begin()+itFields);
                    ++itFields;
                    break;
                }
            }
            float conf = 1 - static_cast<float>(std::max(static_cast<float>(0.), minComp/(field.getData().size())));
            std::cout << "Confidence: " << conf << std::endl;

            int exists = 0;
            for(size_t itDoubtFilds = 0; itDoubtFilds < doubtfulAss.size(); ++itDoubtFilds)
            {
                //std::cout << "Doubtful field type: "<< doubtfulAss[itDoubtFilds].fieldType << "\t conf: " << doubtfulAss[itDoubtFilds].confidence << std::endl;
                if((doubtfulAss[itDoubtFilds].fieldType == bestTypeField))
                {
                    exists = 1;
                    if(doubtfulAss[itDoubtFilds].confidence < conf)
                    {
                        std::cout << "Prev conf: "<< doubtfulAss[itDoubtFilds].confidence << "\t curr conf: " << conf << std::endl;
                        //std::cout << "Field to remove: " << doubtfulAss[itDoubtFilds].dataField << "   MRZ Field: " << doubtfulAss[itDoubtFilds].mrzDataField << "   MRZ Field type: " << doubtfulAss[itDoubtFilds].fieldType << std::endl;
                        doubtfulAss.erase(doubtfulAss.begin()+itDoubtFilds);
                        ++itDoubtFilds;
                        association ass;
                        ass.dataField = field.getData();
                        ass.mrzDataField = bestField;
                        ass.fieldType = bestTypeField;
                        ass.confidence = conf;

                        if(conf == 1)
                        {
                            std::cout << "Added in final association: " << field.getData() << " - " << bestField << std::endl;
                            finAss.push_back(ass);
                        }
                        else
                        {
                            std::cout << "Added in doubtful association: " << field.getData() << " - " << bestField  << std::endl;
                            doubtfulAss.push_back(ass);
                        }
                        break;
                    }
                }
            }
            if((exists == 0) && (conf > 0))
            {
                association ass;
                ass.dataField = field.getData();
                ass.mrzDataField = bestField;
                ass.fieldType = bestTypeField;
                ass.confidence = conf;

                if(conf == 1)
                {
                    std::cout << "Added in final association: " << field.getData() << " - " << bestField  << std::endl;
                    finAss.push_back(ass);
                }
                else
                {
                    std::cout << "Added in doubtful association: " << field.getData() << " - " << bestField  << std::endl;
                    doubtfulAss.push_back(ass);
                }
            }
        }
        std::cout << std::endl;
    }
    numDoubtfulFields = doubtfulAss.size();
    computeConfFinal();
}


void Fields::printFinAss()
{
    std::cout << "\nAssociations:" << std::endl;
    for(auto ass: finAss)
    {
        std::cout << "\nField: " << ass.fieldType << std::endl;
        std::cout << "data-field: " << ass.dataField << std::endl;
        std::cout << "mrz-data-field: " << ass.mrzDataField << std::endl;
        std::cout << "confidence: " << ass.confidence << std::endl;
    }
}


void Fields::printDoubtfulAss()
{
    std::cout << "\nDoubtful fields:" << std::endl;
    for(auto ass: doubtfulAss)
    {
        std::cout << "\nField: " << ass.fieldType << std::endl;
        std::cout << "data-field: " << ass.dataField << std::endl;
        std::cout << "mrz-data-field: " << ass.mrzDataField << std::endl;
        std::cout << "confidence: " << ass.confidence << std::endl;
    }
}


void Fields::computeConfFinal()
{
    float sum = 0;
    for(auto ass: finAss)
        sum += ass.confidence;
    for(auto ass: doubtfulAss)
        sum += ass.confidence;
    
    confFinal = sum / (finAss.size() + doubtfulAss.size());

    if(confFinal < confThreshold)
        result = false;
}

// DA RIGUARDARE!!

void Fields::compareMRZFields(metricsType metricType)
{
    for(auto & field: fields)
    {
        if((field.getData().size() >= 6) && (field.getData().size() <= 12))
            checkDate(field);
        std::string nameFiled = field.getData();

        for(auto it = mrzGeneral.getAllFieldsStruct().begin(); it != mrzGeneral.getAllFieldsStruct().end(); ++it)
        {

        }



        std::cout << nameFiled << std::endl;
        auto search = std::find_if(mrzGeneral.getAllFieldsStruct().begin(), mrzGeneral.getAllFieldsStruct().end(), 
                       [nameFiled] (const detection& d) { std::cout << d.fieldMRZ << std::endl; return (d.fieldMRZ == nameFiled);});
        std::cout << "\nField: " << field.getData() << " type of data: " << field.getTypeOfData() << std::endl;
        if(search != mrzGeneral.getAllFieldsStruct().end())
        {
            if (field.getTypeOfData() == "None")
            {
                std::cout << "Equal" << std::endl;
                std::cout << "Field: " << field.getData() << " ";
                std::cout << " Find: " << search->fieldMRZ << " " << search->typeFieldMRZ << std::endl;
                field.setTypeOfData(search->typeFieldMRZ);
                //finalAssociation.emplace(search->typeFieldMRZ, std::make_pair(std::make_pair(field.getData(), search->fieldMRZ), 1.));
                association ass;
                ass.dataField = field.getData();
                ass.mrzDataField = search->fieldMRZ;
                ass.fieldType = search->typeFieldMRZ;
                ass.confidence = 1;
                finAss.push_back(ass);
                std::cout << "Associated with " << search->fieldMRZ << std::endl;
                mrzGeneral.removeAllFieldsStruct(search);
                std::cout << "post remove" << std::endl;
            } 
        }
        //else if(field.getTypeOfData() != "MRZ")
        //    mostCompatible(field, metricType);   
    }
    std::cout << std::endl;
    
    for(auto &association: finalAssociation)
    {
        if(association.second.second != 1)
            numDoubtfulFields++;
    }
    /*
    for(auto &association: finAss)
    {
        if(association.confidence != 1)
            numDoubtfulFields++;
    }
    */
    computeConfFinal();
}

/*
void Fields::compareMRZFieldsOLD(metricsType metricType)
{
    for(auto & field: fields)
    {
        if((field.getData().size() >= 6) && (field.getData().size() <= 12))
            checkDate(field);
        auto search = mrzGeneral.getAllFieldsInv().find(field.getData());
        //mrzGeneral.getAllFieldsStruct()
        std::cout << "\nField: " << field.getData() << std::endl;
        if((search != mrzGeneral.getAllFieldsInv().end()) && (field.getTypeOfData() == ""))
        {
            if(field.getData() == search->first)
            {
                std::cout << "Equal" << std::endl;
                std::cout << "Field: " << field.getData() << " ";
                std::cout << " Find: " << search->first << " " << search->second << std::endl;
                field.setTypeOfData(search->second);
                finalAssociation.emplace(search->second, std::make_pair(std::make_pair(field.getData(), search->first), 1.));
                association ass;
                ass.dataField = field.getData();
                ass.mrzDataField = search->first;
                ass.fieldType = search->second;
                ass.confidence = 1;
                finAss.push_back(ass);
                std::cout << "Associated with " << search->first << std::endl;
                mrzGeneral.removeFieldInv(search); //NEW
            }
            else
                mostCompatible(field, metricType);
            
        }      
    }
    std::cout << std::endl;
    
    for(auto &association: finalAssociation)
    {
        if(association.second.second != 1)
            numDoubtfulFields++;
    }
    
    //for(auto &association: finAss)
    //{
    //    if(association.confidence != 1)
    //        numDoubtfulFields++;
    //}
    
    computeConfFinal();
}
*/


void Fields::checkDate(Field & field)
{
    Date date(field.getData());
    
    // REQUIRED DATE DETECTED CORRECTLY
    // If it is a date, it is refactorized in format yymmdd.
    if(date.isDate())
        field.setData(date.getNewDate());
}


void Fields::mostCompatible(Field & field, metricsType metricType)
{
    float currentComp = 0;
    std::string bestTypeField = mrzGeneral.getAllFieldsStruct().begin()->typeFieldMRZ;
    std::string bestField = mrzGeneral.getAllFieldsStruct().begin()->fieldMRZ;
    //std::string bestField = mrzGeneral.getAllFieldsInv().begin()->second;
    //std::string bestField = "None";
    if(metricType == pairs)
    {
        float maxComp = 0;
        for(auto mrzField: mrzGeneral.getAllFieldsStruct())
        {
            std::cout << "Field: " << field.getData() << "   MRZ Field: " << mrzField.fieldMRZ;
            currentComp = countPairs(field.getData(), mrzField.fieldMRZ);
            std::cout << "   Comp: " << currentComp << std::endl;
            if(currentComp > maxComp)
            {
                maxComp = currentComp;
                bestTypeField = mrzField.typeFieldMRZ;
                bestField = mrzField.fieldMRZ;
            }
        }
        float conf = static_cast<float>(maxComp/(field.getData().size()));

        auto search = std::find_if(mrzGeneral.getAllFieldsStruct().begin(), mrzGeneral.getAllFieldsStruct().end(), 
                       [bestTypeField] (const detection& d) { 
                          return (d.fieldMRZ == bestTypeField); 
                       });
        if(finalAssociation.find(bestTypeField) == finalAssociation.end())
        {
            
            finalAssociation.emplace(bestTypeField, std::make_pair(std::make_pair(field.getData(), search->typeFieldMRZ), conf));
            //finalAssociation.emplace(bestField, std::make_pair(std::make_pair(field.getData(), mrzGeneral.getAllFields().find(bestField)->second), conf));
            /*
            association ass;
            ass.dataField = field.getData();
            ass.mrzDataField = search->first;
            ass.fieldType = search->second;
            ass.confidence = 1;
            finAss.push_back(ass);
            */
            field.setTypeOfData(bestTypeField);
            std::cout << "Associated with " << mrzGeneral.getAllFields().find(bestTypeField)->second << std::endl;
            std::cout << "Associated with " << search->typeFieldMRZ << std::endl;
        }
        else if(finalAssociation.find(bestTypeField)->second.second < conf)
        {
            std::cout << "Prev conf: "<< finalAssociation.find(bestTypeField)->second.second << "\t curr conf: " << conf << std::endl;
            //finalAssociation.find(bestField)->second = std::make_pair(std::make_pair(field.getData(), mrzGeneral.getAllFields().find(bestField)->second), conf);
            finalAssociation.find(bestTypeField)->second = std::make_pair(std::make_pair(field.getData(), search->typeFieldMRZ), conf);
            //std::cout << "Associated with " << mrzGeneral.getAllFields().find(bestField)->second << std::endl;
            std::cout << "Associated with " << search->typeFieldMRZ << std::endl;
        }
    }
    else if(metricType == distLev)
    {
        float minComp = distanceLevenshtein(field.getData(), (*mrzGeneral.getAllFieldsStruct().begin()).fieldMRZ);
        //for(auto mrzField: mrzGeneral.getAllFieldsInv())
        for(auto it = mrzGeneral.getAllFieldsStruct().begin(); it != mrzGeneral.getAllFieldsStruct().end(); ++it)
        {
            //std::cout << "Field: " << field.getData() << "   MRZ Field: " << mrzField.first;
            std::cout << "Field: " << field.getData() << "   MRZ Field: " << (*it).fieldMRZ;
            //currentComp = distanceLevenshtein(field.getData(), mrzField.first);
            currentComp = distanceLevenshtein(field.getData(), (*it).fieldMRZ);
            std::cout << "   Comp: " << currentComp << std::endl;
            if(currentComp < minComp)
            {
                minComp = currentComp;
                //bestField = mrzField.second;
                bestTypeField = (*it).typeFieldMRZ;
                bestField = (*it).fieldMRZ;
                std::cout << bestTypeField << " new best " << std::endl;
            }
            if(currentComp == 0)
            {
                //mrzGeneral.removeFieldInv(mrzGeneral.getAllFieldsInv().upper_bound(mrzField.first)); //NEW
                //mrzGeneral.removeFieldInv((it)); //NEW
                mrzGeneral.removeAllFieldsStruct(it);
                break;
            } 
        }
        float conf = 1 - static_cast<float>(minComp/(field.getData().size()));
        std::cout << "conf: " << conf << std::endl;

        auto searchType = std::find_if(finAss.begin(), finAss.end(), 
                       [bestTypeField] (const association& a) { 
                            //std::cout << "mrz f: " << d.typeFieldMRZ << " fieldbest: " << bestField << std::endl; 
                          return (a.fieldType == bestTypeField); 
                       });
        if(searchType == finAss.end()) //!!!!
        {
            association ass;
            ass.dataField = field.getData();
            ass.mrzDataField = bestField;
            ass.fieldType = bestTypeField;
            ass.confidence = conf;
            finAss.push_back(ass);
            std::cout << "Associated with " << bestField << std::endl;
        }
        else if(searchType->confidence < conf)
        {
            std::cout << "Prev conf: "<< searchType->confidence << "\t curr conf: " << conf << std::endl;
            searchType->dataField = field.getData();
            searchType->mrzDataField = bestField;
            searchType->fieldType = bestTypeField;
            searchType->confidence = conf;

            std::cout << "Associated with " << searchType->mrzDataField << std::endl;
        }        
    }
}

/*
void Fields::mostCompatibleOLD(Field & field, metricsType metricType)
{
    float currentComp = 0;
    std::string bestField = mrzGeneral.getAllFieldsInv().begin()->second;
    //std::string bestField = "None";
    if(metricType == pairs)
    {
        float maxComp = 0;
        for(auto mrzField: mrzGeneral.getAllFieldsInv())
        {
            std::cout << "Field: " << field.getData() << "   MRZ Field: " << mrzField.first;
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
            finalAssociation.emplace(bestField, std::make_pair(std::make_pair(field.getData(), mrzGeneral.getAllFields().find(bestField)->second), conf));
            
            association ass;
            ass.dataField = field.getData();
            ass.mrzDataField = search->first;
            ass.fieldType = search->second;
            ass.confidence = 1;
            finAss.push_back(ass);
            
            field.setTypeOfData(bestField);
            std::cout << "Associated with " << mrzGeneral.getAllFields().find(bestField)->second << std::endl;
        }
        else if(finalAssociation.find(bestField)->second.second < conf)
        {
            std::cout << "Prev conf: "<< finalAssociation.find(bestField)->second.second << "\t curr conf: " << conf << std::endl;
            finalAssociation.find(bestField)->second = std::make_pair(std::make_pair(field.getData(), mrzGeneral.getAllFields().find(bestField)->second), conf);
            std::cout << "Associated with " << mrzGeneral.getAllFields().find(bestField)->second << std::endl;
        }
    }
    else if(metricType == distLev)
    {
        float minComp = distanceLevenshtein(field.getData(), (*mrzGeneral.getAllFieldsInv().begin()).first);
        for(auto mrzField: mrzGeneral.getAllFieldsInv())
        {
            std::cout << "Field: " << field.getData() << "   MRZ Field: " << mrzField.first;
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
                std::cout << "Associated with " << mrzGeneral.getAllFields().find(bestField)->second << std::endl;
            }
            else if(finalAssociation.find(bestField)->second.second < conf)
            {
                std::cout << "Prev conf: "<< finalAssociation.find(bestField)->second.second << "\t curr conf: " << conf << std::endl;
                finalAssociation.find(bestField)->second = std::make_pair(std::make_pair(field.getData(), mrzGeneral.getAllFields().find(bestField)->second), conf);
                std::cout << "Associated with " << mrzGeneral.getAllFields().find(bestField)->second << std::endl;
            }
        }
        
    }
}
*/

void Fields::printNotFilledAndFilledFields()
{
    std::cout << "\nNot filled fields:" << std::endl;
    for(auto & field: fields)
        if(field.getTypeOfData() == "")
            std::cout << "None: " << field.getData() << std::endl;

    std::cout << std::endl;        
    
    std::cout << "Filled fields:" << std::endl;
    for(auto & field: fields)
        if(field.getTypeOfData() != "")
            std::cout << field.getTypeOfData() << ": " << field.getData() << std::endl;
    std::cout << std::endl;
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

/*
void Fields::computeConfFinalOLD()
{
    float sum = 0;
    for(auto & association: finalAssociation)
        sum += association.second.second;
    
    confFinal = sum / finalAssociation.size();

    if(confFinal < confThreshold)
        result = false;
}
*/