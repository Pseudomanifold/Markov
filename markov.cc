/*
 * An implementation of Markov chains for text generation using C++11 and STL
 * methods.
 *
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

#include <algorithm>
#include <fstream>
#include <iterator>
#include <iostream>
#include <list>
#include <map>
#include <random>
#include <sstream>
#include <string>
#include <vector>

const std::string punctuation = ",;:.!?";
typedef std::map< std::string, std::vector<std::string> > database_type;

/** Checks whether a string is a punctuation string */
bool isPunctuation( const std::string& string )
{
  return    string.length() == 1
         && string.find_first_of( punctuation ) != std::string::npos;
}

/**
  Joins a sequence of tokens and returns a single string. If one of the tokens
  is a punctuation mark, spurious punctuation will be avoided.
*/

template <class InputIterator> std::string join( InputIterator begin, InputIterator end )
{
  std::string result;

  for( auto it = begin; it != end; ++it )
  {
    // Is this punctuation? If so, do not add any whitespace.
    if( isPunctuation( *it ) )
      result += *it;
    else
    {
      if( it != begin )
        result += " ";

      result += *it;
    }
  }

  return result;
}

/** Splits a string into its sequence of tokens. */
template <class OutputIterator> void split( const std::string& string, OutputIterator result )
{
  std::istringstream buffer( string );

  std::vector<std::string> rawTokens;

  std::copy(  std::istream_iterator<std::string>( buffer ),
              std::istream_iterator<std::string>(),
              std::back_inserter( rawTokens ) );

  for( auto&& rawToken : rawTokens )
  {
    // Only use the _last_ punctuation that may be found in the token. We do
    // not want to split a chapter number or a word.
    auto pos = rawToken.find_last_of( punctuation );
    if( rawToken.length() > 1 && pos != std::string::npos && pos + 1 == rawToken.length() )
    {
      *result++ = rawToken.substr( 0, pos );
      *result++ = rawToken.substr( pos );
    }

    // Simply copy the token
    else
      *result++ = rawToken;
  }
}


/**
  Tokenizes a given file. Any punctuation mark and any whitespace character is
  considered a token. The function returns a container of all tokens in the
  order in which they appear in the text. By concatenating subsequent  entries
  with a whitespace character, the original text may be formed again.
*/

std::vector<std::string> getTokens( const std::string& filename )
{
  std::ifstream in( filename );

  std::vector<std::string> rawTokens;

  std::copy(  std::istream_iterator<std::string>( in ),
              std::istream_iterator<std::string>(),
              std::back_inserter( rawTokens ) );

  std::vector<std::string> tokens;
  tokens.reserve( rawTokens.size() );

  for( auto&& rawToken : rawTokens )
    split( rawToken, std::back_inserter( tokens ) );

  return tokens;
}

/**
  Given a vector of tokens and a prefix length, traverses the vector and stores
  the appropriate suffix of the token. The model for updating the suffixes is
  very simple --- subsequent words are merely added to a list of known words.
  This makes choosing the next word easier.
*/

database_type buildDatabase( const std::vector<std::string>& tokens, unsigned int prefixLength )
{
  database_type database;

  std::list<std::string> prefixWords( prefixLength );

  for( std::size_t i = 0; i < tokens.size(); i++ )
  {
    prefixWords.pop_front();
    prefixWords.push_back( tokens.at(i) );

    if( i + 1 < tokens.size() )
    {
      std::string prefix = join( prefixWords.begin(),
                                 prefixWords.end() );

      database[prefix].push_back( tokens.at(i+1) );
    }
  }

  return database;
}

/**
  Starts creating text from the database of tokens, using a Markov chain. The
  function will choose a prefix at random from the database, then select a
  subsequent word from the word list at random, thereby creating a new prefix.
  This process will be repeated until a number of pre-defined iterations has
  been reached.
*/

std::string spew( const database_type& database, unsigned int numIterations )
{
  std::random_device rd;
  std::default_random_engine engine( rd() );

  std::ostringstream output;
  std::string prefix;

  {
    std::uniform_int_distribution<std::size_t> distribution( 0, database.size() - 1 );
    std::size_t index = distribution( engine );
    auto it           = database.begin();

    std::advance( it, index );

    prefix = it->first;
    output << prefix;
  }

  for( unsigned int i = 1; i < numIterations; i++ )
  {
    auto&& words = database.at( prefix );

    std::uniform_int_distribution<std::size_t> distribution( 0, words.size() - 1 );
    std::size_t index = distribution( engine );
    std::string word  = words.at( index );

    // Choose a new prefix. We do this by splitting the old prefix into a list
    // of tokens, deleting the first one, and appending the current word.

    std::list<std::string> prefixTokens;
    split( prefix,
           std::back_inserter( prefixTokens ) );

    prefixTokens.pop_front();
    prefixTokens.push_back( word );

    prefix = join( prefixTokens.begin(), prefixTokens.end() );

    if( !isPunctuation( word ) )
      output << " ";

    output << word;
  }

  return output.str();
}

int main( int, char** )
{
  auto tokens   = getTokens( "../King_James.txt" );
  auto database = buildDatabase( tokens, 2 );

  std::cout << spew( database, 100 ) << std::endl;

  return 0;
}
