CXX=g++-6
CXXFLAGS=-Wall -Wextra -pedantic -std=c++11 -O0 -ggdb -Isrc --coverage -fsanitize=address
LDFLAGS=--coverage -lasan

.PHONY: all
all : testrunner

.PHONY: unittest
unittest : testrunner
	./testrunner

testrunner : src/tests.o
	$(CXX) -fuse-ld=gold $(LDFLAGS) -o $@ $^

src/tests.o : src/tests.cpp src/string_view.hpp

.PHONY: coverage
coverage : testrunner
	lcov --quiet --zerocounters --directory src --output-file coverage.info
	./testrunner
	lcov --quiet --capture --no-external --directory src --output-file coverage.info
	lcov --quiet --remove coverage.info '/usr/*'
	lcov --quiet --remove coverage.info '*doctest*'
	genhtml coverage.info --output-directory coverage

.PHONY: clean
clean :
	rm -f testrunner
	rm -f coverage.info
	rm -fr coverage
	find . -name "*.o" | xargs rm -f
	find . -name "*.gcda" | xargs rm -f
	find . -name "*.gcno" | xargs rm -f

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<
