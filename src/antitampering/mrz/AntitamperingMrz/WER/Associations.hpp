#pragma once

#include "../AntitamperingMrz.hpp"
#include "SplitFields.hpp"
#include "Date.hpp"
#include "WER.hpp"

#include <vector>

class Associations : public AntitamperingMrz 
{
    public:
        Associations(const float confidenceThreshold);
        virtual ~Associations(){};
        std::vector<DoubtfulFields> extractDoubtfulFields(const Fields *allFields, const size_t fieldsSize) override;
        float computeConfFinal(std::vector<DoubtfulFields> doubtfulAss);
    
    private:
        const float confidenceThreshold;
        std::vector<DoubtfulFields> doubtfulAss;
        std::vector<AssociatedField> finAss;
        Fields convertIfDate(Fields field);
        std::pair<std::vector<AssociatedField>, std::vector<DoubtfulFields>> computeAssociations(std::vector<Fields> fields, std::vector<MrzFields> mrzFields);
        bool findField(Fields dataField, std::string mrzDataField, std::string fieldType, float confidence, std::vector<DoubtfulFields> &doubtfulAss, std::vector<AssociatedField> &finAss);
        std::vector<AssociatedField> addFinalAssociation(Fields dataField, std::string mrzDataField, std::string fieldType, float confidence, std::vector<AssociatedField> finAss);
        std::vector<DoubtfulFields> addDoubtfulAssociations(char* dataField, std::string mrzDataField, std::string fieldType, float confidence, std::vector<DoubtfulFields> doubtfulAss);
};