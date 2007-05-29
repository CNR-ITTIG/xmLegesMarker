/******************************************************************************
* Project:	xmLeges
* Module:	Marker
* File:		testa.h
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Mirco Taddei (m.taddei@ittig.cnr.it)
* 			Lorenzo Bacci (bacci@ittig.cnr.it)
******************************************************************************/
#ifndef TESTA_H
#define TESTA_H


#include <libxml/tree.h>
//const char *testa(char *testo, int * p_notes);

 int testa(xmlNodePtr pTextNode, xmlNodePtr ptipodoc, xmlNodePtr pmeta,
		  xmlNodePtr pdescrittori,
		  xmlNodePtr pintestazione,
		  xmlNodePtr pformulainiziale,
		  int tdoc) ;

 char *sstring(char *tmp);
 
#endif

