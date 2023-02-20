#include "WER.hpp"

WER::WER(float e):
    eps(e){};

std::vector<DoubtfulFields> WER::check(const Fields *fields, const size_t fieldsSize)
{
    std::vector<DoubtfulFields> doubtfulFields;
    // TO BE FILLED

    return doubtfulFields;
}

MRZ extractMRZ(const Fields *fields, const size_t fieldsSize)
{
    
}