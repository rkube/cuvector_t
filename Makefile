CC = /Users/ralph/local/bin/clang++
CFLAGS = -std=c++14

test_transform: test_transform.cpp
	$(CC) $(CFLAGS) -o test_transform test_transform.cpp