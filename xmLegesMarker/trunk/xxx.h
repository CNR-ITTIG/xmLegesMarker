#ifndef __XXX_H__
#define __XXX_H__


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
	void		xxxClearStateFrom(tagTipo ptag);
	void		xxxSetCurrentTagState(tagTipo ptag,xmlNodePtr pnode);

	void		xxxSetID(tagTipo ptag,int numConv,int latConv);
	void		xxxSetID2(tagTipo ptag,char *pnomeattr,int pnumConv,int platConv);
	void		xxxSetIDtoNode(xmlNodePtr	pnodo, tagTipo ptag,int pnumConv,int platConv,char *pnomeattr);
	
	char		*xxxExtractStringBeforeIndex(int pindex);
	void		xxxAppendTextToLastNode(int pIndex);

    void		xxxInit(tagTipo tRoot,xmlNodePtr	pRoot,  char *ptext);
	void		xxxClose(void);

	void		xxxTagAdd3(tagTipo ptag,int pindex,int plen,int pnumConv,int platConv);
	xmlNodePtr	xxxTagOpen(tagTipo ptag,int pindex,int plen);
	void		xxxTagInsertEmpty(tagTipo tparent,tagTipo tcur,int pindex);
	void		xxxTagClose(tagTipo t);
	void		xxxTagCloseFrom(tagTipo t);

	void		xxxAttributeIDUpdate(xmlNodePtr node,char *pParentID, char * pAnnessoParentID);
	xmlNodePtr	xxxGetLastTag(tagTipo ptag);

#ifdef __cplusplus
}
#endif

#endif

