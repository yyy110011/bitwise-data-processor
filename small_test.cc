#include "data_analyzer.h"

#include <iostream>
#include <vector>
#include <bitset>

bool BitCompare(std::vector<uint8_t> a, std::vector<std::bitset<8>> b)
{
    for (size_t i = 0; i < a.size(); i++)
    {
        auto curr = std::bitset<8>(a[i]);
        if (curr != b[i])
        {
            return false;
        }
    }
    return true;
}

int main()
{
    int source_size, identical_size;
    DataAnalyzer da{3, 1};

    std::vector<uint8_t> a = {0b00000000, 0b11001100, 0b11110000};
    std::vector<uint8_t> b = {0b00000000, 0b00110011, 0b00110011};
    std::vector<std::bitset<8>> expect_result = {0b00000000, 0b11111111, 0b11000011};

    // Test Compare
    auto result = da.DoCompare(a, b);
    if (BitCompare(result, expect_result))
        std::cout << "PASS" << '\n';
    else
        std::cout << "FAIL" << '\n';

    // Test Score
    auto score = da.CalScore(a, b);
    if (score - (50/24.0) < 0.000001)
        std::cout << "PASS" << '\n';
    else
        std::cout << "FAIL" << '\n';

    return 0;
}