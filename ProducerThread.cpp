#include <cstring>

#include "ProducerThread.hpp"

#include "FullStringGenerator.hpp"
#include "ThreadVariables.hpp"

void cpuProducerThread( char* string_buffer, std::string string_format,
                        uint32_t string_length, uint32_t buffer_size,
                        uint32_t buffer_count,
                        const std::vector< std::string >& dictionary,
                        const std::vector< std::string >& prefix_dictionary,
                        const std::vector< std::string >& suffix_dictionary ) {
  FullStringGenerator generator(
    string_format,
    dictionary,
    prefix_dictionary,
    suffix_dictionary,
    string_length
  );

  while( !producer_done.test() ) {
    uint32_t base = 
      current_buffer_producer * buffer_size * ( string_length+1 );

    // Fill with strings
    for( uint32_t i = 0; i < buffer_size && !generator.isEnd(); ++i ) {
      // Generate string
      const char* next_string = generator.get();

      std::memcpy(
        string_buffer + base + i * ( string_length+1 ),
        next_string,
        string_length
      );

      generator.advance();
    }

    // Prepare the next loop
    uint32_t cur_buf = (current_buffer_producer + 1) % buffer_count;
    current_buffer_producer.store( cur_buf );
    current_buffer_producer.notify_all();

    // If Generator Done, Set Done Flag
    if( generator.isEnd() ) {
      producer_done.test_and_set();
    }

    // Wait until the consumer is not on the same buffer as us
    if( !producer_done.test() ) {
      current_buffer_consumer.wait( current_buffer_producer );
    }
  }
}

