#include "data_sender.h"

void DataSender::CopyToInternalQueue()
{
    sending_queue_.insert(sending_queue_.end(), score_queue_.get()->queue.begin(), score_queue_.get()->queue.end());
}

void DataSender::SendData()
{
    while (true)
    {
        std::lock_guard<std::mutex> lock(score_queue_.get()->mutex);
        if (!score_queue_.get()->queue.empty())
        {
            if (score_queue_.get()->queue.size() >= cfg_.output_amount)
            {
                auto time = getUNIXTimestamp();
                CopyToInternalQueue();
                ClearScoreQueue();
                
                // Generate string for Http POST, "channel_id, timestamp, score, timestamp, score..."
                std::string post_string(cfg_.channel_id);
                for (auto data: sending_queue_)
                {
                    post_string.append(",");
                    post_string.append(std::to_string(data.first));
                    post_string.append(",");
                    post_string.append(std::to_string(data.second));
                }

                if(DoHttpPOST(post_string))
                {
                    log_info("[Sender] timestamp: %lld, Sent %d data.", time, sending_queue_.size());
                    ClearSendingQueue();
                }
                else
                {
                    log_info("[Sender] Sent fail, sending_queue size: %d", sending_queue_.size());
                }   
            }     
        } 
    }
}

bool DataSender::DoHttpPOST(std::string &data)
{
    curl_easy_setopt(curl_, CURLOPT_URL, cfg_.hostname.c_str());
    curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, data.c_str());
    auto res = curl_easy_perform(curl_);
    if(res != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }
    return res == CURLE_OK;
}