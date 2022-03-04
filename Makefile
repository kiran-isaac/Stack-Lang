all:
	cd build && make
	cd src/portable && make build

fib:
	make all
	cd build && ./baskvm compile ../test/fib.bsk
	./build/baskvm run test/a.out