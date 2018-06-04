CXX=g++
CXXFLAGS=	-O3	-std=c++11
output:	embedjoin.o	verification.o	time.o
	g++ embedjoin.o verification.o time.o -o embedjoin
clean:
	$(RM)	*.o	*.d	embedjoin output
-include	$(wildcard *.d)