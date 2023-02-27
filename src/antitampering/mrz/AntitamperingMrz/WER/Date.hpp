#pragma once

#include <iostream>
#include <string>
#include <map>


class Date
{
    private:
        std::string data;
        bool dayOk(std::string day);
        bool monthOk(std::string month);
        bool isNumber(const std::string& str);
        std::string extractNumericDate(std::string data);
        std::string extractAlphanumericDate(std::string data, std::string month, size_t index);
        std::pair<std::string, size_t> findMonth(std::string data);
        std::pair<std::string, size_t> searchMonth(std::string field, std::map<std::string, std::string> months);
        std::map<std::string, std::string> monthUCtoNum {{"JAN", "01"}, {"FEB", "02"}, {"MAR", "03"}, {"APR", "04"},
                                                         {"MAY", "05"}, {"JUN", "06"}, {"JUL", "07"}, {"AUG", "08"},
                                                         {"SEP", "09"}, {"OCT", "10"}, {"NOV", "11"}, {"DEC", "12"}};
        std::map<std::string, std::string> monthLCtoNum {{"Jan", "01"}, {"Feb", "02"}, {"Mar", "03"}, {"Apr", "04"},
                                                         {"May", "05"}, {"Jun", "06"}, {"Jul", "07"}, {"Aug", "08"},
                                                         {"Sep", "09"}, {"Oct", "10"}, {"Nov", "11"}, {"Dec", "12"}};
    
    public:
        Date(){};
        std::string findDate(std::string data);
};