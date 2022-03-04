all:
	../build/bask -c -o a.out -i stack.bsk str.bsk
	rm -f /usr/lib/bask/stdlib
	rm -f -r /usr/lib/bask
	mkdir /usr/lib/bask
	ln a.out /usr/lib/bask/stdlib