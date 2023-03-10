#ifndef __DATA_SENDER_H__
#define __DATA_SENDER_H__

#include "utils.h"

#include <string>
#include <memory>
#include <mutex>
#include <curl/curl.h>

struct DataSenderConfig {
    std::string channel_id;
    std::string hostname;
    int output_amount;
};

class DataSender {

    private:

        CURL *curl_;
        int output_amount_;
        struct DataSenderConfig cfg_;
        std::vector<std::pair<int64_t, float>> sending_queue_;
        std::shared_ptr<shared_queue<std::pair<int64_t, float>>> score_queue_;

    public:

        DataSender(std::shared_ptr<shared_queue<std::pair<int64_t, float>>> shared_score_queue, 
                    struct DataSenderConfig data_sender_config): 
                    score_queue_(shared_score_queue), cfg_(data_sender_config){
            curl_ = curl_easy_init();
        };

        ~DataSender(){ curl_easy_cleanup(curl_); };

        void ClearScoreQueue() { score_queue_.get()->queue.clear(); };

        void ClearSendingQueue() {
            sending_queue_.clear();
        };

        void CopyToInternalQueue();
        void SendData();
        bool DoHttpPOST(std::string &data);
};

#endif // __DATA_SENDER_H__