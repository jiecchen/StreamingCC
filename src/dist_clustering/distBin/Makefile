all:
	cd .. && make 

clean:
	cd ../bin && make clean 
	cd .. && make clean 

tags:
	cd .. && rm -f tags && ctags -R *

test:
	here := $(pwd) && echo $here
