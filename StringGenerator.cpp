#include "StringGenerator.hpp"

StringGenerator::StringGenerator( unsigned int len, std::string alphabet ) :
  str_len( len ),
  indices( len, 0 ),
  lookup( alphabet ),
  lookup_size( alphabet.length() ),
  end( false ) {
  if( len == 0 ) {
    end = true;
  }
}

std::string StringGenerator::convertToString() {
  std::string out = "";
  for( unsigned int i = 0; i < str_len; ++i ) {
    out += lookup[indices[i]];
  }
  return out;
}

std::string StringGenerator::getNext() {
  if( isEnd() ) {
    return std::string( "" );
  }

  std::string out = convertToString();
  increment();
  return out;
}

void StringGenerator::increment() {
  if( end ) {
    return;
  }

  bool inc = false;
  unsigned int index = str_len-1;
  do {
    ++indices[ index ];
      
    inc = ( indices[ index ] >= lookup_size );
    if( inc ) {
      indices[ index ] = indices[ index ] % lookup_size;
       
      end = ( index == 0 );
      if( end ) {
        return;
      }

      --index;
    }
  } while( inc && index >= 0 );
}

bool StringGenerator::isEnd() {
  return end;
}

