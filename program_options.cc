#include "program_options.hh"

namespace
{

bool isOption( const std::string& string )
{
  return string.substr( 0, 2 ) == std::string( "--" );
}

} // end of anonymous namespace

namespace Markov
{

ProgramOptions::ProgramOptions( int argc, char* argv[] )
{
  std::vector<std::string> arguments;

  for( int i = 1; i < argc; i++ )
    arguments.push_back( argv[i] );

  for( std::size_t i = 0; i < arguments.size(); i++ )
  {
    const std::string& argument = arguments.at(i);
    if( isOption( argument ) )
    {
      Option option;
      option.name = argument;

      if( i+1 < arguments.size() && !isOption( arguments.at(i+1) ) )
      {
        option.value = arguments.at(i+1);
        ++i;
      }

      _namedOptions.push_back( option );
    }
    else
      _positionalOptions.push_back(argument);
  }
}

bool ProgramOptions::has( const std::string& name, bool requireValue ) const
{
  auto pos = std::find_if( _namedOptions.begin(), _namedOptions.end(),
                           [&] ( const Option& option )
                           {
                             return option.name == name;
                           } );

  return  pos != _namedOptions.end()
         && ( !requireValue || !pos->value.empty() );
}

} // end of namespace "Markov"
