/******************************************************************************
* Project:	xmLeges
* Module:	Marker
* File:		coda.c
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Mirco Taddei (m.taddei@ittig.cnr.it)
* 			Lorenzo Bacci (bacci@ittig.cnr.it)
******************************************************************************/
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

#include "coda.h"
#include "util.h"

#include <IttigLogger.h>

/******************************************************************************/
/******************************************************************** CODA ****/
/******************************************************************************/

int coda( int pnotes,xmlNodePtr ptipodoc, xmlNodePtr pCorpo,xmlNodePtr pmeta,xmlNodePtr pdescrittori,
			xmlNodePtr pformulafinale,xmlNodePtr pconclusione, int tdoc) {

	if(configGetDisableFooter()) {
		return pnotes;
	}

	//FIX: Salva un eventuale PI/error msg e lo riaggiunge in seguito
	//L'HeaderParser lo elimina durante l'analisi del footer
	if(pCorpo == NULL) {
		//printf("\ncoda() warning! pCorpo is null! Skipping footer analysis..\n");
		return 0;
	}

	char *piContent = NULL;
	xmlNodePtr corpoNode = pCorpo->parent;
	xmlNodePtr cnode = corpoNode->children;
	//printf("\nUNZ corpoNode:%s cnode:%s\n", corpoNode->name, cnode->name);
	if( cnode != NULL && !xmlStrcmp(cnode->name, BAD_CAST "error") ) {
		piContent = strdup((char *) xmlNodeGetContent(cnode));
		//printf("\npi:%s\n", piContent);
	}

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

	//Inserisce di nuovo l'error msg
	if(piContent != NULL && !domPIAdded(corpoNode) ) {
		xmlNodePtr piNode = xmlNewPI(BAD_CAST tagTipoToNome(tagerrore), BAD_CAST piContent);
		xmlAddChild(corpoNode, piNode);
	}

	utilPercCalc(43);
	return pnotes ;
}

//Ritorna 1 se nella lista children c'è un nodo error
int domPIAdded(xmlNodePtr node) {
	xmlNodePtr child=NULL;
	child = node->children;
	//printf("\ndomPIAdded()");
	while(child!=NULL) {
		//printf("\n name:%s\n", (char *)child->name);
		if(!xmlStrcmp(child->name, BAD_CAST "error"))
			return 1;
		child=child->next;
	}
	return 0;
}
