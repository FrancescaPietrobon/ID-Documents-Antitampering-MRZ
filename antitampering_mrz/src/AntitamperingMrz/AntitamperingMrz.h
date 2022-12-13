#ifndef _ANTITAMPERING_MRZ_H_
#define _ANTITAMPERING_MRZ_H_

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>
#include "EuclDist/Response.h"

struct AntitamperingMrzData {
  char* imageName;
  //char* imageBase64;
  //char* documentMetaData;
  float confidenceThreshold;
};

class AntitamperingMrz {
    public:
        AntitamperingMrz(){};
        virtual ~AntitamperingMrz(){};
        virtual OcrMrzResultDetail check(const AntitamperingMrzData& data) = 0;
};

#endif /* _ANTITAMPERING_MRZ_H_ */