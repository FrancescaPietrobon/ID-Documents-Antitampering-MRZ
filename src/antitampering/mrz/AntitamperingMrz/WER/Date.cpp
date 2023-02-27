#include "Date.hpp"

std::string Date::findDate(std::string data)
{
    std::string newDate = "None";
    std::pair<std::string, size_t> monthWithIndex = findMonth(data);
    std::string month = monthWithIndex.first;
    size_t monthIndex = monthWithIndex.second;
    if(month != "None")
        newDate = extractAlphanumericDate(data, month, monthIndex);
    else if(isNumber(data) && ((data.size() == 6) || (data.size() == 9)))
        newDate = extractNumericDate(data);
    return newDate;
}

bool Date::isNumber(const std::string& str)
{
    return str.find_first_not_of("0123456789") == std::string::npos;
}

std::pair<std::string, size_t> Date::findMonth(std::string data)
{
    // It check if a month in text is detected (all the characters of the month must be correct)
    // and return the possibile month in characters and the corresponding index in the field
    std::pair<std::string, size_t> monthWithIndex = searchMonth(data, monthUCtoNum);
    if(monthWithIndex.first == "None")
        monthWithIndex = searchMonth(data, monthLCtoNum);
    return monthWithIndex;
}


std::pair<std::string, size_t> Date::searchMonth(std::string field, std::map<std::string, std::string> months)
{
    std::string month = "None";
    size_t index = 0;
    for(auto m: months)
    {
        // Check if the name of one month is contained in the string field
        if(field.find(m.first) != std::string::npos) 
        {
            month = months[m.first];
            index = field.find(m.first);
            return std::make_pair(month, index);
        }
    }
    return std::make_pair(month, index);
    
}


std::string Date::extractAlphanumericDate(std::string data, std::string month, size_t index)
{
    // TO DO: manage also yy mm dd and yyyy mm dd format

    std::string newDate = "None";
    std::string year, day;

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
    
    if(!dayOk(day))
        return newDate;

    newDate = year + month + day;

    std::cout << "Date " << data << " converted in " << newDate << "\n" << std::endl;

    return newDate;
}


std::string Date::extractNumericDate(std::string data)
{
    std::string newDate = "None";
    std::string year, month, day;

    day = data[0];
    day += data[1];
    if(!dayOk(day))
        return newDate;

    month = data[2];
    month += data[3];
    if(!monthOk(month))
        return newDate;

    year = data[data.size()-2];
    year += data[data.size()-1];

    newDate = year + month + day;

    std::cout << "Date " << data << " converted in " << newDate << "\n" << std::endl;

    return newDate;
}


bool Date::dayOk(std::string day)
{
    return (isNumber(day) && (std::stoi(day) > 0) && (std::stoi(day) <= 31));
}


bool Date::monthOk(std::string month)
{
    return (isNumber(month) && (std::stoi(month) > 0) && (std::stoi(month) <= 12));
}