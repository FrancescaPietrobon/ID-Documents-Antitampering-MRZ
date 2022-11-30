#ifndef _ANTITAMPERING_MRZ_EXCEPTIONS_H_
#define _ANTITAMPERING_MRZ_EXCEPTIONS_H_

#include <json/json.h>

namespace AntitamperingMrzErrorCode
{
static const int PARSE_ERROR = 1001;
static const int DOCUMENT_NOT_FOUND = 1002;
static const int NOT_COMPLIANT_METADATA = 1003;
static const int ANTITAMPERING_MRZ_TYPE_NOT_FOUND = 1004;
}

class AntitamperingMrzException
{

public:
  AntitamperingMrzException (int code, const std::string &message, const Json::Value &data = Json::Value::null)
  {
    this->code = code;
    this->message = message;
    this->data = data;

    if (data.isNull () ) {
      std::string errorType;

      if (code == AntitamperingMrzErrorCode::PARSE_ERROR) {
        errorType = "PARSE_ERROR";
      } else if (code == AntitamperingMrzErrorCode::DOCUMENT_NOT_FOUND) {
        errorType = "DOCUMENT_NOT_FOUND";
      }
      else if(code == AntitamperingMrzErrorCode::NOT_COMPLIANT_METADATA) 
      {
        errorType = "NOT_COMPLIANT_METADATA";
      }
      else if(code == AntitamperingMrzErrorCode::ANTITAMPERING_MRZ_TYPE_NOT_FOUND) 
      {
        errorType = "ANTITAMPERING_MRZ_TYPE_NOT_FOUND";
      }
      if (!errorType.empty() ) {
        this->data["type"] = errorType;
      }
    }
  }

  int getCode()
  {
    return code;
  }

  std::string getMessage()
  {
    return message;
  }

  Json::Value getData()
  {
    return data;
  }

private:
  int code;
  std::string message;
  Json::Value data;
};

#endif /* _ANTITAMPERING_MRZ_EXCEPTIONS_H_ */