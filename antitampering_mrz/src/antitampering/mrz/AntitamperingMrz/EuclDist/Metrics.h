#ifndef _METRICS_H_
#define _METRICS_H_

#include <iostream>
#include <string>
#include <unordered_map>

enum metricsType
{
    pairs = 1,
    WER
};

float computeConfidence(metricsType metric, const std::string& s1, const std::string& s2);

int computeWER(const std::string& s1, const std::string& s2);
int min3(int a, int b, int c);
float confWER(const std::string& s1, const std::string& s2);

int countPairs(const std::string& s1, const std::string& s2);
float confCountPairs(const std::string& s1, const std::string& s2);

#endif