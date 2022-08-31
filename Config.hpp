#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <map>
#include <string>
#include <vector>

const std::vector< std::string > loadFile( const std::string& filename );

const std::map< std::string, std::string > loadConfig(
  const std::string& filename
);

#endif

