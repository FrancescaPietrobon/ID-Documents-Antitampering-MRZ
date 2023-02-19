#pragma once

#include "CharactersAssociator/DBSCAN/DBSCAN.hpp"
#include "common/exceptions/Exceptions.hpp"

#include <spdlog/spdlog.h>
#include <map>

#define EPS 27

enum AssociatorType
{
    Dbscan,
    //boh
};

class CharactersAssociatorFactory {
    public: 
        static std::shared_ptr<CharactersAssociator> createAssociator(const std::string& algorithmType);
};