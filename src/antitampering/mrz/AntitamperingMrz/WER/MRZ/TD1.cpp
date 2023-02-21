#include "TD1.hpp"


TD1::TD1(std::vector<Fields> mrz):
    Mrz(mrz){}


void TD1::extractFields(std::vector<Fields> mrz)
{
    // First line

    docType = mrz[0].label[0];
    if(mrz[0].label[1] != '<')
        docType = docType + mrz[0].label[1];
    //det.fieldMRZ = docType;
    //det.typeFieldMRZ = "docType";
    //allFields.push_back(det);

    state = mrz[0].label[2] + mrz[0].label[3] + mrz[0].label[4];
    //det.fieldMRZ = state;
    //det.typeFieldMRZ = "state";
    //allFields.push_back(det);

    for(size_t i = 5; i < 14 && mrz[0].label[i] != '<'; ++i)
        docNumber += mrz[0].label[i];
    //det.fieldMRZ = docNumber;
    //det.typeFieldMRZ = "docNumber";
    //allFields.push_back(det);

    checkDocNum = mrz[0].label[14];

    if(mrz[0].label[15] == '<')
    {
        optionalData = "NULL";
        checkOptionalData = "NULL";
    }
    else
    {
        for(size_t i = 15; i < 29 && mrz[0].label[i] != '<'; ++i)
            optionalData += mrz[0].label[i];

        checkOptionalData = mrz[0].label[29];
    }

    // Second line

    for(size_t i = 0; i < 6; ++i)
        dateBirth += mrz[1].label[i];
    //det.fieldMRZ = dateBirth;
    //det.typeFieldMRZ = "dateBirth";
    //allFields.push_back(det);

    checkDateBirth = mrz[1].label[6];

    sex = mrz[1].label[7];
    //det.fieldMRZ = sex;
    //det.typeFieldMRZ = "sex";
    //allFields.push_back(det);

    for(size_t i = 8; i < 14; ++i)
        dateExpireDoc += mrz[1].label[i];
    //det.fieldMRZ = dateExpireDoc;
    //det.typeFieldMRZ = "dateExpireDoc";
    //allFields.push_back(det);

    checkDateExpireDoc = mrz[1].label[14];

    nationality = mrz[1].label[15] + mrz[1].label[16] + mrz[1].label[17];
    //det.fieldMRZ = nationality;
    //det.typeFieldMRZ = "nationality";
    //allFields.push_back(det);

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
            surname += " ";
        else
            surname += mrz[2].label[j];
    }
    //det.fieldMRZ = surname;
    //det.typeFieldMRZ = "surname";
    //allFields.push_back(det);

    for(size_t j = i; j < 30; ++j)
    {
        if(mrz[2].label[j] == '<' && mrz[2].label[j-1] == '<')
            break;
        else if(mrz[2].label[j] == '<')
            name += " ";
        else
            name += mrz[2].label[j];
    }
    //det.fieldMRZ = name;
    //det.typeFieldMRZ = "name";
    //allFields.push_back(det);
}


void TD1::printMRZFields()
{
    std::cout << "\nMRZ fields detected in TD1 MRZ:" << std::endl;
    std::cout << "Document type: " << docType << std::endl;
    std::cout << "State: " << state << std::endl;
    std::cout << "Document number: " << docNumber << std::endl;
    std::cout << "Check document number: " << checkDocNum << std::endl;
    std::cout << "Optional data: " << optionalData << std::endl;
    std::cout << "Check optional data: " << checkOptionalData << std::endl;
    std::cout << "Date of birth: " << dateBirth << std::endl;
    std::cout << "Check date of birth: " << checkDateBirth << std::endl;
    std::cout << "Sex: " << sex << std::endl;
    std::cout << "Date of expire: " << dateExpireDoc << std::endl;
    std::cout << "Check date of expire: " << checkDateExpireDoc << std::endl;
    std::cout << "Nationality: " << nationality << std::endl;
    std::cout << "Second optional data: " << secondOptionalData << std::endl;
    std::cout << "Check first two lines: " << checkFirstTwoLines << std::endl;
    std::cout << "Surname: " << surname << std::endl;
    std::cout << "Name: " << name << std::endl;  
}