#pragma once

#include "../AntitamperingMrz.hpp"
#include "SplitFields.hpp"
#include "Date.hpp"
#include "Levenshtein.hpp"

#include <vector>
#include <iomanip>

class Associations : public AntitamperingMrz 
{
    public:
        Associations(const float confidenceThreshold);
        virtual ~Associations(){};
        std::vector<DoubtfulField> extractDoubtfulFields(const Field *allFields, const size_t fieldsSize) override;
        float computeConfFinal(std::vector<DoubtfulField> doubtfulAss);
    
    private:
        const float confidenceThreshold;
        bool CheckDigitsResult = true;
        std::vector<DoubtfulField> doubtfulAss;
        std::vector<AssociatedField> finAss;
        Field convertIfDate(Field field);
        std::pair<std::vector<AssociatedField>, std::vector<DoubtfulField>> computeAssociations(std::vector<Field> fields, std::vector<MrzField> mrzFields);
        bool findField(Field dataField, std::string mrzDataField, std::string fieldType, float confidence,
                       std::vector<DoubtfulField> &doubtfulAss, std::vector<AssociatedField> &finAss);
        std::pair<std::vector<AssociatedField>, std::vector<DoubtfulField>> addAssociation(Field dataField, std::string mrzDataField, std::string fieldType,
                                                                                           float confidence, std::vector<DoubtfulField> &doubtfulAss,
                                                                                           std::vector<AssociatedField> &finAss);
        std::vector<AssociatedField> addFinalAssociation(Field dataField, std::string mrzDataField, std::string fieldType,
                                                         float confidence, std::vector<AssociatedField> finAss);
        std::vector<DoubtfulField> addDoubtfulAssociations(char* dataField, std::string mrzDataField, std::string fieldType,
                                                           float confidence, std::vector<DoubtfulField> doubtfulAss);
};
