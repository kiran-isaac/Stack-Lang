test:
	ld -r -b binary -o binary.o a.out
	g++ -o bask_portable_test binary.o main.cpp ../stack.cpp ../inbuilt.cpp ../byte_ops.cpp
	./bask_portable_test

build:
	g++ -c main.cpp ../stack.cpp ../inbuilt.cpp ../byte_ops.cpp
	ld -o bask_portable -r main.o stack.o inbuilt.o byte_ops.o
	rm *.o
	mv bask_portable /home/kiran/baskvm/build/bask_portable