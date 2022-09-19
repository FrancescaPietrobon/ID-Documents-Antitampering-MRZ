#include <unordered_map>
#include <string>
#include <algorithm>

std::unordered_map<unsigned, std::string> dictionary =   {{0, "0",},
                                                {1, "1",},
                                                {2, "2",},
                                                {3, "3",},
                                                {4, "4",},
                                                {5, "5",},
                                                {6, "6",},
                                                {7, "7",},
                                                {8, "8",},
                                                {9, "9",},
                                                {10, "A",},
                                                {11, "a",},
                                                {12, "B",},
                                                {13, "b",},
                                                {14, "C",},
                                                {15, "c",},
                                                {16, "D",},
                                                {17, "d",},
                                                {18, "E",},
                                                {19, "e",},
                                                {20, "F",},
                                                {21, "f",},
                                                {22, "G",},
                                                {23, "g",},
                                                {24, "H",},
                                                {25, "h",},
                                                {26, "I",},
                                                {27, "i",},
                                                {28, "J",},
                                                {29, "j",},
                                                {30, "K",},
                                                {31, "k",},
                                                {32, "L",},
                                                {33, "l",},
                                                {34, "M",},
                                                {35, "m",},
                                                {36, "N",},
                                                {37, "n",},
                                                {38, "O",},
                                                {39, "o",},
                                                {40, "P",},
                                                {41, "p",},
                                                {42, "Q",},
                                                {43, "q",},
                                                {44, "R",},
                                                {45, "r",},
                                                {46, "S",},
                                                {47, "s",},
                                                {48, "T",},
                                                {49, "t",},
                                                {50, "U",},
                                                {51, "u",},
                                                {52, "V",},
                                                {53, "v",},
                                                {54, "W",},
                                                {55, "w",},
                                                {56, "X",},
                                                {57, "x",},
                                                {58, "Y",},
                                                {59, "y",},
                                                {60, "Z",},
                                                {61, "z",},
                                                {62, "-",},
                                                {63, "/",},
                                                {64, "<",}};


template<typename K, typename V>
std::unordered_map<V, K> inverse_map(std::unordered_map<K, V> &map)
{
    std::unordered_map<V, K> inv;
    std::for_each(map.begin(), map.end(),
                [&inv] (const std::pair<K, V> &p) {
                    inv.insert(std::make_pair(p.second, p.first));
                });
    return inv;
}

std::unordered_map<std::string, unsigned> inverse_dictionary = inverse_map(dictionary);
