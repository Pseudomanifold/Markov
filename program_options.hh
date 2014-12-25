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

  std::vector<std::string> getPositionalOptions() const;

private:
  std::vector<Option> _namedOptions;
  std::vector<std::string> _positionalOptions;
};

} // end of namespace "Markov"

#endif
