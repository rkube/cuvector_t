CC = /opt/local/bin/g++-mp-6
CFLAGS = -std=c++14 -O0 -g

test_transform: test_transform.cpp include/iterators.h include/myvector.h
	$(CC) $(CFLAGS) -o test_transform test_transform.cpp
