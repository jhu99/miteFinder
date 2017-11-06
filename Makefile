OS = $(shell uname -s)
CXX = g++
DEBUG = yes

# Default mode is "Release"
DEFAULT_MODE  = Release
MODE         ?= $(DEFAULT_MODE)
# If mode is something other than "Debug" or "Release",throw a fit
ifneq ($(MODE),Debug)
ifneq ($(MODE),Release)
$(error MODE must be one of {Debug,Release})
endif
endif
# Flags for compiling the code
ifeq ($(MODE),Debug)
CXXFLAGS = -Wall -g3 -DDEBUG -std=c++0x -DVERBOSE -Ilib/
else
CXXFLAGS = -Wall -O3 -ffast-math -Ilib/ -std=c++0x -DNDEBUG
endif
all: miteFinderTest patternLearningTest ArgParserTest
miteFinderTest: tests/miteFinderTest.cpp lib/genome.o lib/mite.o lib/argparser.o
	${CXX} ${CXXFLAGS} -o $@ $^
patternLearningTest: tests/patternTest.cpp lib/genome.o
	${CXX} ${CXXFLAGS} -o $@ $^
ArgParserTest: tests/testargparser.cpp lib/argparser.o
	${CXX} ${CXXFLAGS} -o $@ $^
clean:
	rm lib/*.o
