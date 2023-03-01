#include "Associations.hpp"

Associations::Associations(float conf):
    confidenceThreshold(conf){};

std::vector<DoubtfulFields> Associations::extractDoubtfulFields(const Fields *allFields, const size_t fieldsSize)
{
    SplitFields splitter;
    std::vector<Fields> mrzLines = splitter.findMrzLines(allFields, fieldsSize);
    std::vector<MrzFields> mrzFields = splitter.extractMrzFields(mrzLines);
    std::vector<Fields> fields = splitter.extractFieldsWithoutMrz(allFields, fieldsSize, mrzLines);
    std::pair<std::vector<AssociatedField>, std::vector<DoubtfulFields>> associations = computeAssociations(fields, mrzFields);
    return associations.second;
}

std::pair<std::vector<AssociatedField>, std::vector<DoubtfulFields>> Associations::computeAssociations(std::vector<Fields> fields, std::vector<MrzFields> mrzFields)
{
    float currentConf = 0, maxConf = 0;
    std::string bestTypeField, bestField;
    WER wer;

    std::cout << "\nCompare fields:" << std::endl;

    for(auto & field: fields)
    {
        std::cout << "\nField: " << field.label << "\n" << std::endl;

        if((field.labelSize >= 6) && (field.labelSize <= 12))
            field = convertIfDate(field);
    
        maxConf = 0;

        std::cout << std::left << std::setw(20) << "MRZ FIELD";
        std::cout << std::left << std::setw(20) << "MRZ FIELD TYPE";
        std::cout << std::left << std::setw(20) << "CONFIDENCE" << std::endl;

        for(size_t itFields = 0; itFields < mrzFields.size(); ++itFields)
        {
            currentConf = wer.computeConfidence(field.label, mrzFields[itFields].mrzDataField);

            std::cout << std::left << std::setw(20) << mrzFields[itFields].mrzDataField;
            std::cout << std::left << std::setw(20) << mrzFields[itFields].fieldType;
            std::cout << std::left << std::setw(20) << currentConf << std::endl;

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
        std::cout << "Max confidence: " << maxConf << std::endl;

        if(!findField(field, bestField, bestTypeField, maxConf, doubtfulAss, finAss) && (maxConf > 0))
        {
            if(maxConf == 1)
                finAss = addFinalAssociation(field, bestField, bestTypeField, maxConf, finAss);
            else
                doubtfulAss = addDoubtfulAssociations(field.label, bestField, bestTypeField, maxConf, doubtfulAss);
        }

        std::cout << std::endl;
    }

    return std::make_pair(finAss, doubtfulAss);
}

Fields Associations::convertIfDate(Fields field)
{
    std::string dataField = field.label;
    Date date;
    
    // If it is a date, it is refactorized in format yymmdd.
    std::string newLabel = date.findDate(dataField);
    if(newLabel != "None")
        field.label = utils::convertStringtoCharPtr(newLabel);

    return field;
}

std::vector<AssociatedField> Associations::addFinalAssociation(Fields dataField, std::string mrzDataField, std::string fieldType, float confidence, std::vector<AssociatedField> finAss)
{
    AssociatedField association;
    association.fieldType = utils::convertStringtoCharPtr(fieldType);
    association.field = dataField;
    association.mrzDataField = utils::convertStringtoCharPtr(mrzDataField);
    association.confidenceField = confidence;

    std::string labelField = dataField.label;
    std::cout << "Added in final association: " << labelField << " - " << mrzDataField << std::endl;
    finAss.push_back(association);

    return finAss;
}

std::vector<DoubtfulFields> Associations::addDoubtfulAssociations(char* dataField, std::string mrzDataField, std::string fieldType, float confidence, std::vector<DoubtfulFields> doubtfulAss)
{
    DoubtfulFields association;
    association.fieldType = utils::convertStringtoCharPtr(fieldType);
    association.dataField = dataField;
    association.mrzDataField = utils::convertStringtoCharPtr(mrzDataField);
    association.confidenceField = confidence;

    std::cout << "Added in doubtful association: " << dataField << " - " << mrzDataField  << std::endl;
    doubtfulAss.push_back(association);

    return doubtfulAss;
}

bool Associations::findField(Fields dataField, std::string mrzDataField, std::string fieldType, float confidence, std::vector<DoubtfulFields> &doubtfulAss, std::vector<AssociatedField> &finAss)
{
    bool found = false;
    for(size_t itDoubtFilds = 0; itDoubtFilds < doubtfulAss.size(); ++itDoubtFilds)
    {
        if((doubtfulAss[itDoubtFilds].fieldType == fieldType))
        {
            found = true;
            if(doubtfulAss[itDoubtFilds].confidenceField < confidence)
            {
                std::cout << "Prev conf: "<< doubtfulAss[itDoubtFilds].confidenceField << "\t curr conf: " << confidence << std::endl;
                std::cout << "Field to remove: " << doubtfulAss[itDoubtFilds].dataField << "   MRZ Field: " << doubtfulAss[itDoubtFilds].mrzDataField << "   MRZ Field type: " << doubtfulAss[itDoubtFilds].fieldType << std::endl;
                doubtfulAss.erase(doubtfulAss.begin()+itDoubtFilds);
                ++itDoubtFilds;

                if(confidence == 1)
                    finAss = addFinalAssociation(dataField, mrzDataField, fieldType, confidence, finAss);
                else
                    doubtfulAss = addDoubtfulAssociations(dataField.label, mrzDataField, fieldType, confidence, doubtfulAss);
                
                break;
            }
        }
    }
    return found;
}

float Associations::computeConfFinal(std::vector<DoubtfulFields> doubtfulAss)
{
    float confFinal = 0;
    float sum = 0;

    // Confidence of doubtful fields
    for(auto ass: doubtfulAss)
        sum += ass.confidenceField;

    // Confidence of perfect associations
    for(auto ass: finAss)
        sum += ass.confidenceField;

    confFinal = sum / (doubtfulAss.size() + finAss.size());
    return confFinal;
}