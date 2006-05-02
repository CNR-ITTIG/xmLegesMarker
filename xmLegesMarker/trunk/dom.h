/******************************************************************************
* Project:	xmLeges
* Module:	Marker
* File:		dom.h
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Mirco Taddei (m.taddei@ittig.cnr.it)
* 			Lorenzo Bacci (lorenzobacci@gmail.com)
******************************************************************************/
#ifndef __DOM_H__
#define __DOM_H__


#define ATTRIB_ID	"id"
#define ATTRIB_ID_SEP	"-"


#ifdef __cplusplus
extern "C" {
#endif

	#include <libxml/tree.h>
	#include <stdio.h>
	#include "tag.h"
	#include "util.h"
	#include "config.h"

	//Funzioni di Azione sul vettore "State"
	void		domClearStateFrom(tagTipo ptag);
	void		domSetCurrentTagState(tagTipo ptag,xmlNodePtr pnode);

	void		domSetID(tagTipo ptag,int numConv,int latConv);
	void		domSetID2(tagTipo ptag,char *pnomeattr,int pnumConv,int platConv);
	void		domSetIDtoNode(xmlNodePtr	pnodo, tagTipo ptag,int pnumConv,int platConv,char *pnomeattr);
	
	char		*domExtractStringBeforeIndex(int pindex);
	void		domAppendTextToLastNode(int pIndex);

    void		domInit(tagTipo tRoot,xmlNodePtr	pRoot,  char *ptext);
	void		domClose(void);

	void		domTagAdd3(tagTipo ptag,int pindex,int plen,int pnumConv,int platConv);
	xmlNodePtr	domTagOpen(tagTipo ptag,int pindex,int plen);
	void		domTagInsertEmpty(tagTipo tparent,tagTipo tcur,int pindex);
	void		domTagClose(tagTipo t);
	void		domTagCloseFrom(tagTipo t);

	void		domAttributeIDUpdate(xmlNodePtr node,char *pParentID, char * pAnnessoParentID);
	xmlNodePtr	domGetLastTag(tagTipo ptag);
	void 		domAddRango(xmlNodePtr node, char *t);
	xmlNodePtr	domGetFirstNode(xmlNodePtr node);

#ifdef __cplusplus
}
#endif

#endif

