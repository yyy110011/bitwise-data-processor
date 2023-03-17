#include "data_analyzer.h"

/*
    - Input: Two vectors of uint8_t
    - Output: Boolean value indicating whether the two vectors have an identical prefix of length identicalsize.
    - Description: This function checks whether the given vectors have an identical prefix of length identicalsize`.
                    It is used to identify whether the current data has any similarity with the previously analyzed data.
*/
bool DataAnalyzer::isPrefixIdentical(const std::vector<uint8_t>& b1, const std::vector<uint8_t>& b2) {
    bool output = true;
    for (size_t i = 0; i < identical_size_; i++) {
        output &= !(b1[i] ^ b2[i]);
    }
    return output;
}

/*
    - Input: Two vectors of uint8_t
    - Output: A vector of uint8_t representing the difference between the two vectors
    - Description: This function computes the difference between two vectors by comparing each element at the same index.
                    It is used to calculate the difference between the current data and the previously analyzed data.
*/
std::vector<uint8_t> DataAnalyzer::DoCompare(const std::vector<uint8_t>& b1, const std::vector<uint8_t>& b2) {
    std::vector<uint8_t> output;
    for (size_t i = 0; i < source_size_; i++) {
        output.push_back(b1[i]^b2[i]);
    }
    return output;
}

/*
    - Input: Two vectors of uint8_t
    - Output: A float value representing the score of similarity between two vectors
    - Description: This function calculates the score of similarity between two vectors.
                    It is used to provide a measure of how similar the current data is to the previously analyzed data.
*/
float DataAnalyzer::CalScore(const std::vector<uint8_t>& c1, const std::vector<uint8_t>& c2) {
    float sum = 0;
    int counter = 0;
    float output = 0;
    int source_size_in_bits = source_size_ << BYTE_DIGIT;
    if (isPrefixIdentical(c1, c2)) {
        for (size_t i = identical_size_; i < source_size_; i++) {
            uint8_t curr = 0;
            uint8_t mask = 0x80;
            for (size_t j = 0; j < 8; j++) {
                auto idx = i * 8 + j;
                assert(idx == counter + (identical_size_ << BYTE_DIGIT));
                bool diff = !((c1[i] & mask) ^ (c2[i] & mask));
                if (diff) {
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

/*
    - Input: A vector of uint8_t representing the current data, and A float reference variable output_score representing the calculated score.
    - Output: None
    - Description: This function analyzes the given data for its similarity with the previously analyzed data.
                    It calculates the difference between the current data and the previously analyzed data,
                    and then it calculates the score of similarity between the two using the CalScore function.
                    Finally, it updates the prev_data_ and prev_difference_ variables with the current data and difference respectively.
                    It also sets the value of output_score to the calculated similarity score.
*/
void DataAnalyzer::Calculate(std::vector<uint8_t> curr_data, float &output_score) {
    if (!prev_data_.empty()) {
        auto curr_differece = DoCompare(prev_data_, curr_data);
        if (!prev_difference_.empty()) {
            output_score = CalScore(prev_difference_, curr_differece);
        }
        prev_difference_ = curr_differece;
    }
    prev_data_ = curr_data;
}