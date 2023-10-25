#pragma once

#include "../AntitamperingMrz.hpp"
#include "Mrz.hpp"
#include "Date.hpp"

#include <vector>

class Levenshtein
{
    public:
        Levenshtein(){};
        float computeConfidence(const std::string& s1, const std::string& s2);

    private:
        float computeLevenshtein(const std::string& s1, const std::string& s2);
        int min3(int a, int b, int c);
};
