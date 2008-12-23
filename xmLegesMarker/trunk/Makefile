PREFIX =../compile
BINDIR=$(PREFIX)/bin
LIBDIR=$(PREFIX)/lib
INCLUDEDIR = $(PREFIX)/include

TARGETBIN = xmLeges-Marker.exe

#CFLAGS=-ggdb -Wall -I$(INCLUDEDIR)
#CFLAGS=-ggdb  -Wall -I$(INCLUDEDIR) `xml2-config --cflags`
CFLAGS=-ggdb  -Wall -I$(INCLUDEDIR) `xml2-config --cflags` `uname | grep -q Linux && echo -static`
CC = gcc
CXX = g++
LEXFLAGS= -i -8
# flag per debug su flex
LEXFLADEB= -i -8 -d


#LIBS=-L$(LIBDIR) -littig-1.0 -lFeatureExtractor -lHeaderParser -lhmm -llexer -lstem `xml2-config --libs`
LIBS=-L$(LIBDIR) -littig-1.0 -lFeatureExtractor -lHeaderParser -lhmm -llexer -lstem -L/usr/local/lib -lxml2 -lm -lpthread -liconv
#LIBS=-L$(LIBDIR) -littig-1.0 -lFeatureExtractor -lHeaderParser -lhmm -llexer -lstem -L/usr/local/lib -lxml2 -lz -lm -lpthread
#MAKELIB = ar rvs

OBJS = parser.o util.o tag.o config.o sequenze.o \
	testa.o coda.o \
	virgolette.o  \
	pre.o pre.lex.yy.o \
	prehtml.o prehtml.lex.yy.o \
	normalizza.o normalizza.lex.yy.o \
	annessi.o annessi.lex.yy.o \
	articolato.o articolato.lex.yy.o \
	struttura.o \
	tabelle.o tabelle.lex.yy.o\
	dom.o\
	disposto.o disposto.lex.yy.o\
	disegni.lex.yy.o\
	prexml.o prexml.lex.yy.o\
	xml.o
	
	
all: $(TARGETBIN)

%.c: %.y


xmLeges-Marker.exe: $(OBJS)
	$(CXX) -o $@ $(OBJS) $(CFLAGS) $(LIBS)

struttura.o: struttura.c struttura.h virgolette.h disposto.h
	$(CXX) -c $(CFLAGS) struttura.c

annessi.lex.yy.o: annessi.lex
	flex $(LEXFLAGS) -Pann -oannessi.lex.yy.c annessi.lex
	$(CC) -c $(CFLAGS) annessi.lex.yy.c

annessi.o: annessi.c annessi.h
	$(CXX) -c $(CFLAGS) annessi.c
	
dom.o: dom.c dom.h
	$(CC) -c $(CFLAGS) dom.c

pre.lex.yy.o: pre.c pre.lex
	flex $(LEXFLAGS) -Ppre -opre.lex.yy.c pre.lex
	$(CC) -c $(CFLAGS) pre.lex.yy.c
	
pre.o: pre.c pre.h
	$(CC) -c $(CFLAGS) pre.c

prehtml.lex.yy.o: prehtml.c prehtml.lex
	flex $(LEXFLAGS) -Pprehtml -oprehtml.lex.yy.c prehtml.lex
	$(CC) -c $(CFLAGS) prehtml.lex.yy.c

disegni.lex.yy.o: disegni.lex disegni.h
	flex $(LEXFLAGS) -Pddl -odisegni.lex.yy.c disegni.lex
	$(CC) -c $(CFLAGS) disegni.lex.yy.c

disposto.lex.yy.o: disposto.lex disposto.h
	flex $(LEXFLAGS) -Pdis -odisposto.lex.yy.c disposto.lex
	$(CC) -c $(CFLAGS) disposto.lex.yy.c

disposto.o: disposto.c disposto.h
	$(CC) -c $(CFLAGS) disposto.c

tabelle.lex.yy.o: tabelle.lex
	flex $(LEXFLAGS) -Ptab -otabelle.lex.yy.c tabelle.lex
	$(CC) -c $(CFLAGS) tabelle.lex.yy.c

tabelle.o: tabelle.c tabelle.h
	$(CC) -c $(CFLAGS) tabelle.c

articolato.lex.yy.o: articolato.lex
	flex $(LEXFLAGS) -Part -oarticolato.lex.yy.c articolato.lex
	$(CC) -c $(CFLAGS) articolato.lex.yy.c

articolato.o: articolato.c articolato.h
	$(CC) -c $(CFLAGS) articolato.c

normalizza.lex.yy.o: normalizza.lex
	flex $(LEXFLAGS) -Pnor -onormalizza.lex.yy.c normalizza.lex
	$(CC) -c $(CFLAGS) normalizza.lex.yy.c

normalizza.o: normalizza.c normalizza.h
	$(CC) -c $(CFLAGS) normalizza.c

config.o: config.c config.h
	$(CC) -c $(CFLAGS) config.c

parser.o: parser.c parser.h
	$(CXX) -c $(CFLAGS) parser.c

tag.o: tag.c tag.h util.h tag.h costanti.h
	$(CC) -c $(CFLAGS) tag.c

util.o: util.c util.h
	$(CC) -c $(CFLAGS) util.c

sequenze.o: sequenze.c sequenze.h tag.h
	$(CC) -c $(CFLAGS) sequenze.c

testa.o: testa.c testa.h config.h
	$(CXX) -c $(CFLAGS) testa.c

coda.o: coda.c coda.h config.h
	$(CXX) -c $(CFLAGS) coda.c
	
virgolette.o: virgolette.c virgolette.h tag.h
	$(CC) -c $(CFLAGS) virgolette.c

prexml.lex.yy.o: prexml.c prexml.lex
	flex $(LEXFLAGS) -Pprexml -oprexml.lex.yy.c prexml.lex
	$(CC) -c $(CFLAGS) prexml.lex.yy.c

xml.o: xml.c xml.h
	$(CXX) -c $(CFLAGS) xml.c
	

test: all
	cd test && $(MAKE) test

clean:
	rm -f *.o *.exe xmLeges-Marker.exe *.yy.c *.tab.* *~

install : all
	strip $(TARGETBIN) || strip *.exe
	cp ${TARGETBIN} ${BINDIR} || cp *.exe ${BINDIR}
