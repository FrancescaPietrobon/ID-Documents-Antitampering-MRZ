#include "MRVA.hpp"

std::vector<MrzFields> MRVA::extractMrzFields(std::vector<Fields> mrz)
{
    std::vector<MrzFields> mrzFields;
    MrzFields field;

    // First line
    
    field.fieldType = "docType";
    field.mrzDataField = mrz[0].label[0];
    if(mrz[0].label[1] != '<')
        field.mrzDataField += + mrz[0].label[1];
    mrzFields.push_back(field);

    field.fieldType = "state";
    field.mrzDataField = "";
    field.mrzDataField = field.mrzDataField + mrz[0].label[2] + mrz[0].label[3] + mrz[0].label[4];
    mrzFields.push_back(field);

    field.fieldType = "surname";
    field.mrzDataField = "";
    int i = 5;
    for(size_t j = i; j < 44; ++j)
    {
        if(mrz[0].label[j] == '<' && mrz[0].label[j-1] == '<')
        {
            i = j + 1;
            break;
        }
        else if(mrz[0].label[j] == '<')
            field.mrzDataField += " ";
        else
            field.mrzDataField += mrz[0].label[j];
    }
    mrzFields.push_back(field);

    field.fieldType = "name";
    field.mrzDataField = "";
    for(size_t j = i; j < 44; ++j)
    {
        if(mrz[0].label[j] == '<' && mrz[0].label[j-1] == '<')
            break;
        else if(mrz[0].label[j] == '<')
            field.mrzDataField += " ";
        else
            field.mrzDataField += mrz[0].label[j];
    }
    mrzFields.push_back(field);

    // Second line

    field.fieldType = "docNumber";
    field.mrzDataField = "";
    for(size_t i = 0; i < 9 && mrz[1].label[i] != '<'; ++i)
        field.mrzDataField += mrz[1].label[i];
    mrzFields.push_back(field);

    checkDocNum = mrz[1].label[9];
    
    field.fieldType = "nationality";
    field.mrzDataField = "";
    field.mrzDataField = field.mrzDataField + mrz[1].label[10] + mrz[1].label[11] + mrz[1].label[12];
    mrzFields.push_back(field);

    field.fieldType = "dateBirth";
    field.mrzDataField = "";
    for(size_t i = 13; i < 19; ++i)
        field.mrzDataField += mrz[1].label[i];
    mrzFields.push_back(field);

    checkDateBirth = mrz[1].label[19];

    field.fieldType = "sex";
    field.mrzDataField = mrz[1].label[20];
    mrzFields.push_back(field);

    field.fieldType = "dateExpireDoc";
    field.mrzDataField = "";
    for(size_t i = 21; i < 27; ++i)
        field.mrzDataField += mrz[1].label[i];
    mrzFields.push_back(field);

    checkDateExpireDoc = mrz[1].label[27];

    if(mrz[1].label[28] != '<')
    {
        for(size_t i = 28; i < 44; ++i)
        {
            if(mrz[1].label[i] == '<' && mrz[1].label[i-1] == '<')
                break;
            else if(mrz[1].label[i] == '<')
                optionalData += " ";
            else
                optionalData += mrz[1].label[i];
        }
    }

    return mrzFields;
}

bool MRVA::checkDigits(std::vector<Fields> mrz, std::vector<MrzFields> mrzFields)
{
    bool result = true;

    if(!check(mrzFields.at(4).mrzDataField, checkDocNum))
    {
        std::cout << "Check in document number faild." << std::endl;
        result = false;
    }  
    else
        std::cout << "Check in document number OK." << std::endl;

    if(!check(mrzFields.at(6).mrzDataField, checkDateBirth))
    {
        std::cout << "Check in date of birth faild." << std::endl;
        result = false;
    }
    else
        std::cout << "Check in date of birth OK." << std::endl;

    if(!check(mrzFields.at(8).mrzDataField, checkDateExpireDoc))
    {
        std::cout << "Check in date of expire faild." << std::endl;
        result = false;
    }
    else
        std::cout << "Check in date of expire OK." << std::endl;

    return result;
}