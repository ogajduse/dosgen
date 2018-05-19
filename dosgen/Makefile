CC = g++ #gcc
CXX = g++

CFLAGS = -g
CFLAGS += -Wno-write-strings
#CFLAGS += -std=c++11
CFLAGS += -O2

CPPFLAGS = -std=c++11

LIBS = -lnl-3
LIBS += -lnl-genl-3
LIBS += -lnl-3
LIBS += -lssh
LIBS += -lm
LIBS += -pthread
LIBS += -z muldefs

.PHONY: install, clean

all: dosgen

trafgen_wrapper.o: trafgen_wrapper.c 
	$(CC) -c $(CFLAGS) -o $@ $^

libtrafgen.a:
	$(MAKE) trafgen -C trafgen
	cp trafgen/libtrafgen.a ./

slowloris.o: slowloris.cpp
	$(CC) $^ -c $(CFLAGS) $(CPPFLAGS) -o $@

httpflood.o: httpFlood.cpp
	$(CC) $^ -c $(CFLAGS) $(CPPFLAGS) -o $@

slowRead.o: slowRead.cpp
	$(CC) $^ -c $(CFLAGS) $(CPPFLAGS) -o $@

rudy.o: rudy.cpp
	$(CC) $^ -c $(CFLAGS) $(CPPFLAGS) -o $@

dosgen.o: dosgen.c
	$(CC) -c $(CFLAGS) -o $@ $^

halaska.o: argsParse.c
	$(CC) -c $(CFLAGS) -o $@ $^

sshPass.o: sshPass.c
	$(CC) $^ -c $(CFLAGS) $(CPPFLAGS) -o $@

# DoSgen
dosgen:  dosgen.o slowloris.o rudy.o httpflood.o sshPass.o slowRead.o argsParse.o trafgen_wrapper.o libtrafgen.a 
	$(CC)  $^ $(LIBS) -o $@

install:
	mkdir -p $(DESTDIR)/usr/bin
	install -m 0755 dosgen $(DESTDIR)/usr/bin/dosgen

clean:
	$(RM) dosgen
	$(RM) *.o
	$(RM) *.a
	$(MAKE) clean -C trafgen
	@echo Clean completed!
