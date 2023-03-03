#include "MRVA.hpp"

std::vector<MrzField> MRVA::extractMrzFields(std::vector<Field> mrz)
{
    std::vector<MrzField> mrzFields;
    MrzField field;

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

    this->checkDocNum = mrz[1].label[9];
    
    field.fieldType = "nationality";
    field.mrzDataField = "";
    field.mrzDataField = field.mrzDataField + mrz[1].label[10] + mrz[1].label[11] + mrz[1].label[12];
    mrzFields.push_back(field);

    field.fieldType = "dateBirth";
    field.mrzDataField = "";
    for(size_t i = 13; i < 19; ++i)
        field.mrzDataField += mrz[1].label[i];
    mrzFields.push_back(field);

    this->checkDateBirth = mrz[1].label[19];

    field.fieldType = "sex";
    field.mrzDataField = mrz[1].label[20];
    mrzFields.push_back(field);

    field.fieldType = "dateExpireDoc";
    field.mrzDataField = "";
    for(size_t i = 21; i < 27; ++i)
        field.mrzDataField += mrz[1].label[i];
    mrzFields.push_back(field);

    this->checkDateExpireDoc = mrz[1].label[27];

    if(mrz[1].label[28] != '<')
    {
        for(size_t i = 28; i < 44; ++i)
        {
            if(mrz[1].label[i] == '<' && mrz[1].label[i-1] == '<')
                break;
            else if(mrz[1].label[i] == '<')
                this->optionalData += " ";
            else
                this->optionalData += mrz[1].label[i];
        }
    }

    return mrzFields;
}

bool MRVA::checkDigits(std::vector<Field> mrz, std::vector<MrzField> mrzFields)
{
    bool result = true;

    if(!this->check(mrzFields.at(4).mrzDataField, this->checkDocNum))
    {
        SPDLOG_DEBUG("Check in document number faild.");
        result = false;
    }  
    else
        SPDLOG_DEBUG("Check in document number OK.");

    if(!this->check(mrzFields.at(6).mrzDataField, this->checkDateBirth))
    {
        SPDLOG_DEBUG("Check in date of birth faild.");
        result = false;
    }
    else
        SPDLOG_DEBUG("Check in date of birth OK.");

    if(!this->check(mrzFields.at(8).mrzDataField, this->checkDateExpireDoc))
    {
        SPDLOG_DEBUG("Check in date of expire faild.");
        result = false;
    }
    else
        SPDLOG_DEBUG("Check in date of expire OK.");

    return result;
}