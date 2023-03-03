#include "TD1.hpp"

std::vector<MrzField> TD1::extractMrzFields(std::vector<Field> mrz)
{
    std::vector<MrzField> mrzFields;
    MrzField field;

    // First line
    
    field.fieldType = "docType";
    field.mrzDataField = mrz[0].label[0];
    if(mrz[0].label[1] != '<')
        field.mrzDataField += mrz[0].label[1];
    mrzFields.push_back(field);

    field.fieldType = "state";
    field.mrzDataField = "";
    field.mrzDataField = field.mrzDataField + mrz[0].label[2] + mrz[0].label[3] + mrz[0].label[4];
    mrzFields.push_back(field);

    field.fieldType = "docNumber";
    field.mrzDataField = "";
    for(size_t i = 5; i < 14 && mrz[0].label[i] != '<'; ++i)
        field.mrzDataField += mrz[0].label[i];
    mrzFields.push_back(field);

    checkDocNum = mrz[0].label[14];

    if(mrz[0].label[15] != '<')
    {
        for(size_t i = 15; i < 29 && mrz[0].label[i] != '<'; ++i)
            optionalData += mrz[0].label[i];

        checkOptionalData += mrz[0].label[29];
    }

    // Second line

    field.fieldType = "dateBirth";
    field.mrzDataField = "";
    for(size_t i = 0; i < 6; ++i)
        field.mrzDataField += mrz[1].label[i];
    mrzFields.push_back(field);

    checkDateBirth = mrz[1].label[6];

    field.fieldType = "sex";
    field.mrzDataField = mrz[1].label[7];
    mrzFields.push_back(field);

    field.fieldType = "dateExpireDoc";
    field.mrzDataField = "";
    for(size_t i = 8; i < 14; ++i)
        field.mrzDataField += mrz[1].label[i];
    mrzFields.push_back(field);

    checkDateExpireDoc = mrz[1].label[14];

    field.fieldType = "nationality";
    field.mrzDataField = "";
    field.mrzDataField = field.mrzDataField + mrz[1].label[15] + mrz[1].label[16] + mrz[1].label[17];
    mrzFields.push_back(field);

    if(mrz[1].label[18] != '<')
    {
        for(size_t i = 18; i < 29 && mrz[1].label[i] != '<'; ++i)
            secondOptionalData += mrz[1].label[i];
    }

    checkFirstTwoLines = mrz[1].label[29];

    // Third line

    field.fieldType = "surname";
    field.mrzDataField = "";
    int i = 0;
    for(size_t j = i; j < 30; ++j)
    {
        if(mrz[2].label[j] == '<' && mrz[2].label[j-1] == '<')
        {
            i = j + 1;
            break;
        }
        else if(mrz[2].label[j] == '<')
            field.mrzDataField += " ";
        else
            field.mrzDataField += mrz[2].label[j];
    }
    mrzFields.push_back(field);

    field.fieldType = "name";
    field.mrzDataField = "";
    for(size_t j = i; j < 30; ++j)
    {
        if(mrz[2].label[j] == '<' && mrz[2].label[j-1] == '<')
            break;
        else if(mrz[2].label[j] == '<')
            field.mrzDataField += " ";
        else
            field.mrzDataField += mrz[2].label[j];
    }
    mrzFields.push_back(field);

    return mrzFields;
}

bool TD1::checkDigits(std::vector<Field> mrz, std::vector<MrzField> mrzFields)
{
    bool result = true;

    if(!check(mrzFields.at(2).mrzDataField, checkDocNum))
    {
        SPDLOG_DEBUG("Check in document number faild.");
        result = false;
    }  
    else
        SPDLOG_DEBUG("Check in document number OK.");

    if(!check(optionalData, checkOptionalData))
    {
        SPDLOG_DEBUG("Check in optional data faild.");
        result = false;
    }
    else
        SPDLOG_DEBUG("Check in optional data OK.");

    if(!check(mrzFields.at(3).mrzDataField, checkDateBirth))
    {
        SPDLOG_DEBUG("Check in date of birth faild.");
        result = false;
    }
    else
        SPDLOG_DEBUG("Check in date of birth OK.");

    if(!check(mrzFields.at(5).mrzDataField, checkDateExpireDoc))
    {
        SPDLOG_DEBUG("Check in date of expire faild.");
        result = false;
    }
    else
        SPDLOG_DEBUG("Check in date of expire OK.");

    // MISSING CHECK FIRST TWO LINES

    return result;
}