#ifndef __DATA_ANALYZER_H__
#define __DATA_ANALYZER_H__

#include "utils.h"

#include <cassert>

/*
    Class: DataAnalyzer

    This class provides an interface to analyze a given set of data for similarity with the previously analyzed data.

    Member Variables:
        - source_size_ : Int variable to store the source size of the data.
        - identical_size_ : Int variable to store the identical size of the data.
        - prev_data_ : Vector of uint8_t to store the previously analyzed data.
        - prev_difference_ : Vector of uint8_t to store the difference between the previously analyzed data and the current data.

    Member Functions:
        - Constructor: Initializes the data members of the class using the given source size and identical size
        - Destructor: Cleans up any resources created during the lifetime of the object
        - int get_source_size(): Returns the source size of the data
        - int get_identical_size(): Returns the identical size of the data
        - bool isPrefixIdentical(const std::vector<uint8_t>& b1, const std::vector<uint8_t>& b2):
*/
class DataAnalyzer {

    private:

        int source_size_;
        int identical_size_;
        std::vector<uint8_t> prev_data_, prev_difference_;

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

        void Calculate(std::vector<uint8_t> curr_data, float &output_score);
};

#endif // __DATA_ANALYZER_H__