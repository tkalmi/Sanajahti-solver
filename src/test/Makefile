#Please note this path as most of the errors may occur because
#the gtest folder cannot be found or it's not the correct version (1.7.0)
GTEST_DIR = $(HOME)/gtest-1.7.0

# Where to find user code.
USER_DIR = ..
TEST_DIR = .

# Flags passed to the preprocessor.
# Set Google Test's header directory as a system directory, such that
# the compiler doesn't generate warnings in Google Test headers.
CPPFLAGS += -isystem $(GTEST_DIR)/include

# Flags passed to the C++ compiler.
CXXFLAGS += -std=c++0x -g -Wall -Wextra -pthread -Wno-missing-field-initializers

# All tests produced by this Makefile.  Remember to add new tests you
# created to the list.
TESTS = solver_test

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

# House-keeping build targets.

all : $(TESTS)

clean :
	rm -f $(TESTS) gtest.a gtest_main.a *.o


# Builds gtest.a and gtest_main.a.

# Usually you shouldn't tweak such internal variables, indicated by a
# trailing _.
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

# For simplicity and to avoid depending on Google Test's
# implementation details, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Test
# compiles fast and for ordinary users its source rarely changes.
gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

# Builds a sample test.  A test should link with either gtest.a or
# gtest_main.a, depending on whether it defines its own main()
# function.

#Note that the main class here is not the project's actual main
#but own main class that the tests use
main.o : $(TEST_DIR)/main.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(TEST_DIR)/main.cpp

solver.o : $(USER_DIR)/solver.cpp $(USER_DIR)/solver.hpp $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/solver.cpp

ocr.o : $(USER_DIR)/ocr.cpp $(USER_DIR)/ocr.hpp $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/ocr.cpp

test_source.o : $(TEST_DIR)/test_source.cpp 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(TEST_DIR)/test_source.cpp

solver_test : main.o solver.o ocr.o test_source.o gtest_main.a -llept -ltesseract
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@
