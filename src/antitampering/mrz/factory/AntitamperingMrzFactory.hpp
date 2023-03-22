#pragma once

#include "../AntitamperingMrz/WER/Associations.hpp"
#include "../AntitamperingMrz/AntitamperingMrz.hpp"
#include "common/exceptions/Exceptions.hpp"

#include <spdlog/spdlog.h>
#include <map>

#define CONF_THRESHOLD_WER 0.7

enum AssociatorType
{
    wer = 1,
    //boh
};

class AntitamperingMrzFactory {
    public: 
        static std::shared_ptr<AntitamperingMrz> createAntitamperingMrz(const std::string& algorithmType);
};
