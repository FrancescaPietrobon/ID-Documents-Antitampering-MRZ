#pragma once

#include "../AntitamperingMrz/Levenshtein/Associations.hpp"
#include "../AntitamperingMrz/AntitamperingMrz.hpp"
#include "common/exceptions/Exceptions.hpp"

#include <spdlog/spdlog.h>
#include <map>

#define CONF_THRESHOLD_LEVENSHTEIN 0.7

enum AssociatorType
{
    levenshtein = 1,
    //boh
};

class AntitamperingMrzFactory {
    public: 
        static std::shared_ptr<AntitamperingMrz> createAntitamperingMrz(const std::string& algorithmType);
};
