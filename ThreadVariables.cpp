#include "ThreadVariables.hpp"

std::atomic<uint32_t> current_buffer_producer( 0 );
std::atomic<uint32_t> current_buffer_consumer( 0 );

std::atomic_flag producer_done = ATOMIC_FLAG_INIT;

std::mutex output_mutex;

