CXX=clang++
INCLUDES=-Iincludes/
CXX_FLAGS=-std=c++20 -gdwarf-4 -fstandalone-debug -O0 -Wall -Wextra -Werror -pedantic $(INCLUDES)

exec: bin/exec

bin/exec: obj/main.o obj/decoder.o obj/utils.o
	$(CXX) $(CXX_FLAGS) $^ -o $@

obj/main.o: src/main.cpp includes/decoder.h includes/utils.h
	$(CXX) $(CXX_FLAGS) -c $< -o $@

obj/decoder.o: src/decoder.cpp includes/decoder.h
	$(CXX) $(CXX_FLAGS) -c $< -o $@

obj/utils.o: src/utils.cpp includes/utils.h
	$(CXX) $(CXX_FLAGS) -c $< -o $@

.DEFAULT_GOAL := exec
.PHONY: clean exec

clean:
	rm -fr bin/* obj/*
