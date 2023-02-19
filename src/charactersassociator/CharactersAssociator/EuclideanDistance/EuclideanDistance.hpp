#pragma once

#include "../CharactersAssociator.hpp"

#include <unordered_map>

typedef std::vector<std::vector<float>> matrix2D;

class EuclideanDistance : public CharactersAssociator
{
    public:
        EuclideanDistance(const float e);
        virtual ~EuclideanDistance(){};
        std::vector<Fields> associateCharacters(const Characters *characters, const size_t charactersSize) override;

    private:
        const float eps;
        
};