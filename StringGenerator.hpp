#ifndef STRING_GENERATOR_HPP
#define STRING_GENERATOR_HPP

#include <string>
#include <vector>

class StringGenerator {
  private:
    unsigned int str_len;
    unsigned int lookup_size;
    std::vector< unsigned int > indices;
    std::string lookup;
    bool end;

  std::string convertToString();

  void increment();

  public:
    StringGenerator( unsigned int len, std::string alphabet );

    std::string getNext();

    bool isEnd();
};

#endif

