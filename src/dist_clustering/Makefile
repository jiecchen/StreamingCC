# Everybody can have different makefiles for each machine
#for silo:
CC=mpic++

#for Bigred||
#CC=cc

CFLAGS= -Wall -lm -std=c++11

#if needed:
INCLUDE = -I/usr/local/include
LIBS = -L/usr/local/lib -lblas -llapack -lm -L./bin/ -lalgorithms

DEST=./distBin/

all:
	cd bin && cmake .. && make
	make local

local:
	make clean
	make distAlgo.o
	make test 
	make main
	make algo

	
	


distAlgo.o:
	$(CC) $(CFLAGS)  -o $(DEST)distAlgo_util.o -c ./distAlgo/distAlgo_util.cc 
	$(CC) $(CFLAGS)  -o $(DEST)multiRoundSampling.o -c ./distAlgo/multiRoundSampling.cc
	$(CC) $(CFLAGS)  -o $(DEST)partitioning.o -c ./distAlgo/partitioning.cc
	

test:
	$(CC) $(CFLAGS) -o $(DEST)distAlgo_util_test ./mpiTest/distAlgo_util_test.cc $(DEST)distAlgo_util.o $(DEST)partitioning.o $(DEST)multiRoundSampling.o $(LIBS)
	$(CC) $(CFLAGS) -o $(DEST)partitioning_test ./mpiTest/partitioning_test.cc $(DEST)distAlgo_util.o $(DEST)partitioning.o $(DEST)multiRoundSampling.o $(LIBS)
	$(CC) $(CFLAGS) -o $(DEST)multi_round_sampling_test ./mpiTest/multi_round_sampling_test.cc $(DEST)distAlgo_util.o $(DEST)partitioning.o $(DEST)multiRoundSampling.o $(LIBS)

main:
	$(CC) $(CFLAGS) -o $(DEST)exemain main.cc $(DEST)distAlgo_util.o $(DEST)partitioning.o $(DEST)multiRoundSampling.o $(LIBS)

algo:
	pwd
	#$(CC) $(CFLAGS) -o algo algo.cc distAlgo_util.o partitioning.o multiRoundSampling.o $(LIBS)

algo:
	$(CC) $(CFLAGS) -o algo algo.cc distAlgo_util.o partitioning.o multiRoundSampling.o $(LIBS)


clean:
	rm $(DEST)*.o -f
	rm $(DEST)*test -f
	rm $(DEST)exe* -f

	
