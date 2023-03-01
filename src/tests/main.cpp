#include "gtest/gtest.h"
#include <string>

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#define SPDLOG_FMT_EXTERNAL 1
#include <spdlog/fmt/fmt.h>

int main(int argc, char **argv) {
    setenv("OPENCV_THREADS_NUM", "1" ,1);
    setenv("SPDLOG_LEVEL", "critical",1);
    setenv("PHYGITALAI_MAIN_LOG_LEVEL", "critical",1);
    setenv("PHYGITALAI_KPI_LOG_LEVEL", "critical",1);
    setenv("PHYGITALAI_MAIN_LOG_LEVEL", "critical",1);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}