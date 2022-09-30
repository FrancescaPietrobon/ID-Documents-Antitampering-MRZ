#include "../include/Date.h"

Date::Date(std::string d, std::string m, size_t idx):
    date(d), possibleMonth(m), index(idx){}


std::string Date::convertAlphanumDate()
{
    extern std::map<std::string, std::string> monthUCtoNum, monthLCtoNum;

    // Check month in upper and lower case
    if(monthUCtoNum.find(possibleMonth) != monthUCtoNum.end())
        month = monthUCtoNum[possibleMonth];
    else if(monthLCtoNum.find(possibleMonth) != monthLCtoNum.end())
        month = monthLCtoNum[possibleMonth];
    else
    {
        std::cerr << "Month not found" << std::endl;
        return "None";
    }
        
    //std::cout << "Month: " << month << std::endl;

    // Take only the last two numbers of the year
    year = date[date.size()-2];
    year += date[date.size()-1];
    //std::cout << "Year: " << year << std::endl;

    // Check if there is only one number for the day or two
    if(index == 3)
    {
        day = "0";
        day += date[0];
    }
    else
    {
        day = date[0];
        day += date[1];
    }
    //std::cout << "Day: " << day << std::endl;

    newDate = year + month + day;

    std::cout << "New date:" << newDate << std::endl;

    return newDate;
}

