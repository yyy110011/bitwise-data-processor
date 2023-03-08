#include "data_analyzer.h"

bool DataAnalyzer::isPrefixIdentical(const std::vector<uint8_t>& b1, const std::vector<uint8_t>& b2)
{
    bool output = true;
    for (size_t i = 0; i < identical_size_; i++)
    {
        output &= !(b1[i] ^ b2[i]);
    }
    return output;
}
    
std::vector<uint8_t> DataAnalyzer::DoCompare(const std::vector<uint8_t>& b1, const std::vector<uint8_t>& b2)
{
    std::vector<uint8_t> output;
    for (size_t i = 0; i < source_size_; i++)
    {
        output.push_back(b1[i]^b2[i]);
    }
    return output;
}

float DataAnalyzer::CalScore(const std::vector<uint8_t>& c1, const std::vector<uint8_t>& c2)
{
    float sum = 0;
    int counter = 0;
    float output = 0;
    int source_size_in_bits = source_size_ << BYTE_DIGIT;
    if (isPrefixIdentical(c1, c2))
    {
        for (size_t i = identical_size_; i < source_size_; i++)
        {
            uint8_t curr = 0;
            uint8_t mask = 0x80;
            for (size_t j = 0; j < 8; j++)
            {
                auto idx = i * 8 + j;
                assert(idx == counter + (identical_size_ << BYTE_DIGIT));
                bool diff = !((c1[i] & mask) ^ (c2[i] & mask));
                if (diff)
                {
                    sum += source_size_in_bits - counter;
                }
                counter++;
                mask = mask >> 1;
            }
        }
        output = sum / (float)source_size_in_bits;
    }
    return output;
}