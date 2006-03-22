#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>
//#include <assert.h>
//#include <fstream>
//#include <sstream>
#include "config.h"
#include "costanti.h"
#include "HeaderParser.h"

#include "util.h"

#include <IttigLogger.h>

/******************************************************************************/
/******************************************************************** CODA ****/
/******************************************************************************/

int coda( int pnotes,xmlNodePtr ptipodoc, xmlNodePtr pCorpo,xmlNodePtr pmeta,xmlNodePtr pdescrittori,
			xmlNodePtr pformulafinale,xmlNodePtr pconclusione, int tdoc) {
	
	loggerInfo("INIZIO Coda");
	
	//xmlNodePtr lastcomma_node = xmlNewChild(articolato_node, NULL, BAD_CAST "corpo", BAD_CAST text.c_str()); // prova
    // if (pCorpo==NULL)printf("ELEMENTO VUOTO");
	//else {
	//	printf("->>>%s<<<-\n",xmlNodeGetContent(pCorpo));
	//}
	HeaderParser parser(configHeaderParserModels());
	parser.setRootNode(ptipodoc);

	//Aggiunta
	/*
	int tdoc = 0; //variabile che tiene conto del tipo di documento
	if(configGetDocTestoTipo() == disegnolegge)
		tdoc=1;
	if(configGetDocTestoTipo() == provCNR)
		tdoc=2;
	*/
		
	pnotes=parser.parseFooter(pCorpo, pmeta, pdescrittori, pformulafinale, pconclusione, tdoc, pnotes);
	loggerInfo("FINE Coda");
	
	utilPercCalc(43);
	return pnotes ;
}

/*
const char * coda(char *testo, int *p_notes) {
	tag *t;
	register int i;

	for (i=tagN()-1, t=tagNumero(i); i>=0 && t->tipo != corpo; i--, t=tagNumero(i))
		;

	std::ostringstream out;
	assert(out.good());
	std::ostringstream footer;
	for (i = t->inizio; testo[i]; i++)
		footer << (unsigned char) testo[i];
	HeaderParser parser(configHeaderParserModels());
	std::istringstream in(footer.str());
	*p_notes = parser.parseFooter(in, out, *p_notes);
	return strdup(out.str().c_str());
}


*/


