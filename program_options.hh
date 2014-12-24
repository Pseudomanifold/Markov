#ifndef MARKOV_PROGRAM_OPTIONS_HH__
#define MARKOV_PROGRAM_OPTIONS_HH__

#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace Markov
{

struct Option
{
  std::string name;
  std::string value;
};

class ProgramOptions
{
public:
  ProgramOptions( int argc, char* argv[] );

  template <class T> T get( const std::string& name ) const
  {
    auto pos = std::find_if( _namedOptions.begin(), _namedOptions.end(),
                             [&] ( const Option& option )
                             {
                               return option.name == name;
                             } );

    if( pos == _namedOptions.end() )
      throw std::runtime_error( "Unable to find named option" );

    if( pos->value.empty() )
      throw std::runtime_error( "Unable to convert empty option" );

    T v = T();
    std::istringstream converter( pos->value );
    converter >> v;

    if( converter.fail() )
      throw std::runtime_error( "Conversion failed" );

    return v;
  }

  bool has( const std::string& name, bool requireValue = false ) const;

private:
  std::vector<Option> _namedOptions;
  std::vector<std::string> _positionalOptions;
};

} // end of namespace "Markov"

#endif
