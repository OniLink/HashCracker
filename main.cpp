#include <cstdint>
#include <cstring>

#include <barrier>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <vector>

#include "Config.hpp"

#include "ProducerThread.hpp"
#include "ConsumerThread.hpp"

#include "ThreadVariables.hpp"

int main( int argc, char* argv[] ) {
  // Default values
  std::vector< std::string > prefix_dictionary;
  std::vector< std::string > suffix_dictionary;
  std::vector< std::string > dictionary;
  std::vector< uint32_t > hash_targets;
  uint32_t fallback_thread_count = std::thread::hardware_concurrency();
  if( fallback_thread_count > 1 ) {
    fallback_thread_count -= 1;
  } else {
    fallback_thread_count = 1;
  }
  uint32_t queue_size = 64;
  uint32_t buffer_count = 2;
  uint32_t max_string_length = 32;
  bool opencl_enabled = false;
  std::string string_format = "dr[Ccds6]";

  /////////////////////////////////////////////////////////
  // Load Configuration
  std::map< std::string, std::string > config = loadConfig( "Config.cfg" );

  if( config.contains( "target_hashes" ) ) {
    std::vector< std::string > hash_strings = loadFile(
      config[ "target_hashes" ]
    );

    for( uint32_t i = 0; i < hash_strings.size(); ++i ) {
      hash_targets.emplace_back( std::stoul( hash_strings[ i ], nullptr, 16 ) );
    }
  } else {
    std::cerr << "ERROR: Missing required field 'target_hashes'." << std::endl;
    return 1;
  }

  if( config.contains( "format" ) ) {
    string_format = config[ "format" ];
  } else {
    std::cerr << "ERROR: Missing required field 'format'." << std::endl;
    std::cerr << "Format of 'format' is a series of characters indicating "
                 "the types of strings to be checked against the hashes."
              << std::endl
              << "Example format: dr[Ccds6]r[d6]dd"
              << std::endl
              << "Explanation:"
              << std::endl
              << "Each d represents a word in the dictionary if one is provided."
              << std::endl
              << "The r[...] field represents random strings of characters."
              << std::endl
              << "Within the brackets is a specifier for how the random strings "
                 "will be generated."
              << std::endl
              << "C indicates uppercase letters can be used."
              << std::endl
              << "c indicates lowercase letters can be used."
              << std::endl
              << "d indicates digits 0-9 can be used."
              << std::endl
              << "s indicates the separate '_' can be used."
              << std::endl
              << "The specifier must end with a decimal number which "
                 "indicates the length of the random string."
              << std::endl
              << "If a suffix or suffix dictionary is specified "
              << "and/or if a prefix or prefix dictionary is specified "
              << "in the configuration, then these suffixes and/or prefixes "
              << "will always be included in tested strings."
              << std::endl;
    return 1;
  }
 
  if( config.contains( "prefix" ) ) {
    prefix_dictionary = { config[ "prefix" ] };
  } else if( config.contains( "prefix_dictionary" ) ) {
    prefix_dictionary = loadFile( config[ "prefix_dictionary" ] );
  }

  if( config.contains( "suffix" ) ) {
    suffix_dictionary = { config[ "suffix" ] };
  } else if( config.contains( "suffix_dictionary" ) ) {
    suffix_dictionary = loadFile( config[ "suffix_dictionary" ] );
  }

  if( config.contains( "dictionary" ) ) {
    dictionary = loadFile( config[ "dictionary" ] );
  }

  if( config.contains( "opencl_enabled" ) ) {
    opencl_enabled = ( std::stoi( config[ "opencl_enabled" ] ) == 1 );
  }

  if( config.contains( "cpu_threads" ) ) {
    fallback_thread_count = std::stoul( config[ "cpu_threads" ] );
  }

  if( config.contains( "queue_size" ) ) {
    queue_size = std::stoul( config[ "queue_size" ] );
  }

  if( config.contains( "max_string_length" ) ) {
    max_string_length = std::stoul( config[ "max_string_length" ] );
  }

  /////////////////////////////////////////////////////////
  // Main Operation
  bool using_opencl = false;

  if( opencl_enabled ) {
    // @TODO Attempt to init OpenCL
  }

  // Make sure we're outputting hashes right before we begin
  std::cout << std::hex << std::uppercase;
  
  if( using_opencl ) {
    ///////////////////////////////////////////////////////
    // OpenCL

    // Initialization
    // @TODO Setup OpenCL buffers (string, match)

    // Main Operation
    // @TODO Run OpenCL Producer Thread
    // @TODO Run OpenCL Consumer Thread

    // Cleanup
    // @TODO Cleanup Threads
    // @TODO Cleanup Buffers
    // @TODO Cleanup OpenCL
  } else {
    ///////////////////////////////////////////////////////
    // CPU
    
    // Initialization
    uint32_t buffer_size = fallback_thread_count * queue_size;
    uint32_t allocation_size =
      buffer_size * ( max_string_length+1 ) * buffer_count;
    char *string_buffer = new char[ allocation_size ];
    std::memset( string_buffer, 0x00, allocation_size );
    
    // Main Operation
    std::thread producer_thread(
      cpuProducerThread,
      &string_buffer[ 0 ],
      string_format,
      max_string_length,
      buffer_size,
      buffer_count,
      std::ref( dictionary ),
      std::ref( prefix_dictionary ),
      std::ref( suffix_dictionary )
    );

    std::barrier< std::function< void() > > consumer_sync(
      fallback_thread_count,
      [=]() noexcept {
        // Set up for the next round
        uint32_t cur_buf = (current_buffer_consumer + 1) % buffer_count;
        current_buffer_consumer.store( cur_buf );
        current_buffer_consumer.notify_all();
      }
    );
    std::vector< std::thread > consumer_threads;
    for( uint32_t id = 0; id < fallback_thread_count; ++id ) {
      consumer_threads.emplace_back(
        cpuConsumerThread,
        id,
        fallback_thread_count,
        &string_buffer[ 0 ],
        max_string_length,
        queue_size,
        buffer_count,
        &hash_targets[ 0 ],
        hash_targets.size(),
        std::ref( consumer_sync )
      );
    }

    // Cleanup
    producer_thread.join();

    for( uint32_t id = 0; id < fallback_thread_count; ++id ) {
      consumer_threads[ id ].join();
    }

    delete[] string_buffer;
  }

  return EXIT_SUCCESS;
}

