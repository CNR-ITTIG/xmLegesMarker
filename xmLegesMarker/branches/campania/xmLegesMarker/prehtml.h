/******************************************************************************
* Project:	xmLeges
* Module:	Marker
* File:		prehtml.h
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Mirco Taddei (m.taddei@ittig.cnr.it)
******************************************************************************/
#ifndef PREHTML_H
#define PREHTML_H

#ifdef __cplusplus
extern "C" {
#endif

void prehtmlAppendString(char *s);
void prehtmlAppendChars(int n, int c) ;
char *prehtmlAnalizza(char *testo);
char prehtmlExtractChar(int elemento);

#ifdef __cplusplus
}
#endif

#endif /* PREHTML_H */
