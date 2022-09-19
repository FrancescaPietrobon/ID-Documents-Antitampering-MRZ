#include "../include/MRZ.h"

MRZ::MRZ(std::vector<std::vector<Character>> characters, int nl):
    mrz(characters), numLines(nl){}

TD1::TD1(std::vector<std::vector<Character>> characters, int nl):
    MRZ(characters, nl){}

TD3::TD3(std::vector<std::vector<Character>> characters, int nl):
    MRZ(characters, nl){}

std::string MRZ::getMRZType()
{
    return MRZType;
}

std::string MRZ::getDocType()
{
    return docType;
}

std::string MRZ::getState()
{
    return state;
}

std::string MRZ::getSurname()
{
    return surname;
}

std::string MRZ::getName()
{
    return name;
}

std::string MRZ::getDocNumber()
{
    return docNumber;
}

std::string MRZ::getCheckDocNum()
{
    return checkDocNum;
}

std::string MRZ::getNationality()
{
    return nationality;
}

std::string MRZ::getDateBirth()
{
    return dateBirth;
}

std::string MRZ::getCheckDateBirth()
{
    return checkDateBirth;
}

std::string MRZ::getSex()
{
    return sex;
}

std::string MRZ::getDateExpireDoc()
{
    return dateExpireDoc;
}

std::string MRZ::getCheckDateExpireDoc()
{
    return checkDateExpireDoc;
}

std::string MRZ::getOptionalData()
{
    return optionalData;
}

std::string MRZ::getCheckOptionalData()
{
    return checkOptionalData;
}


void MRZ::printMRZ()
{
    for(size_t i = 0; i < mrz.size(); ++i)
    {
        for(size_t j = 0; j < mrz[0].size(); ++j)
        {
            std::cout << mrz[i][j].getLabel();
        }
        std::cout << std::endl;
    }
}





void TD3::printMRZFields()
{
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
    std::cout << "Check overall: " << checkOverall << std::endl;
}


void TD3::extractFields()
{
    // First line

    docType = mrz[0][0].getLabel();

    state = mrz[0][2].getLabel() + mrz[0][3].getLabel() + mrz[0][4].getLabel();

    surname = mrz[0][5].getLabel();

    int i = 6;
    while(mrz[0][i].getLabel() != "<")
    {
        surname += mrz[0][i].getLabel();
        ++i;
    }
    while(mrz[0][i].getLabel() == "<")
        ++i;
    
    while(mrz[0][i].getLabel() != "<")
    {
        name += mrz[0][i].getLabel();
        ++i;
    }
    ++i;
    if(mrz[0][i].getLabel() != "<")
    {
        while(mrz[0][i].getLabel() != "<")
        {
            name += mrz[0][i].getLabel();
            ++i;
        }
    }
    

    // Second line

    i = 0;
    while(i < 9 &&  mrz[1][i].getLabel() != "<")
    {
        docNumber += mrz[1][i].getLabel();
        ++i;
    }
    checkDocNum = mrz[1][9].getLabel();
    
    nationality = mrz[1][10].getLabel() + mrz[1][11].getLabel() + mrz[1][12].getLabel();

    for(size_t i = 13; i < 19; ++i)
        dateBirth += mrz[1][i].getLabel();

    checkDateBirth = mrz[1][19].getLabel();

    sex = mrz[1][20].getLabel();

    for(size_t i = 21; i < 27; ++i)
        dateExpireDoc += mrz[1][i].getLabel();

    checkDateExpireDoc = mrz[1][27].getLabel();

    if(mrz[1][28].getLabel() == "<")
    {
        optionalData = "NULL";
        checkOptionalData = "NULL";
    }
    else
    {
        while(i < 27 &&  mrz[1][i].getLabel() != "<")
        {
            optionalData += mrz[1][i].getLabel();
            ++i;
        }
        checkOptionalData = mrz[1][42].getLabel();
    }

    checkOverall = mrz[1][43].getLabel();
}


void TD1::extractFields()
{
    // First line

    docType = mrz[0][0].getLabel();

    state = mrz[0][2].getLabel() + mrz[0][3].getLabel() + mrz[0][4].getLabel();

    int i = 5;
    while(i < 14 &&  mrz[0][i].getLabel() != "<")
    {
        docNumber += mrz[0][i].getLabel();
        ++i;
    }
    checkDocNum = mrz[0][14].getLabel();

    i = 15;
    if(mrz[0][15].getLabel() == "<")
    {
        optionalData = "NULL";
        checkOptionalData = "NULL";
    }
    else
    {
        while(i < 35 &&  mrz[0][i].getLabel() != "<")
        {
            optionalData += mrz[0][i].getLabel();
            ++i;
        }
        checkOptionalData = mrz[0][35].getLabel();
    }

    // Second line

    for(size_t i = 0; i < 6; ++i)
        dateBirth += mrz[1][i].getLabel();

    checkDateBirth = mrz[1][6].getLabel();

    sex = mrz[1][7].getLabel();

    for(size_t i = 8; i < 14; ++i)
        dateExpireDoc += mrz[1][i].getLabel();

    checkDateExpireDoc = mrz[1][14].getLabel();

    nationality = mrz[1][15].getLabel() + mrz[1][16].getLabel() + mrz[1][17].getLabel();

    i = 18;
    if(mrz[1][18].getLabel() == "<")
        secondOptionalData = "NULL";
    else
    {
        while(i < 35 &&  mrz[1][i].getLabel() != "<")
        {
            optionalData += mrz[1][i].getLabel();
            ++i;
        }
    }

    checkFirstTwoLines = mrz[1][35].getLabel();

    // Third line

    i = 0;
    while(mrz[2][i].getLabel() != "<")
    {
        surname += mrz[2][i].getLabel();
        ++i;
    }
    while(mrz[2][i].getLabel() == "<")
        ++i;
    
    while(mrz[2][i].getLabel() != "<")
    {
        name += mrz[2][i].getLabel();
        ++i;
    }
    ++i;
    if(mrz[2][i].getLabel() != "<")
    {
        while(mrz[2][i].getLabel() != "<")
        {
            name += mrz[2][i].getLabel();
            ++i;
        }
    }
}
