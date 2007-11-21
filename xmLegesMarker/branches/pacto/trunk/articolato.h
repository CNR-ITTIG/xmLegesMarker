/******************************************************************************
* Project:	xmLeges
* Module:	Marker
* File:		articolato.h
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Mirco Taddei (m.taddei@ittig.cnr.it)
******************************************************************************/
#ifndef ARTICOLATO_H
#define ARTICOLATO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "tag.h"
#include "util.h"
#include "dom.h"
//#include "annessi.h"


#include <libxml/tree.h>
#include <IttigLogger.h>

#ifdef __cplusplus
extern "C" {
#endif

//#include "tag.h"
xmlNodePtr ArticolatoAnalizza(  char *testo);
void cercaArticoliVuoti(xmlNodePtr pArticolato);
void cercaCommiVuoti(xmlNodePtr pNodo);

#ifdef __cplusplus
}
#endif

#endif
