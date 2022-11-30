#include "AntitamperingMrzUnitTestSuite.hpp"
#include "src/AntitamperingMrz/EuclDist/Document.h"
#include "src/AntitamperingMrz/EuclDist/AntitamperingMrzEuclDist.h"

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
    cv::Mat imagePreprocessed = document.preprocessing();

    //Assertion
    ASSERT_THAT(imagePreprocessed.at<double>(100,100), static_cast<double>(4.7421036049794724e+49)) << TestHelper::PrintTo();
    ASSERT_THAT(imagePreprocessed.at<double>(10,10), static_cast<double>(-6.5849542761516262e-219)) << TestHelper::PrintTo();    
}



TEST_F(AntitamperingMrzTestFixture, predictFromModelUnitTest)
{
    //Set Up
    std::string networkPath = "/home/f_pietrobon/thesis/MRZ_Antitampering/antitampering_mrz/models/Frozen_graph_lnorm_5e6_156img.pb";
    std::string imagePath = "testData/images/AFG_AO_01001_FRONT.JPG";

    Document document(imagePath, 800, 800, 10);
    cv::Mat imagePreprocessed = document.preprocessing();

    AntitamperingMrzEuclDist antitampering(networkPath);

    //Testing
    std::pair<matrix2D, std::vector<float>> result = antitampering.predictFromModel(document, 63, 0.3, 0.005);
    matrix2D pred = result.first;
    std::vector<float> probabilities = result.second;

    //Assertion
    ASSERT_THAT(pred.size(), 185) << TestHelper::PrintTo();
    ASSERT_THAT(pred[1].size(), 4) << TestHelper::PrintTo();
    ASSERT_THAT(pred[10][0], 382.317) << TestHelper::PrintTo();
    ASSERT_THAT(probabilities.size(), 185) << TestHelper::PrintTo();
    ASSERT_THAT(probabilities[10], 62) << TestHelper::PrintTo(); 
}
