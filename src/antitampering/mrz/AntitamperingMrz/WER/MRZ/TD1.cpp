#include "TD1.hpp"

MrzFields TD1::extractMrzFields(std::vector<Fields> mrz)
{
    MrzFields mrzFields;

    // First line

    mrzFields.docType = mrz[0].label[0];
    if(mrz[0].label[1] != '<')
       mrzFields.docType = mrzFields.docType + mrz[0].label[1];

    mrzFields.state = "";
    mrzFields.state = mrzFields.state + mrz[0].label[2] + mrz[0].label[3] + mrz[0].label[4];

    for(size_t i = 5; i < 14 && mrz[0].label[i] != '<'; ++i)
        mrzFields.docNumber += mrz[0].label[i];

    mrzFields.checkDocNum = mrz[0].label[14];

    if(mrz[0].label[15] == '<')
    {
        mrzFields.optionalData = "NULL";
        checkOptionalData = "NULL";
    }
    else
    {
        for(size_t i = 15; i < 29 && mrz[0].label[i] != '<'; ++i)
            mrzFields.optionalData += mrz[0].label[i];

        checkOptionalData = mrz[0].label[29];
    }

    // Second line

    for(size_t i = 0; i < 6; ++i)
        mrzFields.dateBirth += mrz[1].label[i];

    mrzFields.checkDateBirth = mrz[1].label[6];

    mrzFields.sex = mrz[1].label[7];

    for(size_t i = 8; i < 14; ++i)
        mrzFields.dateExpireDoc += mrz[1].label[i];

    mrzFields.checkDateExpireDoc = mrz[1].label[14];

    mrzFields.nationality = "";
    mrzFields.nationality = mrzFields.nationality + mrz[1].label[15] + mrz[1].label[16] + mrz[1].label[17];

    if(mrz[1].label[18] == '<')
        secondOptionalData = "NULL";
    else
    {
        for(size_t i = 18; i < 29 && mrz[1].label[i] != '<'; ++i)
            secondOptionalData += mrz[1].label[i];
    }

    checkFirstTwoLines = mrz[1].label[29];

    // Third line

    int i = 0;
    for(size_t j = i; j < 30; ++j)
    {
        if(mrz[2].label[j] == '<' && mrz[2].label[j-1] == '<')
        {
            i = j + 1;
            break;
        }
        else if(mrz[2].label[j] == '<')
            mrzFields.surname += " ";
        else
            mrzFields.surname += mrz[2].label[j];
    }

    for(size_t j = i; j < 30; ++j)
    {
        if(mrz[2].label[j] == '<' && mrz[2].label[j-1] == '<')
            break;
        else if(mrz[2].label[j] == '<')
            mrzFields.name += " ";
        else
            mrzFields.name += mrz[2].label[j];
    }

    return mrzFields;
}


void TD1::printMrzFields(MrzFields mrzFields)
{
    std::cout << "\nMRZ fields detected in TD1 MRZ:" << std::endl;
    std::cout << "Document type: " << mrzFields.docType << std::endl;
    std::cout << "State: " << mrzFields.state << std::endl;
    std::cout << "Document number: " << mrzFields.docNumber << std::endl;
    std::cout << "Check document number: " << mrzFields.checkDocNum << std::endl;
    std::cout << "Optional data: " << mrzFields.optionalData << std::endl;
    std::cout << "Check optional data: " << checkOptionalData << std::endl;
    std::cout << "Date of birth: " << mrzFields.dateBirth << std::endl;
    std::cout << "Check date of birth: " << mrzFields.checkDateBirth << std::endl;
    std::cout << "Sex: " << mrzFields.sex << std::endl;
    std::cout << "Date of expire: " << mrzFields.dateExpireDoc << std::endl;
    std::cout << "Check date of expire: " << mrzFields.checkDateExpireDoc << std::endl;
    std::cout << "Nationality: " << mrzFields.nationality << std::endl;
    std::cout << "Second optional data: " << secondOptionalData << std::endl;
    std::cout << "Check first two lines: " << checkFirstTwoLines << std::endl;
    std::cout << "Surname: " << mrzFields.surname << std::endl;
    std::cout << "Name: " << mrzFields.name << std::endl;  
}