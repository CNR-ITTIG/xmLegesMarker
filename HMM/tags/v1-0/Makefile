PREFIX = /usr/local
BINDIR = $(PREFIX)/bin
LIBDIR = $(PREFIX)/lib
INCLUDEDIR = $(PREFIX)/include

TARGETBIN = HMM
TARGETLIB = libhmm.a

COMPILEXX = g++ -O3 -Wno-deprecated
LIBS = -L$(LIBDIR) -llexer -LLexer
MAKELIB = ar rvs

OBJECTS= HMM.o



all: $(TARGETBIN) lib

lib: $(OBJECTS)
	$(MAKELIB) $(TARGETLIB) $+

HMM: StandaloneHMM.o
	$(COMPILEXX) -o $@ $<

install: all
	cp $(TARGETBIN) *.pl *.exe $(BINDIR); cp $(TARGETLIB) $(LIBDIR); cp *.h $(INCLUDEDIR)

StandaloneHMM.o: HMM.cpp
	$(COMPILEXX) -c $< -DSTANDALONEHMM -o $@

%.o: %.cpp
	$(COMPILEXX) -c $< -o $@

clean:
	rm -f *.o *.a *.exe $(TARGETBIN)  $(TARGETLIB)
