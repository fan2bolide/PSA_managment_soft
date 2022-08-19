GTEST_ROOT = googletest/googletest

CFLAGS = -Wall -Wextra -pedantic -g -O2 -std=c99
CXXFLAGS = -Wall -Wextra -pedantic -g -O2 -std=c++11 -I$(GTEST_ROOT)/include -I$(GTEST_ROOT)
LDLIBS = -lpthread

all: hashtable

hashtable: hashtable_test.o hashtable.o $(GTEST_ROOT)/src/gtest-all.o
	$(CXX) -o $@ $^ -lpthread

clean:
	rm -f hashtable_test.o hashtable.o $(GTEST_ROOT)/src/gtest-all.o
	rm -f hashtable
