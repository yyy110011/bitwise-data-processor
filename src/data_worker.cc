#include "data_worker.h"

void DataWorker::InitCurl() {
    curl_easy_setopt(curl_, CURLOPT_URL, config_.url.c_str());
    curl_easy_setopt(curl_, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl_, CURLOPT_WRITEDATA, this);
    curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, &DataWorker::CURLWriteFunction);
}

bool DataWorker::Download() {
    auto res = curl_easy_perform(curl_);
    if(res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        throw std::runtime_error("");
    }
    return res == CURLE_OK;
}

size_t DataWorker::CURLWriteFunction(char *data, size_t size, size_t nmemb, void *userp) {
    DataWorker* dp = static_cast<DataWorker*>(userp);
    return dp->Task(data, size * nmemb);
}

size_t DataWorker::Task(char* data, size_t size) {    
    std::vector<uint8_t> curr_data(data, data + size);
    float score = 0;

    std::lock_guard<std::mutex> lock(score_queue_->mutex);
    data_analyzer_->Calculate(curr_data, score);
    score_queue_->queue.emplace_back(time, score);
    auto time = getUNIXTimestamp();
    log_info("[Worker] timestamp: %lld, Score: %f", time, score);

    return size;
}
