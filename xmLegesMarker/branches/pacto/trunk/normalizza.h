/******************************************************************************
* Project:	xmLeges
* Module:	Marker
* File:		normalizza.h
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Mirco Taddei (m.taddei@ittig.cnr.it)
******************************************************************************/
#ifndef NORMALIZZA_H
#define NORMALIZZA_H

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <IttigLogger.h>

#ifdef __cplusplus
extern "C" {
#endif

char *normalizza(char *testo);
void norAppendString(char *s);
void norAppendChars(int n, int c);

#ifdef __cplusplus
}
#endif

#endif
