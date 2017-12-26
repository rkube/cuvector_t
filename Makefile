#CC = /opt/local/bin/g++-mp-6
CC = /Users/ralph/local/bin/clang++
CFLAGS = -std=c++14 -O0 -g -stdlib=libc++

test_transform: test_transform.cpp include/iterators.h include/myvector.h
	$(CC) $(CFLAGS) -o test_transform test_transform.cpp

test_bounds: test_bounds.cpp include/bounds.h
	$(CC) $(CFLAGS) -o test_bounds test_bounds.cpp

test_view: test_view.cpp include/vector2d.h include/view.h include/bounds.h
	$(CC) $(CFLAGS) -o test_view test_view.cpp
