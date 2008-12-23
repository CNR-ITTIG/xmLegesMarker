/******************************************************************************
* Project:	xmLeges
* Module:	Marker
* File:		tabelle.h
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Mirco Taddei (m.taddei@ittig.cnr.it)
******************************************************************************/
#ifndef __TABELLE_
#define __TABELLE_

#include <libxml/tree.h>
#include "dom.h"
#include "config.h"
#include "util.h"

#include <IttigLogger.h>

#ifdef __cplusplus
extern "C" {
#endif

	
	xmlNodePtr TabelleAnalizza(  char *buffer);

#ifdef __cplusplus
}
#endif


#endif
