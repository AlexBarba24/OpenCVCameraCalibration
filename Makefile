# Sample makefile for EECS 280 Setup Tutorials

CXX ?= g++ 
CXXFLAGS ?= -Wall -Werror -pedantic -g --std=c++17 -Wno-c11-extensions -Wno-sign-compare -Wno-comment `pkg-config --cflags --libs opencv4` -L/usr/local/lib -rpath /usr/local/lib

# Compile the main executable
main.exe: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o main.exe
	# ./main.exe ./pics

# Remove automatically generated files
clean :
	rm -rvf *.exe *~ *.out *.dSYM *.stackdump

# Disable built-in rules
.SUFFIXES:
