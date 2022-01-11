all:
	cd build && make

fib:
	make all
	cd build && ./baskvm compile ../test/fib.bsk
	./build/baskvm run test/a.out

