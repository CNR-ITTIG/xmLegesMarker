
#include <HeaderParser.h>
#include <IttigLogger.h>
#include "config.h"
#include "testa.h"

/******************************************************************************/
/******************************************************************* TESTA ****/
/******************************************************************************/

int testa(xmlNodePtr pTextNode, xmlNodePtr ptipodoc, xmlNodePtr pmeta,
		  xmlNodePtr pdescrittori,
		  xmlNodePtr pintestazione,
		  xmlNodePtr pformulainiziale) {

	loggerInfo("INIZIO Testa");
	
	char *tmptxt;
	if (pTextNode){
		tmptxt=(char *)xmlNodeGetContent(pTextNode);
	}
	else { //Se non vi è testo
		tmptxt="";
	}

	std::string	tmpstr=tmptxt;
	tmpstr.insert(0,"\n"); //inserito un \n prima del testo xchè l'header parser altrimenti mangia il testo

	//cout << "[" << tmpstr << "]" << endl;

	HeaderParser parser(configHeaderParserModels());
	parser.setRootNode(ptipodoc);
	int notes = 1;
	
	if(configGetDocTestoTipo() == unknown) {
		//Esegui il parser per cercare di individuare il tipo di documento...
		notes=parser.parseHeaderGetTipo(tmpstr, notes);
		exit(-1);	
	}

	//Aggiunta
	int tdoc = 0; //variabile che tiene conto del tipo di documento
	if(configGetDocTestoTipo() == disegnolegge)
		tdoc=1;

	if(configGetDocTestoTipo() == provCNR)
		tdoc=2;

	notes=parser.parseHeader(tmpstr, pmeta, pdescrittori, pintestazione, pformulainiziale, tdoc, notes);
	
	loggerInfo("FINE Testa");

	utilPercCalc(44);

	return notes;
}

/*
const char *testaOLD(char *testo, int * p_notes) {
	tag *t;
	register int i;
	FILE *fpsub;
	char cmd[MAXSTRINGA+1];

	t=tagNumero(0);

	std::ostringstream out;
	assert(out.good());
	std::ostringstream header;
	if (t)
		for (i=0; i<t->inizio; i++)
			header << (  char) testo[i];
	else
		header << testo;
	HeaderParser parser(configHeaderParserModels());
	std::istringstream in(header.str());
	*p_notes = parser.parseHeader(in, out);
	return strdup(out.str().c_str());
}
*/

