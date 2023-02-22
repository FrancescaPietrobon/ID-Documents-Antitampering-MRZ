#include "TD3.hpp"

TD3::TD3(std::vector<Fields> mrz):
    Mrz(mrz){}


void TD3::extractFields(std::vector<Fields> mrz)
{
    // First line

    docType = mrz[0].label[0];
    if(mrz[0].label[1] != '<')
        docType = docType + mrz[0].label[1];
    //det.fieldMRZ = docType;
    //det.typeFieldMRZ = "docType";
    //allFields.push_back(det);

    state = "";
    state = state + mrz[0].label[2] + mrz[0].label[3] + mrz[0].label[4];
    //det.fieldMRZ = state;
    //det.typeFieldMRZ = "state";
    //allFields.push_back(det);

    int i = 5;
    for(size_t j = i; j < 44; ++j)
    {
        if(mrz[0].label[j] == '<' && mrz[0].label[j-1] == '<')
        {
            i = j + 1;
            break;
        }
        else if(mrz[0].label[j] == '<')
            surname += " ";
        else
            surname += mrz[0].label[j];
    }
    surname = surname.substr(0, surname.size() - 1);
    //det.fieldMRZ = surname;
    //det.typeFieldMRZ = "surname";
    //allFields.push_back(det);

    for(size_t j = i; j < 44; ++j)
    {
        if(mrz[0].label[j] == '<' && mrz[0].label[j-1] == '<')
            break;
        else if(mrz[0].label[j] == '<')
            name += " ";
        else
            name += mrz[0].label[j];
    }
    name = name.substr(0, name.size() - 1);
    //det.fieldMRZ = name;
    //det.typeFieldMRZ = "name";
    //allFields.push_back(det);

    // Second line

    for(size_t i = 0; i < 9 && mrz[1].label[i] != '<'; ++i)
        docNumber += mrz[1].label[i];
    //det.fieldMRZ = docNumber;
    //det.typeFieldMRZ = "docNumber";
    //allFields.push_back(det);

    checkDocNum = mrz[1].label[9];

    if(!check(docNumber, checkDocNum))
        std::cout << "Check in document number faild." << std::endl;
    else
        std::cout << "Check in document number OK." << std::endl;
    
    nationality = "";
    nationality = nationality + mrz[1].label[10] + mrz[1].label[11] + mrz[1].label[12];
    //det.fieldMRZ = nationality;
    //det.typeFieldMRZ = "nationality";
    //allFields.push_back(det);

    for(size_t i = 13; i < 19; ++i)
        dateBirth += mrz[1].label[i];
    //det.fieldMRZ = dateBirth;
    //det.typeFieldMRZ = "dateBirth";
    //allFields.push_back(det);

    checkDateBirth = mrz[1].label[19];

    if(!check(dateBirth, checkDateBirth))
        std::cout << "Check in date of birth faild." << std::endl;
    else
        std::cout << "Check in date of birth OK." << std::endl;

    sex = mrz[1].label[20];
    //det.fieldMRZ = sex;
    //det.typeFieldMRZ = "sex";
    //allFields.push_back(det);

    for(size_t i = 21; i < 27; ++i)
        dateExpireDoc += mrz[1].label[i];
    //det.fieldMRZ = dateExpireDoc;
    //det.typeFieldMRZ = "dateExpireDoc";
    //allFields.push_back(det);

    checkDateExpireDoc = mrz[1].label[27];

    if(!check(dateExpireDoc, checkDateExpireDoc))
        std::cout << "Check in date of expire faild." << std::endl;
    else
        std::cout << "Check in date of expire OK." << std::endl;

    if(mrz[1].label[28] == '<')
    {
        optionalData = "NULL";
        checkOptionalData = "NULL";
    }
    else
    {
        for(size_t i = 28; i < 42 && mrz[1].label[i] != '<'; ++i)
            optionalData += mrz[1].label[i];

        checkOptionalData = mrz[1].label[42];

        if(!check(optionalData, checkOptionalData))
            std::cout << "Check in optional data faild." << std::endl;
        else
            std::cout << "Check in optional data OK." << std::endl;
    }

    checkOverallDigit = mrz[1].label[43];

    if(!checkOverall(checkOverallDigit))
        std::cout << "Check overall faild." << std::endl;
    else
        std::cout << "Check overall OK." << std::endl;
}


void TD3::printMRZFields()
{
    std::cout << "\nMRZ fields detected in TD3 MRZ:" << std::endl;
    std::cout << "Document type: " << docType << std::endl;
    std::cout << "State: " << state << std::endl;
    std::cout << "Surname: " << surname << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Document number: " << docNumber << std::endl;
    std::cout << "Check document number: " << checkDocNum << std::endl;
    std::cout << "Nationality: " << nationality << std::endl;
    std::cout << "Date of birth: " << dateBirth << std::endl;
    std::cout << "Check date of birth: " << checkDateBirth << std::endl;
    std::cout << "Sex: " << sex << std::endl;
    std::cout << "Date of expire: " << dateExpireDoc << std::endl;
    std::cout << "Check date of expire: " << checkDateExpireDoc << std::endl;
    std::cout << "Optional data: " << optionalData << std::endl;
    std::cout << "Check optional data: " << checkOptionalData << std::endl;
    std::cout << "Check overall: " << checkOverallDigit << std::endl;
}