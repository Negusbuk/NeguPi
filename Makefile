all:
	make -C src
	make -C examples

install:
	make -C src install
	make -C examples install

distclean: clean
	make -C src distclean
	make -C examples distclean

clean:
	make -C src clean
	make -C examples clean