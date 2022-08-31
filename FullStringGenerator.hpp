#ifndef FULL_STRING_GENERATOR_HPP
#define FULL_STRING_GENERATOR_HPP

#include <cstdint>

#include <string>
#include <vector>

#include "StringGenerator.hpp"

class FullStringGenerator : public StringGenerator {
  private:
    std::vector< StringGenerator* > subgenerators;
    StringGenerator** subgen;
    uint32_t subgen_count;

  protected:
    virtual void incrementState();
    virtual void fillBuffer();
    virtual void resetState();

  public:
    FullStringGenerator( const std::string& format,
                         const std::vector< std::string >& dictionary,
                         const std::vector< std::string >& prefix_dictionary,
                         const std::vector< std::string >& suffix_dictionary,
                         const uint32_t max_length );
    ~FullStringGenerator();
};

#endif

