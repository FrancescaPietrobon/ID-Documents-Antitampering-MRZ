#include "../include/Fields.h"

Fields::Fields(std::vector<Character> chars, int n):
    originalCluster(chars), numClusters(n)
    {
        MRZ mrz;
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
        fields.push_back(field);
        field.clear();
    }
    
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

        // TO DO: create ENUM for mrz type
        if(MRZType == "TD3")
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
        else if(MRZType == "TD1")
        {
            TD1 mrz(mrzChar, numLineOfMRZ);
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
    else if(mrz[0].size() == 36)
    {
        if(mrz[0][0].getLabel() == "V")
            MRZType = "MRVB";
        else
            MRZType = "TD2";
    }
    else if(mrz[0].size() == 44)
    {
        if(mrz[0][0].getLabel() == "V")
            MRZType = "MRVA";
        else
            MRZType = "TD3";
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


void checkAlphanumDate(Field & field)
{
    size_t countNotDigit = 0, countDigit = 0;
    //std::cout << "Field: " << field.getData() << std::endl;

    const char * text = field.getData().c_str();
    std::string possibleMonth;
    size_t idxData = 0;
    for(size_t i = 0; text[i] != NULL; ++i)
    {
        if(!std::isdigit(text[i]) && countNotDigit <= 3)
        {   
            idxData = i;
            ++countNotDigit;
            possibleMonth += text[i];
        }
        else
            ++countDigit;
    }

    if(countNotDigit != 0 && countDigit >= 3)
    {
        //std::cout << "Possible date: " << field.getData();
        //std::cout << "   with possible month: " << possibleMonth << std::endl;

        Date date(field.getData(), possibleMonth, idxData);
        std::string newDate = date.convertAlphanumDate();
        //std::cout << "New date: " << newDate << std::endl; 
        field.setData(newDate);
    }  
}

void Fields::printNotFilledAndFilledFields()
{
    std::cout << "Not filled fields:" << std::endl;
    for(auto & field: fields)
        if(field.getTypeOfData() == "None")
            std::cout << field.getTypeOfData() << ": " << field.getData() << std::endl;

    std::cout << std::endl;        
    
    std::cout << "Filled fields:" << std::endl;
    for(auto & field: fields)
        if(field.getTypeOfData() != "None")
            std::cout << field.getTypeOfData() << ": " << field.getData() << std::endl;
}


void Fields::compareMRZFields()
{
    //OCCHIO CHE SE HO SPECIMEN sia per nome che cognome mi dirà entrambe le volte SPECIMEN è cognome (primo che vede)
    // Then check for identical field
    for(auto & field: fields)
    {
        if(field.getData().size() >= 6 && field.getData().size() <= 9)
            checkAlphanumDate(field);
        auto search = mrzGeneral.getAllFieldsInv().find(field.getData());
        //std::cout << "Field: " << field.getData() << std::endl;
        if((field.getData() == search->first) && (field.getTypeOfData() == ""))
        {
            //std::cout << "Field: " << field.getData() << " ";
            //std::cout << " Find: " << search->first << " " << search->second << std::endl;
            field.setTypeOfData(search->second);
        }
        else
        {
            std::string bestField = mostCompatible(field);
            field.setTypeOfData(bestField);
        }
        // TO DO: gestisci casi con dato già settato a un valore!!
         
    }
    //std::cout << std::endl;
    

}

std::string Fields::mostCompatible(Field field)
{
    int maxComp = 0, currentComp = 0;
    std::string bestField = mrzGeneral.getAllFieldsInv().begin()->first;
    //std::cout << "first field: " << bestField << std::endl;
    for(auto mrzField: mrzGeneral.getAllFieldsInv())
    {
        //std::cout << "Field: " << field.getData() << "   MRZ Field: " << mrzField.first;
        currentComp = countPairs(field.getData(), mrzField.first);
        //std::cout << "   Comp: " << currentComp << std::endl;
        if(currentComp > maxComp)
        {
            maxComp = currentComp;
            bestField = mrzField.second;
        }
    }

    if(maxComp <= field.getData().size() * 0.83)
        bestField = "None";
    return bestField;
}



/*

void Fields::fillFields()
{
    Field field(originalCluster);
    //Potrei dichiarare fields qui se poi non la uso ma uso sempre orderedFields
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

    for(auto & fieldScan: fields)
    {
        fieldScan.second.computeMeanY();
        orderedFields.push_back(fieldScan.second);
    }
}


void Fields::printOrderedFields()
{
    for(auto & field: fields)
    {
        std::cout << field.first << " " << field.second.getMeanY() << " ";
        std::string data;
        for(auto & character: field.second.getField())
            data += character.second.getLabel();
        
        field.second.setData(data);
        std::cout << field.second.getData() << " is part of MRZ: " << (field.second).getIsPartOfMRZ() << std::endl;
        //field.second.print();
        //if(mrzGeneral.getAllFieldsInv().find(field.second.getData()) != mrzGeneral.getAllFieldsInv().end())
        //    std::cout << "Field: " << field.second.getData() << " " << mrzGeneral.getAllFieldsInv().find(field.second.getData())->second << std::endl;

    }

}





*/





/*
void Fields::checkMRZ()
{
    int count = 0;
    //std::map<float, Field> splittedMRZ;
    std::vector<std::vector<Character>> mrzChar;
    // Save in splittedMRZ all the mrz lines ordered wrt the mean y coordinate
    for(auto & field: fields)
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

    std::cout << "Fine primo for" << std::endl;

    for(auto mrzLine: splittedMRZ)
        mrzChar.push_back(mrzLine.second.getCharacters());

    std::cout << "Fine second for" << std::endl;
    if(count > 1)
    {
        std::string MRZType = findMRZType(mrzChar);

        std::cout << "Fine findMRZType" << std::endl;

        std::cout << "MRZ type: "<< MRZType << std::endl;
        //MRZ mrzGeneral(mrzChar, numLineOfMRZ);

        if(MRZType == "TD3")
        {
            TD3 mrz(mrzChar, numLineOfMRZ);
            mrz.extractFields();
            //mrz.printMRZFields(); così printerei anche i campi che ha solo il TD3, con la funzione generica invece solo i valori standard
            mrzGeneral = mrz;
        }
        else if(MRZType == "MRVA")
        {
            MRVA mrz(mrzChar, numLineOfMRZ);
            mrz.extractFields();
            //mrz.printMRZFields();
            mrzGeneral = mrz;
        }
        else if(MRZType == "TD1")
        {
            TD1 mrz(mrzChar, numLineOfMRZ);
            //mrz.printMRZ();
            mrz.extractFields();
            mrzGeneral = mrz;
        }
        
        std::cout << std::endl;
        std::cout << "MRZ:" << std::endl;
        mrzGeneral.printMRZ();
        std::cout << std::endl;
        mrzGeneral.printMRZFields();
    }
    
}

void Fields::printMRZAllFieldsInv()
{
    for(auto allMrz: mrzGeneral.getAllFieldsInv())
    {
        std::cout << allMrz.first << " " << allMrz.second << std::endl;
    }
}

void Fields::compareMRZFields()
{
    //OCCHIO CHE SE HO SPECIMEN sia per nome che cognome mi dirà entrambe le volte SPECIMEN è cognome (primo che vede)
    for(auto & fieldIter: fields)
    {
        auto search = mrzGeneral.getAllFieldsInv().find(fieldIter.second.getData());
        std::cout << "Field: " << fieldIter.second.getData() << std::endl;
        if((fieldIter.second.getData() == search->first) && (fieldIter.second.getTypeOfData() == ""))
        {
            std::cout << search->first << " " << search->second << " ";
            fieldIter.second.setTypeOfData(search->first);
            
            std::cout << fieldIter.second.getTypeOfData() << std::endl;
        }        
    }

    //scorri prima per mrz fields e cerca i field sparsi
    
    for(auto mrzField: mrzGeneral.getAllFieldsInv())
    {
        
        mrzField.second.find()
    }
    
}

/*


void Fields::searchField(std::string data)
{
    
    for(auto field: fields)
    {
        if(field.second.getTypeOfData() == "")
        {
            if(field.second.getData() == data)
        }
        
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

