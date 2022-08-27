#include <iostream>
#include <thread>
#include <vector>

#include "Config.hpp"
#include "CPUProducerConsumer.hpp"
#include "StringGenerator.hpp"

int main() {
  std::cout << std::hex << std::uppercase;
  for( unsigned int len = MIN_LEN; len <= MAX_LEN; ++len ) {
    std::cout << "Trying strings of length " << len << std::endl;
    
    std::vector< std::thread > consumer_threads;

    std::thread producer_thread( threadProducer, len );
    for( unsigned int i = 0; i < THREADS; ++i ) {
      consumer_threads.emplace_back( threadConsumer );
    }

    producer_thread.join();
    for( unsigned int i = 0; i < THREADS; ++i ) {
      consumer_threads[ i ].join();
    }
  }

  return 0;
}

