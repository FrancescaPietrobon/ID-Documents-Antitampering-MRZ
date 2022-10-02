#ifndef _METRICS_H
#define _METRICS_H

#include <iostream>
#include <string>
#include <unordered_map>

enum metricsType
{
    pairs = 1,
    distLev
};


int distanceLevenshtein(const std::string& s0, const std::string& s1);
int min3(int a, int b, int c);

int countPairs(std::string s1, std::string s2);

#endif