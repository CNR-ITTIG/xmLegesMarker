BINDIR=../../bin
MAKELIB = ar rvs
TARGETLIB = libhmm.a
LIBDIR=../../lib
TARGETBIN = HMM
OBJECTS= HMM.o
COMPILEXX = g++ -O3

all : ${TARGETBIN} lib

lib : ${OBJECTS}
	${MAKELIB} ${TARGETLIB} $+

HMM : StandaloneHMM.o
	${COMPILEXX} -o $@ $<

install : all
	cp ${TARGETBIN} *.pl ${BINDIR}; cp ${TARGETLIB} ${LIBDIR}

StandaloneHMM.o : HMM.cpp
	${COMPILEXX} -c $< -DSTANDALONEHMM -o $@

%.o : %.cpp
	${COMPILEXX} -c $< -o $@

clean:	
	rm *.o *.a ${TARGETBIN} -f 
