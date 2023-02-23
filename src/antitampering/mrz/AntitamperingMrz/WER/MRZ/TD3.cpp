#include "TD3.hpp"

MrzFields TD3::extractMrzFields(std::vector<Fields> mrz)
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
    mrzFields.surname = mrzFields.surname.substr(0, mrzFields.surname.size() - 1);

    for(size_t j = i; j < 44; ++j)
    {
        if(mrz[0].label[j] == '<' && mrz[0].label[j-1] == '<')
            break;
        else if(mrz[0].label[j] == '<')
            mrzFields.name += " ";
        else
            mrzFields.name += mrz[0].label[j];
    }
    mrzFields.name = mrzFields.name.substr(0, mrzFields.name.size() - 1);

    // Second line

    for(size_t i = 0; i < 9 && mrz[1].label[i] != '<'; ++i)
        mrzFields.docNumber += mrz[1].label[i];

    mrzFields.checkDocNum = mrz[1].label[9];

    if(!check(mrzFields.docNumber, mrzFields.checkDocNum))
        std::cout << "Check in document number faild." << std::endl;
    else
        std::cout << "Check in document number OK." << std::endl;
    
    mrzFields.nationality = "";
    mrzFields.nationality = mrzFields.nationality + mrz[1].label[10] + mrz[1].label[11] + mrz[1].label[12];

    for(size_t i = 13; i < 19; ++i)
        mrzFields.dateBirth += mrz[1].label[i];

    mrzFields.checkDateBirth = mrz[1].label[19];

    if(!check(mrzFields.dateBirth, mrzFields.checkDateBirth))
        std::cout << "Check in date of birth faild." << std::endl;
    else
        std::cout << "Check in date of birth OK." << std::endl;

    mrzFields.sex = mrz[1].label[20];

    for(size_t i = 21; i < 27; ++i)
        mrzFields.dateExpireDoc += mrz[1].label[i];

    mrzFields.checkDateExpireDoc = mrz[1].label[27];

    if(!check(mrzFields.dateExpireDoc, mrzFields.checkDateExpireDoc))
        std::cout << "Check in date of expire faild." << std::endl;
    else
        std::cout << "Check in date of expire OK." << std::endl;

    if(mrz[1].label[28] == '<')
    {
        mrzFields.optionalData = "NULL";
        checkOptionalData = "NULL";
    }
    else
    {
        for(size_t i = 28; i < 42 && mrz[1].label[i] != '<'; ++i)
            mrzFields.optionalData += mrz[1].label[i];

        checkOptionalData = mrz[1].label[42];

        if(!check(mrzFields.optionalData, checkOptionalData))
            std::cout << "Check in optional data faild." << std::endl;
        else
            std::cout << "Check in optional data OK." << std::endl;
    }

    checkOverallDigit = mrz[1].label[43];

    if(!checkOverall(mrz, checkOverallDigit))
        std::cout << "Check overall faild." << std::endl;
    else
        std::cout << "Check overall OK." << std::endl;

    return mrzFields;
}

void TD3::printMrzFields(MrzFields mrzFields)
{
    std::cout << "\nMRZ fields detected in TD3 MRZ:" << std::endl;
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
    std::cout << "Check optional data: " << checkOptionalData << std::endl;
    std::cout << "Check overall: " << checkOverallDigit << std::endl;
}