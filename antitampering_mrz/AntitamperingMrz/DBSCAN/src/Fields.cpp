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

std::vector<association> Fields::getDoubtfulFields()
{
    return doubtfulAss;
}


void Fields::fillFields()
{
    Field field;
    size_t countLower = 0;
    for(int i = 0; i <= numClusters; ++i)
    {
        countLower = 0;
        for(size_t j = 0; j < originalCluster.size(); ++j)
        {
            if(originalCluster[j].getCluster() == i)
            {
                field.setField(originalCluster[j].getX(), originalCluster[j]);
                if(originalCluster[j].getLabel() == "<")
                    ++countLower;
            }
        }
        if(countLower > 3) //the treshold of 3 "<" can be changed
        {
            field.setIsPartOfMRZ(true);
            ++numLineOfMRZ;
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
            splittedMRZ.emplace(field.getMeanY(), field);
            ++count;
        }
    }

    for(auto mrzLine: splittedMRZ)
        mrzChar.push_back(mrzLine.second.getCharacters());

    if(count > 1) //if MRZ is detected
    {
        MRZType type = findMRZType(mrzChar);
        MRZ* mrz;
        if(type != NONE)
        {
            mrz->createMRZ(type, mrzChar);
            mrz->extractFields();
            mrz->printMRZFields();
        }
        else
        {
            std::cout << "MRZ type not found." << std::endl;
            return false;
        }


        /*
        //std::cout << "\nMRZ type: "<< MRZtype << std::endl;
        if(MRZType == "TD1")
        {
            TD1 mrz(mrzChar);
            mrz.extractFields();
            mrz.printMRZFields();
            mrzGeneral = mrz;
        }
        else if(MRZType == "TD2")
        {
            TD2 mrz(mrzChar);
            mrz.extractFields();
            mrz.printMRZFields();
            mrzGeneral = mrz;
        }
        else if(MRZType == "TD3")
        {
            TD3 mrz(mrzChar);
            mrz.extractFields();
            mrz.printMRZFields();
            mrzGeneral = mrz;
        }
        else if(MRZType == "MRVA")
        {
            MRVA mrz(mrzChar);
            mrz.extractFields();
            mrz.printMRZFields();
            mrzGeneral = mrz;
        }
        else if(MRZType == "MRVB")
        {
            MRVB mrz(mrzChar);
            mrz.extractFields();
            mrz.printMRZFields();
            mrzGeneral = mrz;
        }
        else
        {
            std::cout << "MRZ type not found." << std::endl;
            return false;
        }
        

        std::cout << "\nMRZ:" << std::endl;
        mrzGeneral.printMRZ();
        //mrzGeneral.printMRZFields();
        */
    }
    else
    {
        std::cout << "MRZ not found." << std::endl;
        return false;
    }
    return true;   
}


MRZType Fields::findMRZType(std::vector<std::vector<Character>> mrz)
{
    std::string MRZtype;
    MRZType mrzType = NONE;
    if((numLineOfMRZ == 3) && (mrz[0].size() == 36) && (mrz[1].size() == 36) && (mrz[2].size() == 36))
    {
        MRZtype = "TD1";
        mrzType = td1;
    }
    else if((mrz[0].size() == 36) && (mrz[1].size() == 36)) // if(mrz[0].size() >= 32 && mrz[0].size() <= 40) less restrictive case
    {
        if(mrz[0][0].getLabel() == "P")
        {
            MRZtype = "TD2";
            mrzType = td2;
        }
        else
        {
            MRZtype = "MRVB";
            mrzType = mrvb;
        }
    }
    else if((mrz[0].size() == 44) && (mrz[1].size() == 44)) // if(mrz[0].size() > 40 && mrz[0].size() <= 48) less restrictive case
    {
        if(mrz[0][0].getLabel() == "P")
        {
            MRZtype = "TD3";
            mrzType = td3;
        }
        else
        {
            MRZtype = "MRVA";
            mrzType = mrva;
        }
    }
    else
        {
            MRZtype = "NULL";
            mrzType = NONE;
        }

    return mrzType;
}


void Fields::printOrderedFields()
{
    std::cout << "\nFields detected:" << std::endl;
    for(auto & field: fields)
        std::cout << field.getMeanY() << " " << field.getData() << " is part of MRZ: " << field.getIsPartOfMRZ() << std::endl;
    std::cout << std::endl;
}


