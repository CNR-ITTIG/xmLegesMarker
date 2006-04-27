/******************************************************************************
* Project:	xmLeges
* Module:	Marker
* File:		pre.h
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Mirco Taddei (m.taddei@ittig.cnr.it)
******************************************************************************/
#ifndef PRE_H
#define PRE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <malloc.h>
#include <string.h>

char *preAnalizza(char *testo);
void preAppendString(char *s);
void preAppendChars(int n, int c);

#ifdef __cplusplus
}
#endif

#endif
