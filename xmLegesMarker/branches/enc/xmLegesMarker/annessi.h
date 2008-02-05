/******************************************************************************
* Project:	xmLeges
* Module:	Marker
* File:		annessi.h
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Mirco Taddei (m.taddei@ittig.cnr.it)
******************************************************************************/
#ifndef ANNESSI_H
#define ANNESSI_H

#include "costanti.h"
#include "struttura.h"
#include "dom.h"
#include "tag.h"
#include "util.h"
#include <string.h>
#include <libxml/tree.h>

#include <IttigLogger.h>


const void AnnessiAnalizza(  char *testo, xmlNodePtr pParentNode, tagTipo pTipoParentNode);
void AnnessiCicla(xmlNodePtr pNodoParent);
void AnnessiInit(void);
xmlNodePtr GerStrutturaPreAnnessi(xmlNodePtr pParentNode);

#ifdef __cplusplus
extern "C" {
#endif
	int _annessiLexStart(char *testo);
#ifdef __cplusplus
}
#endif

#endif

