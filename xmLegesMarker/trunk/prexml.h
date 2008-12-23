/******************************************************************************
* Project:	xmLeges
* Module:	Marker
* File:		prexml.h
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Lorenzo Bacci (bacci@ittig.cnr.it)
******************************************************************************/
#ifndef PREXML_H
#define PREXML_H

#ifdef __cplusplus
extern "C" {
#endif

void prexmlAppendString(char *s);
void prexmlAppendChars(int n, int c) ;
char *prexmlAnalizza(char *testo);
char prexmlExtractChar(int elemento);


#ifdef __cplusplus
}
#endif

#endif /* PREXML_H */
