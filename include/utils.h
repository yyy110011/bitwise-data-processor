#ifndef __UTILS_H__
#define __UTILS_H__

extern "C" {
    #include "../logging/src/log.h"
}

#include <vector>
#include <chrono>
#include <mutex>

#define BYTE_DIGIT 3

template <typename T>
struct shared_queue
{
    std::vector<T> queue;
    std::mutex mutex;
};

static std::string GetEnv(const std::string & var)
{
    const char * val = std::getenv(var.c_str());
    if(val == nullptr)
    {
        return "";
    }
    else
    {
        log_info("Get %s=%s", var.c_str(), val);
        return val;
    }
}

static long long getUNIXTimestamp()
{
    const auto p1 = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(p1.time_since_epoch()).count();
}
#endif // __UTILS_H__