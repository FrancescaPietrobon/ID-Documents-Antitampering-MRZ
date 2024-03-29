#include "Associations.hpp"

Associations::Associations(float conf):
    confidenceThreshold(conf){};

std::vector<DoubtfulField> Associations::extractDoubtfulFields(const Field *allFields, const size_t fieldsSize)
{
    SplitFields splitter;
    std::vector<Field> mrzLines = splitter.findMrzLines(allFields, fieldsSize);
    SPDLOG_INFO("Extract Mrz Fields");
    std::vector<MrzField> mrzFields = splitter.extractMrzFields(mrzLines);
    std::vector<Field> fields = splitter.extractFieldsWithoutMrz(allFields, fieldsSize, mrzLines);
    this->CheckDigitsResult = splitter.getCheckDigitsResult();
    SPDLOG_INFO("Compute Associations");
    std::pair<std::vector<AssociatedField>, std::vector<DoubtfulField>> associations = this->computeAssociations(fields, mrzFields);
    return associations.second;
}

std::pair<std::vector<AssociatedField>, std::vector<DoubtfulField>>
Associations::computeAssociations(std::vector<Field> fields, std::vector<MrzField> mrzFields)
{
    float currentConf = 0, maxConf = 0;
    std::string bestTypeField, bestField;
    Levenshtein levenshtein;

    SPDLOG_INFO("Compare fields:");
    for(auto & field: fields)
    {
        SPDLOG_INFO("Data field: {}", field.label);
        field = this->convertIfDate(field);

        maxConf = 0;
        for(size_t itFields = 0; itFields < mrzFields.size(); ++itFields)
        {
            currentConf = levenshtein.computeConfidence(field.label, mrzFields[itFields].mrzDataField);
            SPDLOG_INFO("Mrz field: {:<10} Type: {:<15} Confidence: {}", mrzFields[itFields].mrzDataField, mrzFields[itFields].fieldType, currentConf);
            if(currentConf > maxConf)
            {
                maxConf = currentConf;
                bestTypeField = mrzFields[itFields].fieldType;
                bestField = mrzFields[itFields].mrzDataField;
            }
            if(currentConf == 1)
            {
                mrzFields.erase(mrzFields.begin()+itFields);
                ++itFields;
                break;
            }
        }
        SPDLOG_INFO("Max confidence {} for Data field {} and MRZ field {}", maxConf, field.label, bestField);
        std::pair<std::vector<AssociatedField>, std::vector<DoubtfulField>> associations;
        associations = addAssociation(field, bestField, bestTypeField, maxConf, this->doubtfulAss, this->finAss);
        this->finAss = associations.first;
        this->doubtfulAss = associations.second;
    }
    return std::make_pair(this->finAss, this->doubtfulAss);
}

std::pair<std::vector<AssociatedField>, std::vector<DoubtfulField>>
Associations::addAssociation(Field dataField, std::string mrzDataField, std::string fieldType, float confidence,
                             std::vector<DoubtfulField> &doubtfulAss, std::vector<AssociatedField> &finAss)
{
    if(!this->findField(dataField, mrzDataField, fieldType, confidence, doubtfulAss, finAss) && (confidence > 0))
    {
        if(confidence == 1)
            finAss = this->addFinalAssociation(dataField, mrzDataField, fieldType, confidence, finAss);
        else
            doubtfulAss = this->addDoubtfulAssociations(dataField.label, mrzDataField, fieldType, confidence, doubtfulAss);
    }
    return std::make_pair(finAss, doubtfulAss);
}

Field Associations::convertIfDate(Field field)
{
    if((field.labelSize >= 6) && (field.labelSize <= 12))
    {
        std::string dataField = field.label;
        Date date;
        // If it is a date, it is refactorized in format yymmdd.
        std::string newLabel = date.findDate(dataField);
        if(newLabel != "None")
            field.label = utils::convertStringtoCharPtr(newLabel);
    }
    return field;
}

std::vector<AssociatedField>
Associations::addFinalAssociation(Field dataField, std::string mrzDataField, std::string fieldType,
                                  float confidence, std::vector<AssociatedField> finAss)
{
    AssociatedField association;
    association.fieldType = utils::convertStringtoCharPtr(fieldType);
    association.field = dataField;
    association.mrzDataField = utils::convertStringtoCharPtr(mrzDataField);
    association.confidenceField = confidence;

    std::string labelField = dataField.label;
    SPDLOG_INFO("Added in final association: {} - {}", labelField, mrzDataField);
    finAss.push_back(association);

    return finAss;
}

std::vector<DoubtfulField>
Associations::addDoubtfulAssociations(char* dataField, std::string mrzDataField, std::string fieldType,
                                      float confidence, std::vector<DoubtfulField> doubtfulAss)
{
    DoubtfulField association;
    association.fieldType = utils::convertStringtoCharPtr(fieldType);
    association.dataField = dataField;
    association.mrzDataField = utils::convertStringtoCharPtr(mrzDataField);
    association.confidenceField = confidence;

    SPDLOG_INFO("Added in doubtful association: {} - {}", dataField, mrzDataField);
    doubtfulAss.push_back(association);

    return doubtfulAss;
}

bool Associations::findField(Field dataField, std::string mrzDataField, std::string fieldType, float confidence,
                             std::vector<DoubtfulField> &doubtfulAss, std::vector<AssociatedField> &finAss)
{
    bool found = false;
    for(size_t itDoubtFilds = 0; itDoubtFilds < doubtfulAss.size(); ++itDoubtFilds)
    {
        if((doubtfulAss[itDoubtFilds].fieldType == fieldType))
        {
            found = true;
            if(doubtfulAss[itDoubtFilds].confidenceField < confidence)
            {
                SPDLOG_INFO("Prev conf: {}\t curr conf: {}", doubtfulAss[itDoubtFilds].confidenceField, confidence);
                SPDLOG_INFO("Field to remove: {}\t MRZ Field: {}\t MRZ Field type:",
                            doubtfulAss[itDoubtFilds].dataField, doubtfulAss[itDoubtFilds].mrzDataField, doubtfulAss[itDoubtFilds].fieldType);
                doubtfulAss.erase(doubtfulAss.begin()+itDoubtFilds);
                ++itDoubtFilds;
                if(confidence == 1)
                    finAss = this->addFinalAssociation(dataField, mrzDataField, fieldType, confidence, finAss);
                else
                    doubtfulAss = this->addDoubtfulAssociations(dataField.label, mrzDataField, fieldType, confidence, doubtfulAss);
                
                break;
            }
        }
    }
    return found;
}

float Associations::computeConfFinal(std::vector<DoubtfulField> doubtfulAss)
{
    float confFinal = 0;
    float sum = 0;

    // Confidence of doubtful fields
    for(auto ass: doubtfulAss)
        sum += ass.confidenceField;

    // Confidence of perfect associations
    for(auto ass: this->finAss)
        sum += ass.confidenceField;

    // Confidence of check digits (1 true, 0 false)
    sum += CheckDigitsResult;

    confFinal = sum / (doubtfulAss.size() + this->finAss.size() + 1);
    return confFinal;
}
