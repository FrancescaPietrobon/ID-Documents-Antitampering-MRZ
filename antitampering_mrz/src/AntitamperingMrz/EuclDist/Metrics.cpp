#include "Metrics.h"


float computeConfidence(metricsType metric, const std::string& s1, const std::string& s2)
{
    if(metric == pairs)
        return confCountPairs(s1, s2);
    else if(metric == WER)
        return confWER(s1, s2);
}

//https://github.com/bangxiang/LevenshteinDistance/blob/master/LevenshteinDistance.hh

int min3(int a, int b, int c) {
    return std::min(std::min(a,b), c);
}


int computeWER(const std::string& s1, const std::string& s2)
{
    unsigned m = s1.size();
    unsigned n = s2.size();
    // d is a table with m+1 rows and n+1 columns
    int *d = new int[(m+1) * (n + 1)];
    #define d(i,j)  d[(i)*(n+1) + (j)]

    // initialization row0 and colum0
    for( unsigned i = 0; i <= m; ++i) {
        d(i,0) = i;
    }
    for( unsigned i = 0; i <= n; ++i) {
        d(0,i) = i;
    }

    for( unsigned j = 1; j <= n; ++j) {
      for (unsigned i = 1; i <= m; ++i) {
            if (s1[i - 1] == s2[j - 1]) {
                d(i, j) = d(i-1, j-1);
            } else {
                d(i, j) = min3(
                            d(i-1, j),
                            d(i, j-1),
                            d(i-1, j-1)
                          ) + 1;
            }
        }
    }
    int dist = d(m,n);
    delete [] d;

    return dist;
}

float confWER(const std::string& s1, const std::string& s2)
{
    int error = computeWER(s1, s2);
    int maxLenght = std::max(s1.size(), s2.size());
    return 1 - static_cast<float>(error)/static_cast<float>(maxLenght);
}


int countPairs(const std::string& s1, const std::string& s2)
{
    extern std::unordered_map<std::string, unsigned> inverse_dictionary;

    // To store the frequencies of characters of string s1 and s2
    int freq1[64] = {0};
    int freq2[64] = {0};
 
    // To store the count of valid pairs
    int count = 0;
    // Update the frequencies of the characters of string s1
    for (size_t i = 0; i != s1.size(); ++i)
    {
        std::string s(1, s1[i]);
        freq1[unsigned(inverse_dictionary[s])]++;
    }
 
    // Update the frequencies of the characters of string s2
    for (size_t i = 0; i != s2.size(); ++i)
    {
        std::string s(1, s2[i]);
        freq2[unsigned(inverse_dictionary[s])]++;
    }
 
    // Find the count of valid pairs
    for (size_t i = 0; i != 64; ++i)
        count += (std::min(freq1[i], freq2[i]));
 
    return count;
}

float confCountPairs(const std::string& s1, const std::string& s2)
{
    int count = countPairs(s1, s2);
    int maxLenght = std::max(s1.size(), s2.size());
    return static_cast<float>(count)/static_cast<float>(maxLenght);
}