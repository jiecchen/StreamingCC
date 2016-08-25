streamingcc:
	python setup.py build_ext --inplace
	sudo rm -rf build/

install:
	sudo python setup.py build_ext install
	sudo rm -rf build/

clean:
	rm -rf build
	find . -name "*.so" | xargs rm -f
	find . -name "*~" | xargs rm -f
	find . -name "*.pyc" | xargs rm -f
	find . -name "*.cpp" | xargs rm -f
