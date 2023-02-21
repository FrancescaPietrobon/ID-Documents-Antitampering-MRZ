#include "WER.hpp"

WER::WER(float e):
    eps(e){};

std::vector<DoubtfulFields> WER::check(const Fields *fields, const size_t fieldsSize)
{
    std::vector<DoubtfulFields> doubtfulFields;
    // TO BE FILLED

    return doubtfulFields;
}

Mrz* WER::extractMrz(const Fields *fields, const size_t fieldsSize)
{
    std::vector<Fields> mrzLines = findMrzLines(fields, fieldsSize);

    MrzType type = findMrzType(mrzLines);
    Mrz* mrz;
    if(type != NONE)
    {
        mrz->createMrz(type, mrzLines);
        mrz->extractFields(mrzLines);
        //mrz->printMRZFields();
    }
    else
    {
        SPDLOG_ERROR("FATAL: Mrz Type NOT FOUND");
        throw Exception(ErrorCode::GENERAL_ERROR, std::string("Mrz Type not handled"));
    }

    return mrz;
}

std::vector<Fields> WER::findMrzLines(const Fields *fields, const size_t fieldsSize)
{
    std::map<float, Fields> orderedMrz;
    size_t countSymbolLower;
    for(size_t i = 0; i < fieldsSize; ++i)
    {
        for(size_t j = 0; j < fields[i].labelSize; ++j)
        {
            if(fields[i].label[j] == '<')
                countSymbolLower += 1;
        }
        if(countSymbolLower > 3)
            orderedMrz.emplace(fields[i].position.topLeftY, fields[i]);     
    }

    std::vector<Fields> mrzLines;
    for(auto & line: orderedMrz)
        mrzLines.push_back(line.second);

    return mrzLines;
}

MrzType WER::findMrzType(std::vector<Fields> mrz)
{
    std::string Mrztype;
    MrzType mrzType = NONE;
    if((mrz.size() == 3) && (mrz[0].labelSize == 36) && (mrz[1].labelSize == 36) && (mrz[2].labelSize == 36))
    {
        Mrztype = "TD1";
        mrzType = td1;
    }
    else if((mrz[0].labelSize == 36) && (mrz[1].labelSize == 36)) // if(mrz[0].labelSize >= 32 && mrz[0].labelSize <= 40) less restrictive case
    {
        if(mrz[0].label[0] == 'P')
        {
            Mrztype = "TD2";
            mrzType = td2;
        }
        else
        {
            Mrztype = "MRVB";
            mrzType = mrvb;
        }
    }
    else if((mrz[0].labelSize == 44) && (mrz[1].labelSize == 44)) // if(mrz[0].labelSize > 40 && mrz[0].labelSize <= 48) less restrictive case
    {
        if(mrz[0].label[0] == 'P')
        {
            Mrztype = "TD3";
            mrzType = td3;
        }
        else
        {
            Mrztype = "MRVA";
            mrzType = mrva;
        }
    }
    else
        {
            Mrztype = "NULL";
            mrzType = NONE;
        }

    return mrzType;
}