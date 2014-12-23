CMAKE_MINIMUM_REQUIRED( VERSION 2.8.9 )
PROJECT( Markov )

SET( CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -std=c++11" )

ADD_EXECUTABLE( markov markov.cc )
