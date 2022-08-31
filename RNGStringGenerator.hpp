#ifndef RNG_STRING_GENERATOR_HPP
#define RNG_STRING_GENERATOR_HPP

#include <cstdint>

#include <string>
#include <vector>

#include "StringGenerator.hpp"

class RNGStringGenerator : public StringGenerator {
  private:
    uint32_t lookup_size;
    uint32_t* indices;
    char* lookup;

  protected:
    virtual void incrementState();
    virtual void fillBuffer();
    virtual void resetState();

  public:
    RNGStringGenerator( const std::string& alphabet, const uint32_t length );
    ~RNGStringGenerator();
};

#endif

