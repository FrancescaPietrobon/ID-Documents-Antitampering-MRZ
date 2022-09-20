#include "../include/MRZ.h"

MRZ::MRZ(std::vector<std::vector<Character>> characters, int nl):
    mrz(characters), numLines(nl){}
    

TD1::TD1(std::vector<std::vector<Character>> characters, int nl):
    MRZ(characters, nl){}

TD2::TD2(std::vector<std::vector<Character>> characters, int nl):
    MRZ(characters, nl){}

TD3::TD3(std::vector<std::vector<Character>> characters, int nl):
    MRZ(characters, nl){}

MRVA::MRVA(std::vector<std::vector<Character>> characters, int nl):
    MRZ(characters, nl){}

MRVB::MRVB(std::vector<std::vector<Character>> characters, int nl):
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

void MRVA::printMRZFields()
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
}


void TD3::extractFields()
{
    // First line

    docType = mrz[0][0].getLabel();

    if(mrz[0][1].getLabel() != "<")
        passportType = mrz[0][1].getLabel();
    else
        passportType = "NULL";

    state = mrz[0][2].getLabel() + mrz[0][3].getLabel() + mrz[0][4].getLabel();

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

    for(size_t j = i; j < 44; ++j)
    {
        if(mrz[0][j].getLabel() == "<" && mrz[0][j-1].getLabel() == "<")
            break;
        else if(mrz[0][j].getLabel() == "<")
            name += " ";
        else
            name += mrz[0][j].getLabel();
    }    

    // Second line

    for(size_t i = 0; i < 9 && mrz[1][i].getLabel() != "<"; ++i)
        docNumber += mrz[1][i].getLabel();

    checkDocNum = mrz[1][9].getLabel();

    if(!check(docNumber, checkDocNum))
        std::cout << "Check in document number faild." << std::endl;
    else
        std::cout << "Check in document number OK." << std::endl;
    
    nationality = mrz[1][10].getLabel() + mrz[1][11].getLabel() + mrz[1][12].getLabel();

    for(size_t i = 13; i < 19; ++i)
        dateBirth += mrz[1][i].getLabel();

    checkDateBirth = mrz[1][19].getLabel();

    if(!check(dateBirth, checkDateBirth))
        std::cout << "Check in date of birth faild." << std::endl;
    else
        std::cout << "Check in date of birth OK." << std::endl;

    sex = mrz[1][20].getLabel();

    for(size_t i = 21; i < 27; ++i)
        dateExpireDoc += mrz[1][i].getLabel();

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


void TD2::extractFields()
{
    // First line

    docType = mrz[0][0].getLabel();

    if(mrz[0][1].getLabel() != "<")
        passportType = mrz[0][1].getLabel();
    else
        passportType = "NULL";

    state = mrz[0][2].getLabel() + mrz[0][3].getLabel() + mrz[0][4].getLabel();

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

    for(size_t j = i; j < 36; ++j)
    {
        if(mrz[0][j].getLabel() == "<" && mrz[0][j-1].getLabel() == "<")
            break;
        else if(mrz[0][j].getLabel() == "<")
            name += " ";
        else
            name += mrz[0][j].getLabel();
    }    

    // Second line

    for(size_t i = 0; i < 9 && mrz[1][i].getLabel() != "<"; ++i)
        docNumber += mrz[1][i].getLabel();

    checkDocNum = mrz[1][9].getLabel();

    if(!check(docNumber, checkDocNum))
        std::cout << "Check in document number faild." << std::endl;
    else
        std::cout << "Check in document number OK." << std::endl;
    
    nationality = mrz[1][10].getLabel() + mrz[1][11].getLabel() + mrz[1][12].getLabel();

    for(size_t i = 13; i < 19; ++i)
        dateBirth += mrz[1][i].getLabel();

    checkDateBirth = mrz[1][19].getLabel();

    if(!check(dateBirth, checkDateBirth))
        std::cout << "Check in date of birth faild." << std::endl;
    else
        std::cout << "Check in date of birth OK." << std::endl;

    sex = mrz[1][20].getLabel();

    for(size_t i = 21; i < 27; ++i)
        dateExpireDoc += mrz[1][i].getLabel();

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


void TD1::extractFields()
{
    // First line

    docType = mrz[0][0].getLabel();

    state = mrz[0][2].getLabel() + mrz[0][3].getLabel() + mrz[0][4].getLabel();

    for(size_t i = 5; i < 14 && mrz[0][i].getLabel() != "<"; ++i)
        docNumber += mrz[0][i].getLabel();

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

    checkDateBirth = mrz[1][6].getLabel();

    sex = mrz[1][7].getLabel();

    for(size_t i = 8; i < 14; ++i)
        dateExpireDoc += mrz[1][i].getLabel();

    checkDateExpireDoc = mrz[1][14].getLabel();

    nationality = mrz[1][15].getLabel() + mrz[1][16].getLabel() + mrz[1][17].getLabel();

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

    for(size_t j = i; j < 30; ++j)
    {
        if(mrz[2][j].getLabel() == "<" && mrz[2][j-1].getLabel() == "<")
            break;
        else if(mrz[2][j].getLabel() == "<")
            name += " ";
        else
            name += mrz[2][j].getLabel();
    }
}


void MRVA::extractFields()
{
    // First line

    docType = mrz[0][0].getLabel();

    if(mrz[0][1].getLabel() != "<")
        passportType = mrz[0][1].getLabel();
    else
        passportType = "NULL";

    state = mrz[0][2].getLabel() + mrz[0][3].getLabel() + mrz[0][4].getLabel();

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

    for(size_t j = i; j < 44; ++j)
    {
        if(mrz[0][j].getLabel() == "<" && mrz[0][j-1].getLabel() == "<")
            break;
        else if(mrz[0][j].getLabel() == "<")
            name += " ";
        else
            name += mrz[0][j].getLabel();
    }

    // Second line

    for(size_t i = 0; i < 9 && mrz[1][i].getLabel() != "<"; ++i)
        docNumber += mrz[1][i].getLabel();

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
        optionalData = "NULL";
    else
    {
        for(size_t i = 28; i < 44; ++i)
        {
            if(mrz[1][i].getLabel() == "<" && mrz[1][i-1].getLabel() == "<")
                break;
            else if(mrz[1][i].getLabel() == "<")
                optionalData += " ";
            else
                optionalData += mrz[1][i].getLabel();
        }
    }
}


void MRVB::extractFields()
{
    // First line

    docType = mrz[0][0].getLabel();

    if(mrz[0][1].getLabel() != "<")
        passportType = mrz[0][1].getLabel();
    else
        passportType = "NULL";

    state = mrz[0][2].getLabel() + mrz[0][3].getLabel() + mrz[0][4].getLabel();

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

    for(size_t j = i; j < 36; ++j)
    {
        if(mrz[0][j].getLabel() == "<" && mrz[0][j-1].getLabel() == "<")
            break;
        else if(mrz[0][j].getLabel() == "<")
            name += " ";
        else
            name += mrz[0][j].getLabel();
    }

    // Second line

    i = 0;
    for(size_t i = 0; i < 9 && mrz[1][i].getLabel() != "<"; ++i)
        docNumber += mrz[1][i].getLabel();

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
        optionalData = "NULL";
    else
    {
        for(size_t i = 28; i < 36; ++i)
        {
            if(mrz[1][i].getLabel() == "<" && mrz[1][i-1].getLabel() == "<")
                break;
            else if(mrz[1][i].getLabel() == "<")
                optionalData += " ";
            else
                optionalData += mrz[1][i].getLabel();
        }
    }
}

bool MRZ::check(std::string field, std::string checkDigit)
{
    int sum = 0, i = 0;
    extern std::unordered_map<char, unsigned> digit_conversion;

    std::vector<int> weight;
    std::vector<int> vals {7, 3, 1};

    while(i < field.size())
    {
        for(int j: vals)
        {
            weight.push_back(j);
            ++i;
        }
    }
    
    for(size_t i = 0; i < field.size(); ++i)
        sum += digit_conversion[field[i]] * weight[i];

    return (checkDigit == std::to_string(sum % 10));
}

bool MRZ::CheckOverAll(std::string checkOverall)
{
    std::string stringForCheckOverall;
    for(int i = 0; i < 10; ++i)
        stringForCheckOverall += mrz[0][i].getLabel();
    
    for(int i = 13; i < 20; ++i)
        stringForCheckOverall += mrz[0][i].getLabel();

    for(int i = 21; i < 43; ++i)
        stringForCheckOverall += mrz[1][i].getLabel();
        
    if(!check(stringForCheckOverall, checkOverall))
        std::cout << "Check overall faild." << std::endl;
    else
        std::cout << "Check overall OK." << std::endl;
    
    return check(stringForCheckOverall, checkOverall);
}