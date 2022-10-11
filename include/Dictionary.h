#ifndef _DICTIONARY_H
#define _DICTIONARY_H

#include <unordered_map>
#include <map>
#include <string>
#include <cstring>
#include <cctype>
#include <functional>
#include <algorithm>
#include <iostream>


enum monthUpperCase
{
    JAN = 1,
    FEB,
    MAR,
    APR,
    MAY,
    JUN,
    JUL,
    AUG,
    SEP,
    OCT,
    NOV,
    DEC
};

enum monthLowerCase
{
    Jan = 1,
    Feb,
    Mar,
    Apr,
    May,
    Jun,
    Jul,
    Aug,
    Sep,
    Oct,
    Nov,
    Dec
};


template<typename K, typename V>
std::unordered_map<V, K> inverse_map(std::unordered_map<K, V> &map);

extern std::unordered_map<unsigned, std::string> dictionary;
extern std::unordered_map<char, unsigned> digit_conversion;

extern std::map<std::string, std::string> monthUCtoNum;
extern std::map<std::string, std::string> monthLCtoNum;

extern std::map<monthUpperCase, std::string> monthUpperCaseMap;
extern std::map<monthLowerCase, std::string> monthLowerCaseMap;

extern std::map<std::string, std::string> statesDic;

#endif