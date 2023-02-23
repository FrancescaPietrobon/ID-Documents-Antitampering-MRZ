#include "MRVA.hpp"

MrzFields MRVA::extractMrzFields(std::vector<Fields> mrz)
{
    MrzFields mrzFields;

    // First line

    mrzFields.docType = mrz[0].label[0];
    if(mrz[0].label[1] != '<')
        mrzFields.docType = mrzFields.docType + mrz[0].label[1];

    mrzFields.state = "";
    mrzFields.state = mrzFields.state + mrz[0].label[2] + mrz[0].label[3] + mrz[0].label[4];

    int i = 5;
    for(size_t j = i; j < 44; ++j)
    {
        if(mrz[0].label[j] == '<' && mrz[0].label[j-1] == '<')
        {
            i = j + 1;
            break;
        }
        else if(mrz[0].label[j] == '<')
            mrzFields.surname += " ";
        else
            mrzFields.surname += mrz[0].label[j];
    }

    for(size_t j = i; j < 44; ++j)
    {
        if(mrz[0].label[j] == '<' && mrz[0].label[j-1] == '<')
            break;
        else if(mrz[0].label[j] == '<')
            mrzFields.name += " ";
        else
            mrzFields.name += mrz[0].label[j];
    }

    // Second line

    for(size_t i = 0; i < 9 && mrz[1].label[i] != '<'; ++i)
        mrzFields.docNumber += mrz[1].label[i];

    mrzFields.checkDocNum = mrz[1].label[9];
    
    mrzFields.nationality = "";
    mrzFields.nationality = mrzFields.nationality + mrz[1].label[10] + mrz[1].label[11] + mrz[1].label[12];

    for(size_t i = 13; i < 19; ++i)
        mrzFields.dateBirth += mrz[1].label[i];

    mrzFields.checkDateBirth = mrz[1].label[19];

    mrzFields.sex = mrz[1].label[20];

    for(size_t i = 21; i < 27; ++i)
        mrzFields.dateExpireDoc += mrz[1].label[i];

    mrzFields.checkDateExpireDoc = mrz[1].label[27];

    if(mrz[1].label[28] == '<')
        mrzFields.optionalData = "NULL";
    else
    {
        for(size_t i = 28; i < 44; ++i)
        {
            if(mrz[1].label[i] == '<' && mrz[1].label[i-1] == '<')
                break;
            else if(mrz[1].label[i] == '<')
                mrzFields.optionalData += " ";
            else
                mrzFields.optionalData += mrz[1].label[i];
        }
    }

    return mrzFields;
}

void MRVA::printMrzFields(MrzFields mrzFields)
{
    std::cout << "\nMRZ fields detected in MRVA MRZ:" << std::endl;
    std::cout << "Document type: " << mrzFields.docType << std::endl;
    std::cout << "State: " << mrzFields.state << std::endl;
    std::cout << "Surname: " << mrzFields.surname << std::endl;
    std::cout << "Name: " << mrzFields.name << std::endl;
    std::cout << "Document number: " << mrzFields.docNumber << std::endl;
    std::cout << "Check document number: " << mrzFields.checkDocNum << std::endl;
    std::cout << "Nationality: " << mrzFields.nationality << std::endl;
    std::cout << "Date of birth: " << mrzFields.dateBirth << std::endl;
    std::cout << "Check date of birth: " << mrzFields.checkDateBirth << std::endl;
    std::cout << "Sex: " << mrzFields.sex << std::endl;
    std::cout << "Date of expire: " << mrzFields.dateExpireDoc << std::endl;
    std::cout << "Check date of expire: " << mrzFields.checkDateExpireDoc << std::endl;
    std::cout << "Optional data: " << mrzFields.optionalData << std::endl;
}