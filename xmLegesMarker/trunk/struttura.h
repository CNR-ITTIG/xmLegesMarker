/******************************************************************************
* Project:	xmLeges
* Module:	Marker
* File:		struttura.h
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Mirco Taddei (m.taddei@ittig.cnr.it)
******************************************************************************/
#ifndef STRUTTURA_H
#define STRUTTURA_H

#include <libxml/tree.h>
#include <stdio.h>
#include "tag.h"
#include "virgolette.h"
#include "articolato.h"

#include "testa.h"
#include "coda.h"
#include "util.h"
#include "dom.h"
#include "config.h"
#include "tabelle.h"

#include <IttigUtil.h>
#include <IttigLogger.h>

typedef enum _ruoloDoc {
	principale,
	allegato
} ruoloDoc;

void MoveNotesInMeta (xmlNodePtr pParentNode, xmlNodePtr predazionale);

xmlNodePtr StrutturaAnalizza (char *buffer, ruoloDoc ruolo);

#endif
