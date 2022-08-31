#include <cstring>

#include <iostream>

#include "FullStringGenerator.hpp"

#include "DictStringGenerator.hpp"
#include "RNGStringGenerator.hpp"

bool parseRNGFormat( const std::string& format, std::string& alphabet,
                     uint32_t& length ) {
  const std::string upper     = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  const std::string lower     = "abcdefghijklmnopqrstuvwxyz";
  const std::string digits    = "0123456789";
  const std::string separator = "_";
      
  for( uint32_t i = 0; i < format.length(); ++i ) {
    if( format[ i ] == 'C' ) {
      // Uppercase
      alphabet += upper;
    } else if( format[ i ] == 'c' ) {
      // Lowercase
      alphabet += lower;
    } else if( format[ i ] == 'd' ) {
      // Digits
      alphabet += digits;
    } else if( format[ i ] == 's' ) {
      // Separator
      alphabet += separator;
    } else if( format[ i ] >= '0' && format[ i ] <= '9' ) {
      // Length
      std::string format_length = format.substr( i );
          
      // Validate
      for( uint32_t j = 0; j < format_length.length(); ++j ) {
        if( format_length[ j ] < '0' || format_length[ j ] > '9' ) {
          std::cerr << "ERROR: Invalid digit in RNG format length '"
                    << format_length
                    << "'."
                    << std::endl;
          return false;
        }
      }

      // Parse
      length = std::stoul( format_length );
      return true;
    } else {
      // Invalid
      std::cerr << "ERROR: Invalid specifier '"
                << format[ i ]
                << "' in RNG format '"
                << format
                << "'."
                << std::endl;
      return false;
    }
  }

  std::cerr << "ERROR: No RNG length specified in RNG format '"
            << format
            << "'."
            << std::endl;
  return false;
}

FullStringGenerator::FullStringGenerator(
  const std::string& format,
  const std::vector< std::string >& dictionary,
  const std::vector< std::string >& prefix_dictionary,
  const std::vector< std::string >& suffix_dictionary,
  const uint32_t max_length
) :
  StringGenerator( max_length ),
  subgen_count( 0 ) {
  // Prefixes
  if( prefix_dictionary.size() > 0 ) {
    subgenerators.emplace_back(
      new DictStringGenerator( prefix_dictionary, max_length )
    );
  }

  // Parse format
  for( uint32_t i = 0; i < format.length(); ++i ) {
    if( format[ i ] == 'd' ) {
      // Dictionary
      subgenerators.emplace_back(
        new DictStringGenerator( dictionary, max_length )
      );
    } else if( format[ i ] == 'r' ) {
      // Random
      // Get subformat
      uint32_t sub_start = format.find_first_of( '[', i );
      uint32_t sub_end = format.find_first_of( ']', i );

      // Validate range
      if( sub_start == std::string::npos ||
          sub_end == std::string::npos ||
          sub_start != i+1 ||
          sub_end < sub_start ) {
        std::cerr << "ERROR: Unable to process RNG format in '"
                  << format.substr( i )
                  << "'."
                  << std::endl;
        continue;
      }

      // Parse
      uint32_t sub_length = sub_end - sub_start - 1;
      std::string subformat = format.substr( sub_start+1, sub_length );
      std::string alphabet = "";
      uint32_t length = 0;
      
      // Create the generator
      if( parseRNGFormat( subformat, alphabet, length ) ) {
        subgenerators.emplace_back(
          new RNGStringGenerator( alphabet, length )
        );
      }
      
      i = sub_end;
    } else {
      // Error
      std::cerr << "ERROR: Invalid character '"
                << format[ i ]
                << "' in format string."
                << std::endl;
    }
  }

  // Suffixes
  if( suffix_dictionary.size() > 0 ) {
    subgenerators.emplace_back(
      new DictStringGenerator( suffix_dictionary, max_length )
    );
  }
  
  subgen = subgenerators.data();
  subgen_count = subgenerators.size(); 

  fillBuffer();
}

FullStringGenerator::~FullStringGenerator() {
  for( uint32_t i = 0; i < subgen_count; ++i ) {
    delete subgenerators[ i ];
  }
}

void FullStringGenerator::incrementState() {
  if( end ) {
    return;
  }

  bool inc = false;
  uint32_t index = subgen_count - 1;

  do {
    subgen[ index ]->advance();
    
    inc = subgen[ index ]->isEnd();
    if( inc ) {
      subgen[ index ]->reset();
      
      end = ( index == 0 );

      --index;
    }
  } while( inc && !end );
}

void FullStringGenerator::fillBuffer() {
  if( end ) {
    //std::memset( buffer, 0x00, max_length );
    return;
  }

  uint32_t write_pos = 0;
  for( uint32_t i = 0; i < subgen_count; ++i ) {
    const char* next = subgen[ i ]->get();
    uint32_t write_length = std::strlen( next );
    std::memcpy( buffer + write_pos, next, write_length );
    write_pos += write_length;
  }

  std::memset( buffer + write_pos, 0x00, max_length - write_pos );
}

void FullStringGenerator::resetState() {
  for( uint32_t i = 0; i < subgenerators.size(); ++i ) {
    subgen[ i ]->reset();
  }
}

