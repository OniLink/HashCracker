#include <cstring>

#include "DictStringGenerator.hpp"

DictStringGenerator::DictStringGenerator(
  const std::vector< std::string >& dictionary,
  const uint32_t max_length
) :
  StringGenerator( max_length ),
  index( 0 ),
  lookup( new char[ dictionary.size() * max_length ] ),
  lookup_size( dictionary.size() ) {
  if( lookup_size == 0 ) {
    setEnd();
  }
  
  std::memset( lookup, 0x00, lookup_size * max_length );

  for( uint32_t i = 0; i < dictionary.size(); ++i ) {
    std::memcpy(
      lookup + i*max_length,
      dictionary[ i ].data(),
      dictionary[ i ].length()
    );
  }

  fillBuffer();
}

DictStringGenerator::~DictStringGenerator() {
  delete[] lookup;
}

void DictStringGenerator::incrementState() {
  if( end ) {
    return;
  }

  ++index;
  end = ( index >= lookup_size );
}

void DictStringGenerator::fillBuffer() {
  if( !end ) {
    std::memcpy( buffer, lookup + index*max_length, max_length );
  }
}

void DictStringGenerator::resetState() {
  index = 0;
}

