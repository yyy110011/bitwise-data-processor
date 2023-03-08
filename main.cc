#include "data_worker.h"
#include "data_sender.h"
#include "utils.h"

#include <thread>
#include <iostream>

int main()
{
    log_set_level(LOG_INFO);
    try
    {
        // Shared queue to share data between DataWorker and DataSender
        auto shared_score_queue = std::make_shared<shared_queue<std::pair<long long, float>>>();
        curl_global_init(CURL_GLOBAL_DEFAULT);

        // Get env to set configs
        struct DataWorkerConfig data_worker_cfg = {
            GetEnv(std::string("SOURCE_HOSTNAME")),
            std::stoi(GetEnv(std::string("SOURCE_SIZE"))),
            std::stoi(GetEnv(std::string("IDENTICAL_SIZE")))
        };
        struct DataSenderConfig data_sender_cfg = {
            GetEnv(std::string("CHANNEL_ID")),
            GetEnv(std::string("OUTPUT_HOSTNAME")),
            std::stoi(GetEnv(std::string("OUTPUT_AMOUNT")))
        };
        log_info("Config all set");

        DataWorker data_worker(shared_score_queue, data_worker_cfg);
        DataSender data_sender(shared_score_queue, data_sender_cfg);
        std::thread data_worker_t(&DataWorker::Download, &data_worker);
        std::thread data_sender_t(&DataSender::SendData, &data_sender);

        data_worker_t.join();
        data_sender_t.join();

        curl_global_cleanup();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}
