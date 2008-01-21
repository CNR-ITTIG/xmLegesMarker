PREFIX=/usr/local
LIBDIR=$(PREFIX)/lib
BINDIR=$(PREFIX)/bin
INCLUDEDIR = $(PREFIX)/include
MAKELIB = ar rvs
TARGETLIB = libHeaderParser.a
ARLIBS=${LIBDIR}/liblexer.a $(LIBDIR)/libFeatureExtractor.a $(LIBDIR)/libhmm.a
LIBS=-lxml2 -L$(LIBDIR) -lFeatureExtractor -llexer -lhmm
OBJECTS = HeaderParser.o
CFLAGS=-ggdb -Wno-deprecated `xml2-config --cflags` -I$(INCLUDEDIR)
COMPILEXX = g++ -O3 $(CFLAGS)
TARGETBIN = HeaderParser

all : default ${TARGETBIN} lib

default :
	./loadDefaults.sh

lib : ${OBJECTS}
	${MAKELIB} ${TARGETLIB} $+ $(ARLIBS)

HeaderParser: provaparser.o StandaloneHeaderParser.o
	${COMPILEXX} $+ -o $@ ${LIBS}

provaparser.o : provaparser.c
	${COMPILEXX} -c $< -o $@

StandaloneHeaderParser.o : HeaderParser.cpp
	${COMPILEXX} -DHEADERPARSER -c $< -o $@

install : all
	strip ${TARGETBIN} || strip *.exe
	cp ${TARGETBIN} ${BINDIR} || cp *.exe ${BINDIR}
	cp ${TARGETLIB} ${LIBDIR}
	cp *.h $(INCLUDEDIR)

%.o : %.cpp
	${COMPILEXX} -c $< -o $@

clean:
	rm *.o *.a *.exe ${TARGETBIN} -f  *~ Default.h
