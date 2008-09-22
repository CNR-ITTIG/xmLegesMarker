/******************************************************************************
* Project:	xmLeges
* Module:	Marker
* File:		testa.c
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Mirco Taddei (m.taddei@ittig.cnr.it)
* 			Lorenzo Bacci (bacci@ittig.cnr.it)
******************************************************************************/
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
		  xmlNodePtr pformulainiziale,
		  int tdoc) {

	if(configGetDisableHeader()) {
		return 0;
	}			
	
	loggerInfo("INIZIO Testa");
	
	if(configTipoInput() == xml) { // problema per header allegati? -> controllare se ruoloDoc==principale
		return 0;
	}
	printf("\ntesta...\n");

	char *tmptxt = NULL;
	if (pTextNode) {
		//Considerando il testo all'interno dei vari tag come una lista di nodi testo/entità
		//non si può tirare fuori il contenuto del tag -errore- con xmlNodeGetContent(),
		//in particolare quando si utilizzano testi in html (ricchi di entità).
		
		//Problema: a volte pTextNode è un nodo di testo, altre è un nodo di nome "error"
		//che ha come figlio il nodo (o la lista di nodi) di testo:
		if(pTextNode->children != NULL)
			pTextNode = pTextNode->children;
		
		xmlChar *xmltxt=NULL;
		xmltxt=xmlNodeListGetString(NULL, pTextNode, 0);
		//Con la precedente riga si perdono le entità per avere un unico nodo di testo
		//(può dare problemi in fase di visualizzazione...)	
		
		tmptxt = (char *)xmltxt;
		if(tmptxt == NULL)
			tmptxt = "";
	} else { //Se non vi è testo
		tmptxt=""; 	//(...se tmptxt=="", si potrebbe anche returnare...)
	}
	
	std::string	tmpstr=tmptxt;
	tmpstr.insert(0,"\n"); //inserito un \n prima del testo xchè l'header parser altrimenti mangia il testo

	HeaderParser parser(configHeaderParserModels());
	parser.setRootNode(ptipodoc);
	int notes = 1;
	
	if(configGetDocTestoTipo() == unknown) {
		//Esegui il parser per cercare di individuare il tipo di documento...
		//notes=parser.parseHeaderGetTipo(tmpstr, notes);
		parser.parseHeaderGetTipo(tmpstr);
		exit(-1);	
	}

	notes=parser.parseHeader(tmpstr, pmeta, pdescrittori, pintestazione, pformulainiziale, tdoc, notes);
	
	loggerInfo("FINE Testa");
	utilPercCalc(44);
	return notes;
}

//Sostituisci eventuali caratteri > e < (messa qui per poter sfruttare gli oggetti 'std::string')
char *sstring(char *tmp){
	std::string buf = tmp;
	char * bufchar = NULL;
	unsigned int beg = 0;
	
	while((beg = buf.find("<",beg)) != string::npos)
		buf.replace(beg,1,"&lt;");
	beg = 0;
	while((beg = buf.find(">",beg)) != string::npos)
		buf.replace(beg,1,"&gt;");
	
	bufchar = (char *)malloc((buf.length()+1) * sizeof(char));
	if(bufchar==NULL)
		printf("\n>>ERROR<< sstring() - malloc() could not allocate memory!\n");
	
	sprintf(bufchar,"%s",buf.c_str());	
	return bufchar;
}

/*
char *getDecoration(char *tmp) {
	std::string buf = tmp;
	static string decorations[] = {"(L)","(l)","(R)","(r)","(LR)","(lr)","(L-R)","(l-r)"};
	
	unsigned int lastchar = buf.find_last_not_of(" \n\t\r");
	unsigned int dec = buf.find_last_of(")");
	
	//Se non termina con ')' esci
	if(lastchar!=dec) return NULL;
	
}
*/

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

