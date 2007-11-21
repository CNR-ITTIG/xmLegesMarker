/******************************************************************************
* Project:	xmLeges
* Module:	Marker
* File:		sequenze.h
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Mirco Taddei (m.taddei@ittig.cnr.it)
******************************************************************************/
#ifndef __SEQUENZE_H__
#define __SEQUENZE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "tag.h"

void sequenzeClear(void);
void sequenzaInit(tagTipo tipo);
int sequenzaCheck(tagTipo tipo, int num, int lat);
tagNum * sequenzaValore(tagTipo tipo);
void sequenzaInc(tagTipo tipo);
void sequenzaSet(tagTipo tipo,int num);
int sequenzaGetNum(tagTipo tipo);
int sequenzaGetLat(tagTipo tipo);
#ifdef __cplusplus
}
#endif

#endif
