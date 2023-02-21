#pragma once

#include "../AntitamperingMrz.hpp"
#include "Mrz.hpp"

#include <vector>

class WER : public AntitamperingMrz 
{
    public:
        WER(const float eps);
        virtual ~WER(){};
        std::vector<DoubtfulFields> check(const Fields *fields, const size_t fieldsSize) override;
    private:
        const float eps;
        Mrz* extractMrz(const Fields *fields, const size_t fieldsSize);
        std::vector<Fields> findMrzLines(const Fields *fields, const size_t fieldsSize);
        MrzType findMrzType(std::vector<Fields> mrz);
};