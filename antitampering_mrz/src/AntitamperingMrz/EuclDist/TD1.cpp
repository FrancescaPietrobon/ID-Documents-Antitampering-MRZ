#include "TD1.h"


TD1::TD1(std::vector<std::vector<Character>> characters):
    MRZ(characters){}


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


void TD1::extractFields()
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

    for(size_t i = 5; i < 14 && mrz[0][i].getLabel() != "<"; ++i)
        docNumber += mrz[0][i].getLabel();
    det.fieldMRZ = docNumber;
    det.typeFieldMRZ = "docNumber";
    allFields.push_back(det);

    checkDocNum = mrz[0][14].getLabel();

    if(mrz[0][15].getLabel() == "<")
    {
        optionalData = "NULL";
        checkOptionalData = "NULL";
    }
    else
    {
        for(size_t i = 15; i < 29 && mrz[0][i].getLabel() != "<"; ++i)
            optionalData += mrz[0][i].getLabel();

        checkOptionalData = mrz[0][29].getLabel();
    }

    // Second line

    for(size_t i = 0; i < 6; ++i)
        dateBirth += mrz[1][i].getLabel();
    det.fieldMRZ = dateBirth;
    det.typeFieldMRZ = "dateBirth";
    allFields.push_back(det);

    checkDateBirth = mrz[1][6].getLabel();

    sex = mrz[1][7].getLabel();
    det.fieldMRZ = sex;
    det.typeFieldMRZ = "sex";
    allFields.push_back(det);

    for(size_t i = 8; i < 14; ++i)
        dateExpireDoc += mrz[1][i].getLabel();
    det.fieldMRZ = dateExpireDoc;
    det.typeFieldMRZ = "dateExpireDoc";
    allFields.push_back(det);

    checkDateExpireDoc = mrz[1][14].getLabel();

    nationality = mrz[1][15].getLabel() + mrz[1][16].getLabel() + mrz[1][17].getLabel();
    det.fieldMRZ = nationality;
    det.typeFieldMRZ = "nationality";
    allFields.push_back(det);

    if(mrz[1][18].getLabel() == "<")
        secondOptionalData = "NULL";
    else
    {
        for(size_t i = 18; i < 29 && mrz[1][i].getLabel() != "<"; ++i)
            secondOptionalData += mrz[1][i].getLabel();
    }

    checkFirstTwoLines = mrz[1][29].getLabel();

    // Third line

    int i = 0;
    for(size_t j = i; j < 30; ++j)
    {
        if(mrz[2][j].getLabel() == "<" && mrz[2][j-1].getLabel() == "<")
        {
            i = j + 1;
            break;
        }
        else if(mrz[2][j].getLabel() == "<")
            surname += " ";
        else
            surname += mrz[2][j].getLabel();
    }
    det.fieldMRZ = surname;
    det.typeFieldMRZ = "surname";
    allFields.push_back(det);

    for(size_t j = i; j < 30; ++j)
    {
        if(mrz[2][j].getLabel() == "<" && mrz[2][j-1].getLabel() == "<")
            break;
        else if(mrz[2][j].getLabel() == "<")
            name += " ";
        else
            name += mrz[2][j].getLabel();
    }
    det.fieldMRZ = name;
    det.typeFieldMRZ = "name";
    allFields.push_back(det);
}