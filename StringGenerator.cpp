#include <cstring>

#include "StringGenerator.hpp"

StringGenerator::StringGenerator( const uint32_t max_str_length ) :
  end( false ),
  max_length( max_str_length ),
  buffer( nullptr ) {
  if( max_length == 0 ) {
    setEnd();
  } else {
    buffer = new char[ max_length+1 ];
    std::memset( buffer, 0x00, max_length+1 );
  }
}

StringGenerator::~StringGenerator() {
  if( buffer ) {
    delete[] buffer;
  }
}

const uint32_t StringGenerator::getMaxLength() const {
  return max_length;
}

const char* StringGenerator::get() const {
  return buffer;
}

void StringGenerator::advance() {
  incrementState();
  fillBuffer();
}

const bool StringGenerator::isEnd() const {
  return end;
}

void StringGenerator::reset() {
  resetState();
  end = false;
  fillBuffer();
}

void StringGenerator::setEnd() {
  end = true;
}

