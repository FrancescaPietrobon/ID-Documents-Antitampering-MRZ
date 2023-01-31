#ifndef _OCR_EXCEPTIONS_H_
#define _OCR_EXCEPTIONS_H_

#include <json/json.h>

namespace OcrErrorCode
{
static const int CORRUPTED_IMAGE = 1001;
static const int BAD_COORDINATES = 1002;
static const int GENERAL_ERROR = 1003;
static const int OCR_TYPE_NOT_FOUND = 1004;
}

class OcrException
{

public:
  OcrException (int code, const std::string &message, const Json::Value &data = Json::Value::null)
  {
    this->code = code;
    this->message = message;
    this->data = data;
    
    if (data.isNull () ) {
      std::string errorType;

      if (code == OcrErrorCode::CORRUPTED_IMAGE) {
        errorType = "CORRUPTED_IMAGE";
      } else if (code == OcrErrorCode::BAD_COORDINATES) {
        errorType = "BAD_COORDINATES";
      }
      else if(code == OcrErrorCode::GENERAL_ERROR) {
        errorType = "GENERAL_ERROR";
      }
      else if(code == OcrErrorCode::OCR_TYPE_NOT_FOUND) 
      {
        errorType = "OCR_TYPE_NOT_FOUND";
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

#endif /* _OCR_EXCEPTIONS_H_ */