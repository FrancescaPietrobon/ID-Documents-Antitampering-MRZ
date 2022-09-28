#ifndef DATE_H
#define DATE_H

#include <iostream>
#include <string>
#include <map>

class Date
{
    private:
        std::string date;
        std::string possibleMonth;
        size_t index;
        std::string year;
        std::string month;
        std::string day;
        std::string newDate;

    public:
        Date(std::string d, std::string m, size_t idx);
        std::string convertDateOLD(std::string);
        std::string convertAlphanumDate();
};




#endif