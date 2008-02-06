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

#include "util.h"

#include <IttigLogger.h>

/******************************************************************************/
/******************************************************************** CODA ****/
/******************************************************************************/

int coda( int pnotes,xmlNodePtr ptipodoc, xmlNodePtr pCorpo,xmlNodePtr pmeta,xmlNodePtr pdescrittori,
			xmlNodePtr pformulafinale,xmlNodePtr pconclusione, int tdoc) {
	
	loggerInfo("INIZIO Coda");
	
	HeaderParser parser(configHeaderParserModels());
	parser.setRootNode(ptipodoc);

	char *tmptxt = (char *) xmlNodeListGetString(utilGetDoc(), pCorpo, 1);	
	std::string footer = "";

	if(tmptxt != NULL && strcmp(tmptxt, "") != 0) {
		footer = utilConvTextToIso(tmptxt);
		footer.insert(0,"\n");
	} else {
		printf("\n>> Warning: footer is null or empty, skipping footer analysis!\n");
		return pnotes;
	}
		
	pnotes=parser.parseFooter(footer, pCorpo, pmeta, pdescrittori, pformulafinale, pconclusione, tdoc, pnotes);
	loggerInfo("FINE Coda");
	
	utilPercCalc(43);
	return pnotes ;
}
