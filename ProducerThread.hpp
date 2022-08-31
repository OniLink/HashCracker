#ifndef PRODUCER_THREAD_HPP
#define PRODUCER_THREAD_HPP

#include <cstdint>

#include <string>
#include <vector>

void cpuProducerThread( char* string_buffer, std::string string_format,
                        uint32_t string_length, uint32_t buffer_size,
                        uint32_t buffer_count,
                        const std::vector< std::string >& dictionary,
                        const std::vector< std::string >& prefix_dictionary,
                        const std::vector< std::string >& suffix_dictionary );

#endif

