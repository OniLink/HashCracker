#ifndef CONSUMER_THREAD_HPP
#define CONSUMER_THREAD_HPP

#include <cstdint>

#include <barrier>
#include <functional>

void cpuConsumerThread( uint32_t id, uint32_t batches, char* string_buffer,
                        uint32_t string_length, uint32_t queue_size,
                        uint32_t buffer_count, uint32_t* hashes,
                        uint32_t hash_count,
                        std::barrier< std::function< void() > >& sync_point );

#endif

