test:
	ld -r -b binary -o binary.o a.out
	g++ -o bask_portable_test binary.o main.cpp ../stack.cpp ../inbuilt.cpp ../byte_ops.cpp
	./bask_portable_test

build:
	g++ -c main.cpp ../vm/stack.cpp ../vm/inbuilt.cpp ../vm/byte_ops.cpp
	ld -o bask_portable -r main.o stack.o inbuilt.o byte_ops.o
	rm *.o
	rm -f /usr/lib/bask/bask_portable
	ln bask_portable /usr/lib/bask/bask_portable