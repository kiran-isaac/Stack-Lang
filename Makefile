all:
	@cd build && make && rm -f /usr/bin/slang && ln slang /usr/bin
	cd src/portable && sudo make build
	@cd std && make