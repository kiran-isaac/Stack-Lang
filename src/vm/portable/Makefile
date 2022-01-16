all:
	@ld -r -b binary -o binary.o a.out
	@g++ -o test.out binary.o main.cpp ../stack.cpp ../inbuilt.cpp ../byte_ops.cpp
	@./test.out