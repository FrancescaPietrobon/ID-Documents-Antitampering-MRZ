#include "../include/MRZ/TD2.h"


TD2::TD2(std::vector<std::vector<Character>> characters, int nl):
    MRZ(characters, nl){}


void TD2::extractFields()
{
    // First line

    docType = mrz[0][0].getLabel();
    allFields.insert(std::pair<std::string, std::string>("docType", docType));
    allFieldsInv.insert(std::pair<std::string, std::string>(docType, "docType"));

    if(mrz[0][1].getLabel() != "<")
        passportType = mrz[0][1].getLabel();
    else
        passportType = "NULL";
    allFields.insert(std::pair<std::string, std::string>("passportType", passportType));
    allFieldsInv.insert(std::pair<std::string, std::string>(passportType, "passportType"));

    state = mrz[0][2].getLabel() + mrz[0][3].getLabel() + mrz[0][4].getLabel();
    allFields.insert(std::pair<std::string, std::string>("state", state));
    allFieldsInv.insert(std::pair<std::string, std::string>(state, "state"));

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
    allFields.insert(std::pair<std::string, std::string>("surname", surname.substr(0, surname.size() - 1)));
    allFieldsInv.insert(std::pair<std::string, std::string>(surname.substr(0, surname.size() - 1), "surname"));

    for(size_t j = i; j < 36; ++j)
    {
        if(mrz[0][j].getLabel() == "<" && mrz[0][j-1].getLabel() == "<")
            break;
        else if(mrz[0][j].getLabel() == "<")
            name += " ";
        else
            name += mrz[0][j].getLabel();
    }
    allFields.insert(std::pair<std::string, std::string>("name", name.substr(0, name.size() - 1)));
    allFieldsInv.insert(std::pair<std::string, std::string>(name.substr(0, name.size() - 1), "name"));   

    // Second line

    for(size_t i = 0; i < 9 && mrz[1][i].getLabel() != "<"; ++i)
        docNumber += mrz[1][i].getLabel();
    allFields.insert(std::pair<std::string, std::string>("docNumber", docNumber));
    allFieldsInv.insert(std::pair<std::string, std::string>(docNumber, "docNumber"));

    checkDocNum = mrz[1][9].getLabel();

    if(!check(docNumber, checkDocNum))
        std::cout << "Check in document number faild." << std::endl;
    else
        std::cout << "Check in document number OK." << std::endl;
    
    nationality = mrz[1][10].getLabel() + mrz[1][11].getLabel() + mrz[1][12].getLabel();
    allFields.insert(std::pair<std::string, std::string>("nationality", nationality));
    allFieldsInv.insert(std::pair<std::string, std::string>(nationality, "nationality"));

    for(size_t i = 13; i < 19; ++i)
        dateBirth += mrz[1][i].getLabel();
    allFields.insert(std::pair<std::string, std::string>("dateBirth", dateBirth));
    allFieldsInv.insert(std::pair<std::string, std::string>(dateBirth, "dateBirth"));

    checkDateBirth = mrz[1][19].getLabel();

    if(!check(dateBirth, checkDateBirth))
        std::cout << "Check in date of birth faild." << std::endl;
    else
        std::cout << "Check in date of birth OK." << std::endl;

    sex = mrz[1][20].getLabel();
    allFields.insert(std::pair<std::string, std::string>("sex", sex));
    allFieldsInv.insert(std::pair<std::string, std::string>(sex, "sex"));

    for(size_t i = 21; i < 27; ++i)
        dateExpireDoc += mrz[1][i].getLabel();
    allFields.insert(std::pair<std::string, std::string>("dateExpireDoc", dateExpireDoc));
    allFieldsInv.insert(std::pair<std::string, std::string>(dateExpireDoc, "dateExpireDoc"));

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

    checkOverall = mrz[1][35].getLabel();

    bool finalCheck = CheckOverAll(checkOverall);
}