/******************************************************************************
* Project:	xmLeges
* Module:	Marker
* File:		virgolette.h
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Mirco Taddei (m.taddei@ittig.cnr.it)
* 			Lorenzo Bacci (lorenzobacci@gmail.com)
******************************************************************************/
#ifndef VIRGOLETTE_H
#define VIRGOLETTE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include "dom.h"
#include "tag.h"
#include <libxml/tree.h>

void virgoletteInMod(xmlNodePtr pParentNode);
int checkCommaMod(char *txt, xmlNodePtr corpo);
void replaceNode(xmlNodePtr a, xmlNodePtr b, xmlNodePtr p);
xmlNodePtr replaceNodeWithChildren(xmlNodePtr a, xmlNodePtr b, xmlNodePtr p);

#ifdef __cplusplus
}
#endif

#endif
