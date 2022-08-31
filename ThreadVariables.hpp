#ifndef THREAD_VARIABLES_HPP
#define THREAD_VARIABLES_HPP

#include <cstdint>

#include <atomic>
#include <mutex>

extern std::atomic<uint32_t> current_buffer_producer;
extern std::atomic<uint32_t> current_buffer_consumer;

extern std::atomic_flag producer_done;

extern std::mutex output_mutex;

#endif

