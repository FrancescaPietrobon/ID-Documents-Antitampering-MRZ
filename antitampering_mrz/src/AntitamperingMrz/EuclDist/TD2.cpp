#include "TD2.h"


TD2::TD2(std::vector<std::vector<Character>> characters):
    MRZ(characters){}


void TD2::printMRZFields()
{
    std::cout << "\nMRZ fields detected in TD2 MRZ:" << std::endl;
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


void TD2::extractFields()
{
    detection det;

    // First line

    docType = mrz[0][0].getLabel();
    if(mrz[0][1].getLabel() != "<")
        docType = docType + mrz[0][1].getLabel();
    det.fieldMRZ = docType;
    det.typeFieldMRZ = "docType";
    allFields.push_back(det);

    state = mrz[0][2].getLabel() + mrz[0][3].getLabel() + mrz[0][4].getLabel();
    det.fieldMRZ = state;
    det.typeFieldMRZ = "state";
    allFields.push_back(det);

    int i = 5;
    for(size_t j = i; j < 36; ++j)
    {
        if(mrz[0][j].getLabel() == "<" && mrz[0][j-1].getLabel() == "<")
        {
            i = j + 1;
            break;
        }
        else if(mrz[0][j].getLabel() == "<")
            surname += " ";
        else
            surname += mrz[0][j].getLabel();
    }
    det.fieldMRZ = surname;
    det.typeFieldMRZ = "surname";
    allFields.push_back(det);

    for(size_t j = i; j < 36; ++j)
    {
        if(mrz[0][j].getLabel() == "<" && mrz[0][j-1].getLabel() == "<")
            break;
        else if(mrz[0][j].getLabel() == "<")
            name += " ";
        else
            name += mrz[0][j].getLabel();
    }
    det.fieldMRZ = name;
    det.typeFieldMRZ = "name";
    allFields.push_back(det);

    // Second line

    for(size_t i = 0; i < 9 && mrz[1][i].getLabel() != "<"; ++i)
        docNumber += mrz[1][i].getLabel();
    det.fieldMRZ = docNumber;
    det.typeFieldMRZ = "docNumber";
    allFields.push_back(det);

    checkDocNum = mrz[1][9].getLabel();

    if(!check(docNumber, checkDocNum))
        std::cout << "Check in document number faild." << std::endl;
    else
        std::cout << "Check in document number OK." << std::endl;
    
    nationality = mrz[1][10].getLabel() + mrz[1][11].getLabel() + mrz[1][12].getLabel();
    det.fieldMRZ = nationality;
    det.typeFieldMRZ = "nationality";
    allFields.push_back(det);

    for(size_t i = 13; i < 19; ++i)
        dateBirth += mrz[1][i].getLabel();
    det.fieldMRZ = dateBirth;
    det.typeFieldMRZ = "dateBirth";
    allFields.push_back(det);

    checkDateBirth = mrz[1][19].getLabel();

    if(!check(dateBirth, checkDateBirth))
        std::cout << "Check in date of birth faild." << std::endl;
    else
        std::cout << "Check in date of birth OK." << std::endl;

    sex = mrz[1][20].getLabel();
    det.fieldMRZ = sex;
    det.typeFieldMRZ = "sex";
    allFields.push_back(det);

    for(size_t i = 21; i < 27; ++i)
        dateExpireDoc += mrz[1][i].getLabel();
    det.fieldMRZ = dateExpireDoc;
    det.typeFieldMRZ = "dateExpireDoc";
    allFields.push_back(det);

    checkDateExpireDoc = mrz[1][27].getLabel();

    if(!check(dateExpireDoc, checkDateExpireDoc))
        std::cout << "Check in date of expire faild." << std::endl;
    else
        std::cout << "Check in date of expire OK." << std::endl;

    if(mrz[1][28].getLabel() == "<")
    {
        optionalData = "NULL";
        checkOptionalData = "NULL";
    }
    else
    {
        for(size_t i = 28; i < 34 && mrz[1][i].getLabel() != "<"; ++i)
            optionalData += mrz[1][i].getLabel();

        checkOptionalData = mrz[1][34].getLabel();

        if(!check(optionalData, checkOptionalData))
            std::cout << "Check in optional data faild." << std::endl;
        else
            std::cout << "Check in optional data OK." << std::endl;
    }

    checkOverallDigit = mrz[1][35].getLabel();

    if(!checkOverall(checkOverallDigit))
        std::cout << "Check overall faild." << std::endl;
    else
        std::cout << "Check overall OK." << std::endl;
}