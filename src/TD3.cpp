#include "../include/TD3.h"


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
    //allFields.emplace("docType", docType);
    //allFieldsInv.emplace(docType, "docType");
    allFields.insert(std::pair<std::string, std::string>("docType", docType));
    allFieldsInv.insert(std::pair<std::string, std::string>(docType, "docType"));

    if(mrz[0][1].getLabel() != "<")
        passportType = mrz[0][1].getLabel();
    else
        passportType = "NULL";

    state = mrz[0][2].getLabel() + mrz[0][3].getLabel() + mrz[0][4].getLabel();
    //allFields.emplace("state", state);
    //allFieldsInv.emplace(state, "state");
    allFields.insert(std::pair<std::string, std::string>("state", state));
    allFieldsInv.insert(std::pair<std::string, std::string>(state, "state"));
    //allFieldsInv.insert(std::pair<std::string, std::string>("state", state));
    //std::cout << state << " " << allFieldsInv.lower_bound(state)->second << std::endl;

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
    //allFields.emplace("surname", surname);
    //allFieldsInv.emplace(surname, "surname");
    allFields.insert(std::pair<std::string, std::string>("surname", surname));
    allFieldsInv.insert(std::pair<std::string, std::string>(surname, "surname"));

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
    //allFields.emplace("name", name);
    //allFieldsInv.emplace(name, "name");
    allFields.insert(std::pair<std::string, std::string>("name", name));
    allFieldsInv.insert(std::pair<std::string, std::string>(name, "name"));

    // Second line

    for(size_t i = 0; i < 9 && mrz[1][i].getLabel() != "<"; ++i)
        docNumber += mrz[1][i].getLabel();
    //allFields.emplace("docNumber", docNumber); 
    //allFieldsInv.emplace(docNumber, "docNumber");
    allFields.insert(std::pair<std::string, std::string>("docNumber", docNumber));
    allFieldsInv.insert(std::pair<std::string, std::string>(docNumber, "docNumber"));

    checkDocNum = mrz[1][9].getLabel();

    if(!check(docNumber, checkDocNum))
        std::cout << "Check in document number faild." << std::endl;
    else
        std::cout << "Check in document number OK." << std::endl;
    
    nationality = mrz[1][10].getLabel() + mrz[1][11].getLabel() + mrz[1][12].getLabel();
    //allFields.emplace("nationality", nationality);
    //allFieldsInv.emplace(nationality, "nationality");
    allFields.insert(std::pair<std::string, std::string>("nationality", nationality));
    allFieldsInv.insert(std::pair<std::string, std::string>(nationality, "nationality"));

    for(size_t i = 13; i < 19; ++i)
        dateBirth += mrz[1][i].getLabel();
    //allFields.emplace("dateBirth", dateBirth); 
    //allFieldsInv.emplace(dateBirth, "dateBirth"); 
    allFields.insert(std::pair<std::string, std::string>("dateBirth", dateBirth));
    allFieldsInv.insert(std::pair<std::string, std::string>(dateBirth, "dateBirth"));

    checkDateBirth = mrz[1][19].getLabel();

    if(!check(dateBirth, checkDateBirth))
        std::cout << "Check in date of birth faild." << std::endl;
    else
        std::cout << "Check in date of birth OK." << std::endl;

    sex = mrz[1][20].getLabel();
    //allFields.emplace("sex", sex); 
    //allFieldsInv.emplace(sex, "sex");
    allFields.insert(std::pair<std::string, std::string>("sex", sex));
    allFieldsInv.insert(std::pair<std::string, std::string>(sex, "sex"));

    for(size_t i = 21; i < 27; ++i)
        dateExpireDoc += mrz[1][i].getLabel();
    //allFields.emplace("dateExpireDoc", dateExpireDoc);
    //allFieldsInv.emplace(dateExpireDoc, "dateExpireDoc");
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