#include "Levenshtein.hpp"

float Levenshtein::computeConfidence(const std::string& s1, const std::string& s2)
{
    int error = this->computeLevenshtein(s1, s2);
    int maxLenght = std::max(s1.size(), s2.size());
    return 1 - static_cast<float>(error)/static_cast<float>(maxLenght);
}

float Levenshtein::computeLevenshtein(const std::string& s1, const std::string& s2)
{
    unsigned m = s1.size();
    unsigned n = s2.size();
    // d is a table with m+1 rows and n+1 columns
    int *d = new int[(m+1) * (n + 1)];
    #define d(i,j)  d[(i)*(n+1) + (j)]

    // Initialization row0 and colum0
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
                d(i, j) = this->min3(d(i-1, j), d(i, j-1), d(i-1, j-1)) + 1;
            }
        }
    }
    int dist = d(m,n);
    delete [] d;

    return dist;
}

int Levenshtein::min3(int a, int b, int c)
{
    return std::min(std::min(a,b), c);
}
