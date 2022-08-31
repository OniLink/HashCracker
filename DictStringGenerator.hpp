#ifndef DICT_STRING_GENERATOR_HPP
#define DICT_STRING_GENERATOR_HPP

#include <cstdint>

#include <string>
#include <vector>

#include "StringGenerator.hpp"

class DictStringGenerator : public StringGenerator {
  private:
    uint32_t lookup_size;
    uint32_t index;
    char* lookup;

  protected:
    virtual void incrementState();
    virtual void fillBuffer();
    virtual void resetState();

  public:
    DictStringGenerator( const std::vector< std::string >& dictionary,
                         const uint32_t max_length );
    ~DictStringGenerator();
};

#endif

