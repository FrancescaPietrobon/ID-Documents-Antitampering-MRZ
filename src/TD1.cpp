#include "../include/TD1.h"


TD1::TD1(std::vector<std::vector<Character>> characters, int nl):
    MRZ(characters, nl){}

void TD1::extractFields()
{
    // First line

    docType = mrz[0][0].getLabel();
    allFields.insert(std::pair<std::string, std::string>("docType", docType));
    allFieldsInv.insert(std::pair<std::string, std::string>(docType, "docType"));

    state = mrz[0][2].getLabel() + mrz[0][3].getLabel() + mrz[0][4].getLabel();
    allFields.insert(std::pair<std::string, std::string>("state", state));
    allFieldsInv.insert(std::pair<std::string, std::string>(state, "state"));

    for(size_t i = 5; i < 14 && mrz[0][i].getLabel() != "<"; ++i)
        docNumber += mrz[0][i].getLabel();
    allFields.insert(std::pair<std::string, std::string>("docNumber", docNumber));
    allFieldsInv.insert(std::pair<std::string, std::string>(docNumber, "docNumber"));

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
    allFields.insert(std::pair<std::string, std::string>("dateBirth", dateBirth));
    allFieldsInv.insert(std::pair<std::string, std::string>(dateBirth, "dateBirth"));

    checkDateBirth = mrz[1][6].getLabel();

    sex = mrz[1][7].getLabel();
    allFields.insert(std::pair<std::string, std::string>("sex", sex));
    allFieldsInv.insert(std::pair<std::string, std::string>(sex, "sex"));

    for(size_t i = 8; i < 14; ++i)
        dateExpireDoc += mrz[1][i].getLabel();
    allFields.insert(std::pair<std::string, std::string>("dateExpireDoc", dateExpireDoc));
    allFieldsInv.insert(std::pair<std::string, std::string>(dateExpireDoc, "dateExpireDoc"));

    checkDateExpireDoc = mrz[1][14].getLabel();

    nationality = mrz[1][15].getLabel() + mrz[1][16].getLabel() + mrz[1][17].getLabel();
    allFields.insert(std::pair<std::string, std::string>("nationality", nationality));
    allFieldsInv.insert(std::pair<std::string, std::string>(nationality, "nationality"));

    if(mrz[1][18].getLabel() == "<")
        secondOptionalData = "NULL";
    else
    {
        for(size_t i = 18; i < 29 && mrz[1][i].getLabel() != "<"; ++i)
            optionalData += mrz[1][i].getLabel();
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
    allFields.insert(std::pair<std::string, std::string>("surname", surname.substr(0, surname.size() - 1)));
    allFieldsInv.insert(std::pair<std::string, std::string>(surname.substr(0, surname.size() - 1), "surname")); 

    for(size_t j = i; j < 30; ++j)
    {
        if(mrz[2][j].getLabel() == "<" && mrz[2][j-1].getLabel() == "<")
            break;
        else if(mrz[2][j].getLabel() == "<")
            name += " ";
        else
            name += mrz[2][j].getLabel();
    }
    allFields.insert(std::pair<std::string, std::string>("name", name.substr(0, name.size() - 1)));
    allFieldsInv.insert(std::pair<std::string, std::string>(name.substr(0, name.size() - 1), "name")); 
}