#include "MRVA.hpp"

std::vector<MrzField> MRVA::extractMrzFields(std::vector<Field> mrz)
{
    std::vector<MrzField> mrzFields;

    // First line
    mrzFields = this->extractDocType(mrz, mrzFields);
    mrzFields = this->extractState(mrz, mrzFields);
    mrzFields = this->extractSurnameAndName(mrz, mrzFields);

    // Second line
    mrzFields = this->extractDocNumber(mrz, mrzFields);
    this->checkDocNum = mrz[1].label[9];
    mrzFields = this->extractNationality(mrz, mrzFields);
    mrzFields = this->extractDateBirth(mrz, mrzFields);
    this->checkDateBirth = mrz[1].label[19];
    mrzFields = this->extractSex(mrz, mrzFields);
    mrzFields = this->extractDateExpireDoc(mrz, mrzFields);
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

std::vector<MrzField> MRVA::extractDocType(std::vector<Field> mrz, std::vector<MrzField> &mrzFields)
{
    MrzField field;
    field.fieldType = "docType";
    field.mrzDataField = mrz[0].label[0];
    if(mrz[0].label[1] != '<')
        field.mrzDataField += + mrz[0].label[1];
    mrzFields.push_back(field);
    return mrzFields;
}

std::vector<MrzField> MRVA::extractState(std::vector<Field> mrz, std::vector<MrzField> &mrzFields)
{
    MrzField field;
    field.fieldType = "state";
    field.mrzDataField = "";
    field.mrzDataField = field.mrzDataField + mrz[0].label[2] + mrz[0].label[3] + mrz[0].label[4];
    mrzFields.push_back(field);
    return mrzFields;
}

std::vector<MrzField> MRVA::extractSurnameAndName(std::vector<Field> mrz, std::vector<MrzField> &mrzFields)
{
    MrzField field;
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
    return mrzFields;
}

std::vector<MrzField> MRVA::extractDocNumber(std::vector<Field> mrz, std::vector<MrzField> &mrzFields)
{
    MrzField field;
    field.fieldType = "docNumber";
    field.mrzDataField = "";
    for(size_t i = 0; i < 9 && mrz[1].label[i] != '<'; ++i)
        field.mrzDataField += mrz[1].label[i];
    mrzFields.push_back(field);
    return mrzFields;
}

std::vector<MrzField> MRVA::extractNationality(std::vector<Field> mrz, std::vector<MrzField> &mrzFields)
{
    MrzField field;
    field.fieldType = "nationality";
    field.mrzDataField = "";
    field.mrzDataField = field.mrzDataField + mrz[1].label[10] + mrz[1].label[11] + mrz[1].label[12];
    mrzFields.push_back(field);
    return mrzFields;
}

std::vector<MrzField> MRVA::extractDateBirth(std::vector<Field> mrz, std::vector<MrzField> &mrzFields)
{
    MrzField field;
    field.fieldType = "dateBirth";
    field.mrzDataField = "";
    for(size_t i = 13; i < 19; ++i)
        field.mrzDataField += mrz[1].label[i];
    mrzFields.push_back(field);
    return mrzFields;
}

std::vector<MrzField> MRVA::extractSex(std::vector<Field> mrz, std::vector<MrzField> &mrzFields)
{
    MrzField field;
    field.fieldType = "sex";
    field.mrzDataField = mrz[1].label[20];
    mrzFields.push_back(field);
    return mrzFields;
}

std::vector<MrzField> MRVA::extractDateExpireDoc(std::vector<Field> mrz, std::vector<MrzField> &mrzFields)
{
    MrzField field;
    field.fieldType = "dateExpireDoc";
    field.mrzDataField = "";
    for(size_t i = 21; i < 27; ++i)
        field.mrzDataField += mrz[1].label[i];
    mrzFields.push_back(field);
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
