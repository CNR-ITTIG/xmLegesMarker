PREFIX = /usr/local
BINDIR = $(PREFIX)/bin
LIBDIR = $(PREFIX)/lib

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
	cp $(TARGETBIN) *.pl $(BINDIR); cp $(TARGETLIB) $(LIBDIR)

StandaloneHMM.o: HMM.cpp
	$(COMPILEXX) -c $< -DSTANDALONEHMM -o $@

%.o: %.cpp
	$(COMPILEXX) -c $< -o $@

clean:
	rm -f *.o *.a $(TARGETBIN)  $(TARGETLIB)
