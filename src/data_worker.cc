#include "data_worker.h"

bool DataWorker::Download()
{
    curl_easy_setopt(curl_, CURLOPT_URL, config_.url.c_str());
    curl_easy_setopt(curl_, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl_, CURLOPT_WRITEDATA, this);
    curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, &DataWorker::CURLWriteFunction);
    auto res = curl_easy_perform(curl_);
    if(res != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        throw std::runtime_error("");
    }
    return res == CURLE_OK;
}

size_t DataWorker::CURLWriteFunction(char *data, size_t size, size_t nmemb, void *userp)
{
    DataWorker* dp = static_cast<DataWorker*>(userp);
    return dp->Task(data, size * nmemb);
}

size_t DataWorker::Task(char* data, size_t size)
{    
    std::vector<uint8_t> curr_data(data, data + size);
    if (!prev_data.empty())
    {
        auto curr_differece = data_analyzer_->DoCompare(prev_data, curr_data);
        if (!prev_difference.empty())
        {
            auto score = data_analyzer_->CalScore(prev_difference, curr_differece);
            auto time = getUNIXTimestamp();

            std::lock_guard<std::mutex> lock(score_queue_->mutex);
            score_queue_->queue.emplace_back(time, score);
            log_info("[Worker] timestamp: %lld, Score: %f", time, score);
        }
        prev_difference = curr_differece;
    }
    prev_data = curr_data;
    return size;
}
