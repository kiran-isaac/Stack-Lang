all:
	@cd build && make && rm -f /usr/bin/bask && ln bask /usr/bin
	cd src/portable && sudo make build
	@cd std && make

fib:
	make all
	cd build && ./baskvm compile ../test/fib.bsk
	./build/baskvm run test/a.out