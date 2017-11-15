CC=g++
CFLAGS=-g -Wno-write-strings -std=c++11 -O2
LIBS= -lnl-3 -lnl-genl-3 -lnl-3 -lssh -lm -pthread -z muldefs 

all: dosgen

trafgen_wrapper.o: trafgen_wrapper.c 
	g++ -c $(CFLAGS) -o $@ $^

libtrafgen.a:
	(cd trafgen; make trafgen && cp libtrafgen.a ../)

slowloris.o: slowloris.cpp
	g++ $^ -c $(CFLAGS) -o $@ 

httpflood.o: httpFlood.cpp
	g++ $^ -c $(CFLAGS) -o $@ 

slowRead.o: slowRead.cpp
	g++ $^ -c $(CFLAGS) -o $@ 

rudy.o: rudy.cpp
	g++ $^ -c $(CFLAGS) -o $@ 

dosgen.o: dosgen.c
	g++ -c $(CFLAGS) -o $@ $^

halaska.o: argsParse.c
	g++ -c $(CFLAGS) -o $@ $^

sshPass.o: sshPass.c
	g++ $^ -c $(CFLAGS) -o $@ 

# DoSgen
dosgen:  dosgen.o slowloris.o rudy.o httpflood.o sshPass.o slowRead.o argsParse.o trafgen_wrapper.o libtrafgen.a 
	g++  $^ $(LIBS) -o $@

clean:
	rm dosgen
	rm *.o
	rm *.a


