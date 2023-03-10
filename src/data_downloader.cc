#include "data_downloader.h"

bool DataDownloader::Download(std::vector<uint8_t> &data) {
    curl_easy_setopt(curl_, CURLOPT_URL, hostname_.c_str());
    curl_easy_setopt(curl_, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &data);
    curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, &DataDownloader::CURLWriteFunction);
    auto res = curl_easy_perform(curl_);
    if(res != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        throw std::runtime_error("");
    }
    return res == CURLE_OK;
}

size_t DataDownloader::CURLWriteFunction(void *ptr, size_t size, size_t nmemb, void *user_data) {
    size_t num_bytes = size * nmemb;
    std::vector<uint8_t> *data = static_cast<std::vector<uint8_t>*>(user_data);
    uint8_t *bytes = static_cast<uint8_t*>(ptr);
    data->insert(data->end(), bytes, bytes + num_bytes);
    return num_bytes;
}