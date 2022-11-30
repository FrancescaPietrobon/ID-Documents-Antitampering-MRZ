#include "Date.h"


Date::Date(std::string d):
    data(d){}


std::string Date::getNewDate()
{
    return newDate;
}


bool Date::isDate()
{
    findMonth();
    if(possibleMonth != "None")
        return checkAlphanumericDate();

    if(isNumber(data) && ((data.size() == 6) || (data.size() == 9)))
        return checkNumericDate();

    return false;
}


void Date::findMonth()
{
    // It check if a month in text is detected (all the characters of the month must be correct)
    // and return the possibile month in characters and the corresponding index in the field

    extern std::map<std::string, std::string> monthUCtoNum, monthLCtoNum;

    searchMonth(data, monthUCtoNum);
    searchMonth(data, monthLCtoNum);
}


void Date::searchMonth(std::string field, std::map<std::string, std::string> months)
{
    for(auto m: months)
    {
        // Check if the name of one month is contained in the string field
        if(field.find(m.first) != std::string::npos) 
        {
            possibleMonth = m.first;
            month = months[possibleMonth];
            index = field.find(m.first);
            return;
        }
    }
}


bool Date::checkAlphanumericDate()
{
    // TO DO: manage also yy mm dd and yyyy mm dd format
    
    // Take only the last two numbers of the year
    year = data[data.size()-2];
    year += data[data.size()-1];
    
    // Check if there is only one number for the day or two
    if(index == 1)
    {
        day = "0";
        day += data[0];
    }
    else
    {
        day = data[0];
        day += data[1];
    }
    
    if(!dayOk())
        return false;

    newDate = year + month + day;

    std::cout << "Date " << data << " converted in " << newDate << "\n" << std::endl;

    return true;
}


bool Date::checkNumericDate()
{
    day = data[0];
    day += data[1];
    if(!dayOk())
        return false;

    month = data[2];
    month += data[3];
    if(!monthOk())
        return false;

    year = data[data.size()-2];
    year += data[data.size()-1];

    newDate = year + month + day;

    std::cout << "Date " << data << " converted in " << newDate << "\n" << std::endl;

    return true;
}


bool Date::dayOk()
{
    return (isNumber(day) && (std::stoi(day) > 0) && (std::stoi(day) <= 31));
}


bool Date::monthOk()
{
    return (isNumber(month) && (std::stoi(month) > 0) && (std::stoi(month) <= 12));
}