#include <cstring>

#include <iostream>
#include <mutex>

#include "ConsumerThread.hpp"

#include "MurmurHash3.h"
#include "ThreadVariables.hpp"

void cpuConsumerThread( uint32_t id, uint32_t batches, char* string_buffer,
                        uint32_t string_length, uint32_t queue_size,
                        uint32_t buffer_count, uint32_t* hashes,
                        uint32_t hash_count,
                        std::barrier< std::function< void() > >& sync_point ) {
  while( true ) {
    // If the producer is done and we've caught up, we're done
    if( producer_done.test() &&
        current_buffer_producer == current_buffer_consumer ) {
      return;
    }

    // Wait for Producer Buffer =/= Current Buffer
    current_buffer_producer.wait( current_buffer_consumer );

    // Find our starting point in the larger buffer
    uint32_t base = ( string_length+1 ) * queue_size *
                    ( batches * current_buffer_consumer + id );

    // Work on the batch
    for( uint32_t i = 0; i < queue_size; ++i ) {
      // Find String Bounds
      char* string_start = string_buffer + base + i * ( string_length+1 );
      uint32_t len = std::strlen( string_start );

      // Calculate Hash
      uint32_t hash = 0;
      MurmurHash3_x86_32( string_start, len, 0, &hash );

      // Compare against Target Hashes
      for( uint32_t j = 0; j < hash_count; ++j ) {
        // If Match, Lock and Report
        if( hashes[ j ] == hash ) {
          std::lock_guard<std::mutex> lock( output_mutex );
          std::cout << "MATCH FOUND: "
                    << hash
                    << " = "
                    << std::string( string_start, len )
                    << std::endl;
        }
      }
    }

    // Wait for all consumer threads to sync
    sync_point.arrive_and_wait();
  }
}

