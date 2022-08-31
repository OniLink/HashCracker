#include <cstring>

#include "RNGStringGenerator.hpp"

RNGStringGenerator::RNGStringGenerator(
  const std::string& alphabet,
  const uint32_t length
) :
  StringGenerator( length ),
  indices( new uint32_t[ length ] ),
  lookup( new char[ alphabet.length() ] ),
  lookup_size( alphabet.length() ) {
  if( lookup_size == 0 ) {
    end = true;
  }

  std::memset( indices, 0, length * sizeof( uint32_t ) );
  std::memcpy( lookup, alphabet.data(), lookup_size );

  fillBuffer();
}

RNGStringGenerator::~RNGStringGenerator() {
  delete[] indices;
  delete[] lookup;
}

void RNGStringGenerator::incrementState() {
  if( end ) {
    return;
  }

  bool inc = false;
  uint32_t index = max_length - 1;

  do {
    ++indices[ index ];
    
    inc = ( indices[ index ] >= lookup_size );
    if( inc ) {
      indices[ index ] %= lookup_size;
      
      end = ( index == 0 );

      --index;
    }
  } while( inc && !end );
}

void RNGStringGenerator::fillBuffer() {
  if( !end ) {
    for( uint32_t i = 0; i < max_length; ++i ) {
      //buffer[ i ] = ( end ? 0x00 : lookup[ indices[ i ] ] );
      buffer[ i ] = lookup[ indices[ i ] ];
    }
  }
}

void RNGStringGenerator::resetState() {
  std::memset( indices, 0x00, max_length );
}

