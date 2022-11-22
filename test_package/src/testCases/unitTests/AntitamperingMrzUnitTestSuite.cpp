#include "AntitamperingMrzUnitTestSuite.hpp"
#include "../../antitampering_mrz/AntitamperingMrz/AntitamperingMrz.h"
#include "../../antitampering_mrz/antitamperingMrzApi.hpp"
#include "../../antitampering_mrz/exceptions/Exceptions.hpp"
#include "../../antitampering_mrz/AntitamperingMrz/DBSCAN/AntitamperingMrzDBSCAN.h"

#include <opencv2/core/utility.hpp>
//#include "spdlog/cfg/env.h"
#include <spdlog/spdlog.h>

class MockAntitamperingMrz : public AntitamperingMrz
{
    public:
        MOCK_METHOD(OcrMrzResponseResult, check, (const AntitamperingMrzData& data), (override));
};

class AntitamperingMrzTestFixture : public ::testing::Test {
    protected:
    /*
    virtual void SetUp() {
        spdlog::cfg::load_env_levels();
        spdlog::enable_backtrace(30);
    }

    virtual void TearDown() {
    }
    */
};