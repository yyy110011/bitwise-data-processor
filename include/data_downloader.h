#ifndef __DATA_DONLOADER_H__
#define __DATA_DONLOADER_H__

#include <curl/curl.h>
#include <string>
#include <vector>

class DataDownloader {
    private:

        CURL *curl_;
        std::string hostname_;

    public:

        DataDownloader(std::string hostname): hostname_(hostname){};
        ~DataDownloader(){};
        bool Download(std::vector<uint8_t> &data);

        // Define a callback function to write data to the vector
        static size_t CURLWriteFunction(void *ptr, size_t size, size_t nmemb, void *user_data);
        size_t Task(char* data, size_t size);
};

#endif // __DATA_DONLOADER_H__