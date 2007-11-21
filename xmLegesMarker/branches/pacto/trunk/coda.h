/******************************************************************************
* Project:	xmLeges
* Module:	Marker
* File:		coda.h
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Mirco Taddei (m.taddei@ittig.cnr.it)
* 			Lorenzo Bacci (bacci@ittig.cnr.it)
******************************************************************************/
#ifndef CODA_H
#define CODA_H

#include <stdio.h>
int coda( int pnotes,xmlNodePtr ptipodoc, xmlNodePtr pCorpo,xmlNodePtr pmeta,
				xmlNodePtr pdescrittori,xmlNodePtr pformulafinale,xmlNodePtr pconclusione,int tdoc);
#endif
