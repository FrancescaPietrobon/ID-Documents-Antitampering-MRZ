#pragma once

#include "CharactersAssociator/DBSCANpoints/DBSCANpoints.hpp"
#include "CharactersAssociator/DBSCANboxes/DBSCANboxes.hpp"
#include "common/exceptions/Exceptions.hpp"

#include <spdlog/spdlog.h>
#include <map>

#define epsDbscanPoints 8 //7
#define epsDbscanBoxes 30 //3

enum AssociatorType
{
    DbscanPoints,
    DbscanBoxes
};

class CharactersAssociatorFactory {
    public: 
        static std::shared_ptr<CharactersAssociator> createAssociator(const std::string& algorithmType);
};