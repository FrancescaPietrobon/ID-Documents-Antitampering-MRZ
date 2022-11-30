#include "AntitamperingMrzUnitTestSuite.hpp"
#include "src/AntitamperingMrz/EuclDist/Document.h"

#include <opencv2/core/utility.hpp>
#include <spdlog/cfg/env.h>

/*
class MockAntitamperingMrz : public AntitamperingMrz
{
    public:
        MOCK_METHOD(OcrMrzResponseResult, check, (const AntitamperingMrzData& data), (override));
};
*/

class AntitamperingMrzTestFixture : public ::testing::Test {
    protected:
    
    virtual void SetUp() {
        spdlog::cfg::load_env_levels();
        spdlog::enable_backtrace(30);
    }

    virtual void TearDown() {
    }

};


TEST_F(AntitamperingMrzTestFixture, imagePreprocessingUnitTest)
{
    //Set Up
    std::string imagePath = "testData/images/AFG_AO_01001_FRONT.JPG";
    Document document(imagePath, 800, 800, 10);

    //Testing
    cv::Mat imagePreprocessed = document.preprocessing(imagePath, 10);

    //Assertion
    ASSERT_THAT(imagePreprocessed.at<double>(100,100), static_cast<double>(4.7421036049794724e+49)) << TestHelper::PrintTo();
    ASSERT_THAT(imagePreprocessed.at<double>(10,10), static_cast<double>(-6.5849542761516262e-219)) << TestHelper::PrintTo();    
}


/*
TEST_F(AntitamperingMrzTestFixture, imagePreprocessingUnitTest)
{
    //Set Up
    std::string imagePath = "testData/images/AFG_AO_01001_FRONT.JPG";
    Document document(imagePath, 800, 800, 10);

    //Testing
    cv::Mat imagePreprocessed = document.preprocessing(document.getInputImage(), 10);

    //Assertion
    ASSERT_THAT(imagePreprocessed.at<double>(100,100), static_cast<double>(4.7421036049794724e+49)) << TestHelper::PrintTo();
    ASSERT_THAT(imagePreprocessed.at<double>(10,10), static_cast<double>(-6.5849542761516262e-219)) << TestHelper::PrintTo();    
}
*/