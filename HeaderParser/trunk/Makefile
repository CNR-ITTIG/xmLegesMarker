PREFIX=/usr/local
LIBDIR=$(PREFIX)/lib
BINDIR=$(PREFIX)/bin
MAKELIB = ar rvs
TARGETLIB = libHeaderParser.a
ARLIBS=${LIBDIR}/liblexer.a $(LIBDIR)/libFeatureExtractor.a $(LIBDIR)/libhmm.a
LIBS=-L$(LIBDIR) -lFeatureExtractor -llexer -lhmm
OBJECTS = HeaderParser.o
COMPILEXX = g++ -O3
TARGETBIN = HeaderParser

all : featExt hmm ${TARGETBIN} lib

featExt:
	cd ../FeatureExtractor && $(MAKE) install

hmm:
	cd ../HMM && $(MAKE) install

lib : ${OBJECTS}
	${MAKELIB} ${TARGETLIB} $+ $(ARLIBS)

HeaderParser: StandaloneHeaderParser.o
	${COMPILEXX} $+ -o $@ ${LIBS}

StandaloneHeaderParser.o : HeaderParser.cpp
	${COMPILEXX} -DHEADERPARSER -c $< -o $@

install : all
	cp ${TARGETBIN} ${BINDIR}; cp ${TARGETLIB} ${LIBDIR}

%.o : %.cpp
	${COMPILEXX} -c $< -o $@

clean:
	rm *.o *.a ${TARGETBIN} -f  *~
	cd ../FeatureExtractor && $(MAKE) clean
	cd ../HMM && $(MAKE) clean
