#include <cstdint>

#include <fstream>
#include <iostream>

#include "Config.hpp"

const std::vector< std::string > loadFile ( const std::string& filename ) {
  std::vector< std::string > file_lines;

  // Open the file
  std::ifstream file( filename );
  
  if( !file.is_open() ) {
    std::cerr << "ERROR: Failed to open file '"
              << filename
              << "'."
              << std::endl;
    return file_lines;
  }

  // Parse lines
  for( std::string line; std::getline( file, line ); ) {
    file_lines.push_back( line );
  }

  file.close();
  return file_lines;
}

const std::map< std::string, std::string > loadConfig(
  const std::string& filename
) {
  std::map< std::string, std::string > config_map;
  std::vector< std::string > lines = loadFile( filename );
 
  // Parse each line
  // Format: <name>=<key>
  for( uint32_t i = 0; i < lines.size(); ++i ) {
    std::string line = lines[ i ];

    // Skip blank lines and comments
    if( line.length() == 0 || line[ 0 ] == '#' ) {
      continue;
    }

    // Validate
    char eq_count = 0;
    for( uint32_t i = 0; i < line.length(); ++i ) {
      if( line[ i ] == '=' ) {
        ++eq_count;
      }
    }

    if( eq_count != 1 ) {
      std::cerr << "ERROR: Unable to parse line '"
                << line
                << "'."
                << std::endl;
      std::cerr << "Format: <key>=<value>" << std::endl;
      continue;
    }

    // Parse key and value
    uint32_t eq_loc = line.find( '=' );
    std::string key = line.substr( 0, eq_loc );
    std::string val = line.substr( eq_loc+1 );
    config_map[ key ] = val;
  }

  return config_map;
}

