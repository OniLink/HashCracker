#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <cstdint>

#include <string>

const std::string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                             "abcdefghijklmnopqrstuvwxyz";
const std::string PREFIX = "";
const std::string SUFFIX = "ExpRevHigh";

const uint32_t LIST_TERM = 0;
const uint32_t TARGET_HASHES[] = {
  0xB677DA57,
  0x15264911,
  LIST_TERM
};

const uint32_t THREADS = 24;
const uint32_t MIN_LEN = 7;
const uint32_t MAX_LEN = 7;
const uint32_t QUEUE_MAX = 1024;
const uint32_t BATCH_SIZE = 64;

#endif

