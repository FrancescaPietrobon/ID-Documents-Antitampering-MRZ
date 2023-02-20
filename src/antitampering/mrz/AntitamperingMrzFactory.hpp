#pragma once

#include "AntitamperingMrz/WER/WER.hpp"
#include "AntitamperingMrz/AntitamperingMrz.hpp"
#include "common/exceptions/Exceptions.hpp"

#include <spdlog/spdlog.h>
#include <map>

#define epsWER 8 //7

enum AssociatorType
{
    wer = 1,
    //boh
};

class AntitamperingMrzFactory {
    public: 
        static std::shared_ptr<AntitamperingMrz> createAntitamperingMrz(const std::string& algorithmType);
};