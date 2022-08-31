#ifndef STRING_GENERATOR_HPP
#define STRING_GENERATOR_HPP

#include <cstdint>

class StringGenerator {
  protected:
    bool end;
    char* buffer;
    uint32_t max_length;

    virtual void incrementState() = 0;
    virtual void fillBuffer() = 0;
    virtual void resetState() = 0;

    void setEnd();

  public:
    StringGenerator( const uint32_t max_str_length );
    virtual ~StringGenerator();

    const uint32_t getMaxLength() const;

    const char* get() const;
    void advance();

    const bool isEnd() const;

    void reset();
};

#endif

