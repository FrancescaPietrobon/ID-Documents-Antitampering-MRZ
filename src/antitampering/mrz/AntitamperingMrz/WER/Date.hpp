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
    
    public:
        Date(){};
        std::string findDate(std::string data);
};