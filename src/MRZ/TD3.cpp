#include "../include/MRZ/TD3.h"


TD3::TD3(std::vector<std::vector<Character>> characters, int nl):
    MRZ(characters, nl){}


void TD3::printMRZFields()
{
    std::cout << "Document type: " << docType << std::endl;
    std::cout << "Passport type: " << passportType << std::endl;
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
    std::cout << "Check overall: " << checkOverall << std::endl;
}


void TD3::extractFields()
{
    // First line

    docType = mrz[0][0].getLabel();
    allFields.insert(std::pair<std::string, std::string>("docType", docType));
    allFieldsInv.insert(std::pair<std::string, std::string>(docType, "docType"));
    detection det;
    det.fieldMRZ = docType;
    det.typeFieldMRZ = "docType";
    allFieldsStruct.push_back(det);
    //std::cout << det.typeFieldMRZ << " " << det.fieldMRZ << std::endl;

    if(mrz[0][1].getLabel() != "<")
        passportType = mrz[0][1].getLabel();
    else
        passportType = "NULL";

    state = mrz[0][2].getLabel() + mrz[0][3].getLabel() + mrz[0][4].getLabel();
    allFields.insert(std::pair<std::string, std::string>("state", state));
    allFieldsInv.insert(std::pair<std::string, std::string>(state, "state"));
    detection det1;
    det1.fieldMRZ = state;
    det1.typeFieldMRZ = "state";
    allFieldsStruct.push_back(det1);
    //std::cout << det1.typeFieldMRZ << " " << det1.fieldMRZ << std::endl;

    int i = 5;
    for(size_t j = i; j < 44; ++j)
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
    surname = surname.substr(0, surname.size() - 1);
    allFields.insert(std::pair<std::string, std::string>("surname", surname));
    allFieldsInv.insert(std::pair<std::string, std::string>(surname, "surname"));
    detection det2;
    det2.fieldMRZ = surname;
    det2.typeFieldMRZ = "surname";
    allFieldsStruct.push_back(det2);
    //std::cout << det2.typeFieldMRZ << " " << det2.fieldMRZ << std::endl;

    for(size_t j = i; j < 44; ++j)
    {
        if(mrz[0][j].getLabel() == "<" && mrz[0][j-1].getLabel() == "<")
            break;
        else if(mrz[0][j].getLabel() == "<")
            name += " ";
        else
            name += mrz[0][j].getLabel();
    }
    name = name.substr(0, name.size() - 1);
    allFields.insert(std::pair<std::string, std::string>("name", name));
    allFieldsInv.insert(std::pair<std::string, std::string>(name, "name"));
    detection det3;
    det3.fieldMRZ = name;
    det3.typeFieldMRZ = "name";
    allFieldsStruct.push_back(det3);
    //std::cout << det3.typeFieldMRZ << " " << det3.fieldMRZ << std::endl;

    // Second line

    for(size_t i = 0; i < 9 && mrz[1][i].getLabel() != "<"; ++i)
        docNumber += mrz[1][i].getLabel();
    allFields.insert(std::pair<std::string, std::string>("docNumber", docNumber));
    allFieldsInv.insert(std::pair<std::string, std::string>(docNumber, "docNumber"));
    detection det4;
    det4.fieldMRZ = docNumber;
    det4.typeFieldMRZ = "docNumber";
    allFieldsStruct.push_back(det4);
    //std::cout << det4.typeFieldMRZ << " " << det4.fieldMRZ << std::endl;

    checkDocNum = mrz[1][9].getLabel();

    if(!check(docNumber, checkDocNum))
        std::cout << "Check in document number faild." << std::endl;
    else
        std::cout << "Check in document number OK." << std::endl;
    
    nationality = mrz[1][10].getLabel() + mrz[1][11].getLabel() + mrz[1][12].getLabel();
    allFields.insert(std::pair<std::string, std::string>("nationality", nationality));
    allFieldsInv.insert(std::pair<std::string, std::string>(nationality, "nationality"));
    detection det5;
    det5.fieldMRZ = nationality;
    det5.typeFieldMRZ = "nationality";
    allFieldsStruct.push_back(det5);
    //std::cout << det5.typeFieldMRZ << " " << det5.fieldMRZ << std::endl;

    for(size_t i = 13; i < 19; ++i)
        dateBirth += mrz[1][i].getLabel();
    allFields.insert(std::pair<std::string, std::string>("dateBirth", dateBirth));
    allFieldsInv.insert(std::pair<std::string, std::string>(dateBirth, "dateBirth"));
    detection det6;
    det6.fieldMRZ = dateBirth;
    det6.typeFieldMRZ = "dateBirth";
    allFieldsStruct.push_back(det6);
    //std::cout << det6.typeFieldMRZ << " " << det6.fieldMRZ << std::endl;

    checkDateBirth = mrz[1][19].getLabel();

    if(!check(dateBirth, checkDateBirth))
        std::cout << "Check in date of birth faild." << std::endl;
    else
        std::cout << "Check in date of birth OK." << std::endl;

    sex = mrz[1][20].getLabel();
    allFields.insert(std::pair<std::string, std::string>("sex", sex));
    allFieldsInv.insert(std::pair<std::string, std::string>(sex, "sex"));
    detection det7;
    det7.fieldMRZ = sex;
    det7.typeFieldMRZ = "sex";
    allFieldsStruct.push_back(det7);
    //std::cout << det7.typeFieldMRZ << " " << det7.fieldMRZ << std::endl;

    for(size_t i = 21; i < 27; ++i)
        dateExpireDoc += mrz[1][i].getLabel();
    allFields.insert(std::pair<std::string, std::string>("dateExpireDoc", dateExpireDoc));
    allFieldsInv.insert(std::pair<std::string, std::string>(dateExpireDoc, "dateExpireDoc"));
    detection det8;
    det8.fieldMRZ = dateExpireDoc;
    det8.typeFieldMRZ = "dateExpireDoc";
    allFieldsStruct.push_back(det8);
    //std::cout << det8.typeFieldMRZ << " " << det8.fieldMRZ << std::endl;

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
        for(size_t i = 28; i < 42 && mrz[1][i].getLabel() != "<"; ++i)
            optionalData += mrz[1][i].getLabel();

        checkOptionalData = mrz[1][42].getLabel();

        if(!check(optionalData, checkOptionalData))
            std::cout << "Check in optional data faild." << std::endl;
        else
            std::cout << "Check in optional data OK." << std::endl;
    }

    checkOverall = mrz[1][43].getLabel();

    bool finalCheck = CheckOverAll(checkOverall);
}