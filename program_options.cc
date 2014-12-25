/*
 * Copyright (c) 2014 Bastian Rieck
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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

std::vector<std::string> ProgramOptions::getPositionalOptions() const
{
  return _positionalOptions;
}

} // end of namespace "Markov"
