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

doc:
	doxygen documentation/Doxyfile
	cd html
	git push origin gh-pages
	cd ..
