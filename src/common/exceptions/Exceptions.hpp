#pragma once

#include <string>
#include <stdexcept>

namespace ErrorCode
{
    static const int CORRUPTED_IMAGE = 1001;
    static const int BAD_COORDINATES = 1002;
    static const int GENERAL_ERROR = 1003;
    static const int DETECTOR_TYPE_NOT_FOUND = 1004;
}

class Exception
{

public:
    Exception(int code, const std::string &message) : code(code), message(message)
    {
        std::string errorType;

        if (code == ErrorCode::CORRUPTED_IMAGE)
        {
            errorType = "CORRUPTED_IMAGE";
        }
        else if (code == ErrorCode::BAD_COORDINATES)
        {
            errorType = "BAD_COORDINATES";
        }
        else if (code == ErrorCode::GENERAL_ERROR)
        {
            errorType = "GENERAL_ERROR";
        }
        else if (code == ErrorCode::DETECTOR_TYPE_NOT_FOUND)
        {
            errorType = "DETECTOR_TYPE_NOT_FOUND";
        }
        else
        {
            throw std::runtime_error("Error Type not found");
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

private:
    int code;
    std::string message;
};