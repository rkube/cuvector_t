#CC = /opt/lo/bin/g++-mp-6
CC = /Users/ralph/local/bin/clang++
CUDACC = /home/rku000/local/bin/clang++
CFLAGS = -std=c++14 -O0 -g -stdlib=libc++


CUDACFLAGS = -std=c++14 -stdlib=libc++ --cuda-gpu-arch=sm_50 -Wall
CUDALFLAGS = -lcudart_static -ldl -lm -lpthread -lrt 

test_transform: test_transform.cpp include/iterators.h include/myvector.h
	$(CC) $(CFLAGS) -o test_transform test_transform.cpp

test_bounds: test_bounds.cpp include/bounds.h
	$(CC) $(CFLAGS) -o test_bounds test_bounds.cpp

test_view: test_view.cpp include/vector2d.h include/view.h include/bounds.h
	$(CC) $(CFLAGS) -o test_view test_view.cpp

test_interp: test_interp.cpp include/vector2d.h include/view.h include/bounds.h include/interp.h
	$(CC) $(CFLAGS) -o test_interp test_interp.cpp

test_apply: test_apply.cpp include/vector2d.h include/utility.h
#	$(CC) $(CFLAGS) -o test_apply test_apply.cpp
	$(CUDACC) $(CUDACFLAGS) -x cuda -o test_apply test_apply.cpp $(CUDALFLAGS)


test_vector_device: test_vector_device.cpp
	$(CC) $(CUDACFLAGS) -x cuda -o test_vector_device test_vector_device.cpp $(CUDALFLAGS)
