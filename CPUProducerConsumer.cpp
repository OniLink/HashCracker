#include <cstdint>

#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <vector>

#include "Config.hpp"
#include "CPUProducerConsumer.hpp"
#include "StringGenerator.hpp"

#include "MurmurHash3.h"

std::queue< std::string > candidate_queue;
std::atomic_bool producer_finished;
std::condition_variable full_notifier;
std::condition_variable empty_notifier;
std::mutex output_mutex;
std::mutex queue_mutex;

bool hashInList( uint32_t test, const uint32_t* list ) {
  for( unsigned int i = 0; list[ i ] != LIST_TERM; ++i ) {
    if( test == list[ i ] ) {
      return true;
    }
  }

  return false;
}

void threadProducer( unsigned int len ) {
  producer_finished = false;
  auto generator = StringGenerator( len, ALPHABET );
  while( !generator.isEnd() ) {
    // Generate strings
    std::vector< std::string > next_strings;
    for( int i = 0; i < BATCH_SIZE; ++i ) {
      if( !generator.isEnd() ) {
        next_strings.emplace_back( generator.getNext() );
      }
    }

    int queue_size = 0;

    // Add them to the queue
    {
      std::unique_lock<std::mutex> queue_lock( queue_mutex );
      while( candidate_queue.size() >= QUEUE_MAX ) {
        empty_notifier.wait( queue_lock );
      }
      for( int i = 0; i < next_strings.size(); ++i ) {
        candidate_queue.push( next_strings[ i ] );
      }
      queue_size = candidate_queue.size();
    }
    full_notifier.notify_all();

    //{
    //  std::unique_lock<std::mutex> output_lock( output_mutex );
    //  std::cout << "Queue size: " << queue_size << std::endl;
    //}
  }

  producer_finished = true;
  full_notifier.notify_all();
}

void threadConsumer() {
  int queue_size = 0;

  while( !producer_finished || queue_size > 0 ) {
    // Load in the next candidate
    std::string candidate = "";
    {
      std::unique_lock<std::mutex> queue_lock( queue_mutex );
      while( candidate_queue.empty() && !producer_finished ) {
        full_notifier.wait( queue_lock );
      }

      if( producer_finished && candidate_queue.empty() ) {
        return;
      }

      candidate = candidate_queue.front();
      candidate_queue.pop();
      queue_size = candidate_queue.size();
    }
    
    if( queue_size < QUEUE_MAX ) {
      empty_notifier.notify_one();
    }

    candidate = PREFIX + candidate + SUFFIX;

    // Calculate and check the hash
    std::uint32_t candidate_hash;
    MurmurHash3_x86_32( candidate.c_str(), candidate.length(),
                        0, &candidate_hash );
    if( hashInList( candidate_hash, TARGET_HASHES ) ) {
      std::unique_lock<std::mutex> output_lock( output_mutex );
      std::cout << "MATCH FOUND: " << candidate_hash
                << " = " << candidate << std::endl;
    }
  }
}

