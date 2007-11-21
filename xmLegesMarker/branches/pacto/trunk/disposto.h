/******************************************************************************
* Project:	xmLeges
* Module:	Marker
* File:		disposto.h
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Mirco Taddei (m.taddei@ittig.cnr.it)
******************************************************************************/
#ifndef DISPOSTO_H
#define DISPOSTO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "struttura.h"

#ifdef __cplusplus
extern "C" {
#endif

void dispostoAnalizza(ruoloDoc ruolo, char *testo, char **disposto, char **prima, int size);

void disAppendString(char *s);
void disAppendChars(int n, int c);
void disSaveIni(void);

#ifdef __cplusplus
}
#endif

#endif
