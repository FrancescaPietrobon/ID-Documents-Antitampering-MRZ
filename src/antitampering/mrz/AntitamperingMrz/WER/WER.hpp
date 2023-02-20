#pragma once

#include "../AntitamperingMrz.hpp"

#include <vector>

class WER : public AntitamperingMrz 
{
    public:
        WER(const float eps);
        virtual ~WER(){};
        std::vector<DoubtfulFields> check(const Fields *fields, const size_t fieldsSize) override;
    private:
        const float eps;
        MRZ extractMRZ(const Fields *fields, const size_t fieldsSize);
};