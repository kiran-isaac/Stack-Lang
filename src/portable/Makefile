test:
	ld -r -b binary -o binary.o a.out
	g++ -o slang_mini_test binary.o main.cpp ../stack.cpp ../inbuilt.cpp ../byte_ops.cpp
	./slang_mini_test

build:
	g++ -c main.cpp ../vm/stack.cpp ../vm/inbuilt.cpp ../vm/byte_ops.cpp
	ld -o slang_mini -r main.o stack.o inbuilt.o byte_ops.o
	rm *.o
	rm -f /usr/lib/slang/slang_mini
	ln slang_mini /usr/lib/slang/slang_mini