void Fields::compareMRZFields(metricsType metric)
{
    /*
    std::vector<detection> mrzFields = mrzGeneral.getAllFields();
    float currentConf = 0, maxConf = 0;
    std::string bestTypeField, bestField;

    std::cout << "\nCompare fields:";

    for(auto & field: fields)
    {
        if(!field.getIsPartOfMRZ())
        {
            std::cout << "\nField: " << field.getData() << "\n" << std::endl;

            if((field.getData().size() >= 6) && (field.getData().size() <= 12))
                checkDate(field);

            maxConf = 0;

            std::cout << std::left << std::setw(20) << "MRZ FIELD";
            std::cout << std::left << std::setw(20) << "MRZ FIELD TYPE";
            std::cout << std::left << std::setw(20) << "CONFIDENCE" << std::endl;

            for(size_t itFields = 0; itFields < mrzFields.size(); ++itFields)
            {
                currentConf = computeConfidence(metric, field.getData(), mrzFields[itFields].fieldMRZ);

                std::cout << std::left << std::setw(20) << mrzFields[itFields].fieldMRZ;
                std::cout << std::left << std::setw(20) << mrzFields[itFields].typeFieldMRZ;
                std::cout << std::left << std::setw(20) << currentConf << std::endl;

                if(currentConf > maxConf)
                {
                    //std::cout << mrzFields[itFields].typeFieldMRZ << " new best " << std::endl;
                    maxConf = currentConf;
                    bestTypeField = mrzFields[itFields].typeFieldMRZ;
                    bestField = mrzFields[itFields].fieldMRZ;
                }
                if(currentConf == 1)
                {
                    //std::cout << "Field to remove: " << field.getData() << "   MRZ Field: " << mrzFields[itFields].fieldMRZ << "   MRZ Field type: " << mrzGeneral.getAllFields()[itFields].typeFieldMRZ << std::endl;
                    mrzFields.erase(mrzFields.begin()+itFields);
                    ++itFields;
                    break;
                }
            }
            std::cout << "Best confidence: " << maxConf << std::endl;

            if(!findField(field.getData(), bestField, bestTypeField, maxConf) && (maxConf > 0))
                addAssociation(field.getData(), bestField, bestTypeField, maxConf);
    
        }
        std::cout << std::endl;
    }
    numDoubtfulFields = doubtfulAss.size();
    computeConfFinal();
    */
}


void Fields::checkDate(Field & field)
{
    Date date(field.getData());
    
    // If it is a date, it is refactorized in format yymmdd.
    if(date.isDate())
        field.setData(date.getNewDate());
}


void Fields::addAssociation(std::string dataField, std::string& mrzDataField, std::string& fieldType, float confidence)
{
    association ass;
    ass.dataField = dataField;
    ass.mrzDataField = mrzDataField;
    ass.fieldType = fieldType;
    ass.confidence = confidence;

    if(confidence == 1)
    {
        std::cout << "Added in final association: " << dataField << " - " << mrzDataField << std::endl;
        finAss.push_back(ass);
    }
    else
    {
        std::cout << "Added in doubtful association: " << dataField << " - " << mrzDataField  << std::endl;
        doubtfulAss.push_back(ass);
    }
}


bool Fields::findField(std::string dataField, std::string& mrzDataField, std::string& fieldType, float confidence)
{
    bool found = false;
    for(size_t itDoubtFilds = 0; itDoubtFilds < doubtfulAss.size(); ++itDoubtFilds)
    {
        //std::cout << "Doubtful field type: "<< doubtfulAss[itDoubtFilds].fieldType << "\t conf: " << doubtfulAss[itDoubtFilds].confidence << std::endl;
        if((doubtfulAss[itDoubtFilds].fieldType == fieldType))
        {
            found = true;
            if(doubtfulAss[itDoubtFilds].confidence < confidence)
            {
                std::cout << "Prev conf: "<< doubtfulAss[itDoubtFilds].confidence << "\t curr conf: " << confidence << std::endl;
                //std::cout << "Field to remove: " << doubtfulAss[itDoubtFilds].dataField << "   MRZ Field: " << doubtfulAss[itDoubtFilds].mrzDataField << "   MRZ Field type: " << doubtfulAss[itDoubtFilds].fieldType << std::endl;
                doubtfulAss.erase(doubtfulAss.begin()+itDoubtFilds);
                ++itDoubtFilds;

                addAssociation(dataField, mrzDataField, fieldType, confidence);
                break;
            }
        }
    }
    return found;
}


void Fields::printAssociations()
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


void Fields::printDoubtfulFields()
{
    if(numDoubtfulFields != 0)
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
    else
        std::cout << "No doubtful fields" << std::endl;
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