all:
	../build/slang compile -o a.out stack.sl str.sl
	rm -f /usr/lib/slang/stdlib
	rm -f -r /usr/lib/slang
	mkdir /usr/lib/slang
	ln a.out /usr/lib/slang/stdlib