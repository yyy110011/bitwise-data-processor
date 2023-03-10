#ifndef __WORKER_H__
#define __WORKER_H__

#include "utils.h"
#include "data_analyzer.h"

#include <memory>
#include <mutex>
#include <curl/curl.h>

struct DataWorkerConfig
{
    std::string url;
    int source_size;
    int identical_size;
};

class DataWorker
{
    private:

        CURL* curl_;
        struct DataWorkerConfig config_;
        std::unique_ptr<DataAnalyzer> data_analyzer_;

        std::shared_ptr<shared_queue<std::pair<int64_t, float>>> score_queue_;

    public:

        DataWorker(std::shared_ptr<shared_queue<std::pair<int64_t, float>>> shared_score_queue,
                        struct DataWorkerConfig cfg): 
                        score_queue_(shared_score_queue), config_(cfg) {
            data_analyzer_ = std::make_unique<DataAnalyzer>(config_.source_size, config_.identical_size);
            curl_ = curl_easy_init();
            InitCurl();
        };

        ~DataWorker(){
            curl_easy_cleanup(curl_);
        };

        void InitCurl();

        bool Download();

        // Define a callback function to write data to the vector
        static size_t CURLWriteFunction(char *data, size_t size, size_t nmemb, void *userp);

        size_t Task(char* data, size_t size);
};
#endif // __WORKER_H__