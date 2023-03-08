#ifndef __DATA_ANALYZER_H__
#define __DATA_ANALYZER_H__

#include "utils.h"

#include <cassert>

class DataAnalyzer 
{
    private:

        int source_size_;
        int identical_size_;

    public:

        DataAnalyzer(int source_size, int identical_size): source_size_(source_size), identical_size_(identical_size){
            if (identical_size_ > source_size_) {
                throw std::out_of_range("identical_size_ should less or equal to source_size_.");
            }
        };

        ~DataAnalyzer(){};

        int get_source_size(){ return source_size_;};
        int get_identical_size(){ return identical_size_;};

        bool isPrefixIdentical(const std::vector<uint8_t>& b1, const std::vector<uint8_t>& b2);

        std::vector<uint8_t> DoCompare(const std::vector<uint8_t>& b1, const std::vector<uint8_t>& b2);

        float CalScore(const std::vector<uint8_t>& c1, const std::vector<uint8_t>& c2);
};
#endif // __DATA_ANALYZER_H__