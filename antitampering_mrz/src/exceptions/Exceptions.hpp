#ifndef _ANTITAMPERING_MRZ_EXCEPTIONS_H_
#define _ANTITAMPERING_MRZ_EXCEPTIONS_H_

#include <json/json.h>

namespace AntitamperingMrzErrorCode
{
static const int CORRUPTED_IMAGE = 1001;
static const int BAD_COORDINATES = 1002;
static const int GENERAL_ERROR = 1003;
static const int ANTITAMPERING_MRZ_TYPE_NOT_FOUND = 1004;
}

class AntitamperingMrzException
{

public:
  AntitamperingMrzException (int code, const std::string &message, const Json::Value &data = Json::Value::null)
  {
    if (data.isNull () ) {
      std::string errorType;

      if (code == AntitamperingMrzErrorCode::CORRUPTED_IMAGE) {
        errorType = "CORRUPTED_IMAGE";
      } else if (code == AntitamperingMrzErrorCode::BAD_COORDINATES) {
        errorType = "BAD_COORDINATES";
      }
      else if(code == AntitamperingMrzErrorCode::GENERAL_ERROR) 
      {
        errorType = "GENERAL_ERROR";
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

  int getCode() const
  {
    return code;
  }

  std::string getMessage() const
  {
    return message;
  }

  Json::Value getData() const
  {
    return data;
  }

private:
  int code;
  std::string message;
  Json::Value data;
};

#endif /* _ANTITAMPERING_MRZ_EXCEPTIONS_H_ */