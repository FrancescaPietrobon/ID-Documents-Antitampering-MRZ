#ifndef DATE_H
#define DATE_H

#include <iostream>
#include <string>
#include <map>
#include "utilities.h"


class Date
{
    private:
        std::string data;
        std::string possibleMonth;
        size_t index;
        bool possibleDate = "None";
        std::string year = "None";
        std::string month = "None";
        std::string day = "None";
        std::string newDate;
        bool dayOk();
        bool monthOk();
        bool checkNumericDate();
        bool checkAlphanumericDate();
        void findMonth();
        void searchMonth(std::string field, std::map<std::string, std::string> months);

    public:
        Date(std::string d);
        bool isDate();
        std::string getNewDate();     
};




#endif