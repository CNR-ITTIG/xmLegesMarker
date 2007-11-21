/******************************************************************************
* Project:	xmLeges
* Module:	Marker
* File:		util.h
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Mirco Taddei (m.taddei@ittig.cnr.it)
******************************************************************************/
#ifndef __UTIL_H__
#define __UTIL_H__

#include <iconv.h>
#include <ctype.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xmlstring.h>
#include <IttigUtil.h>
#include <IttigLogger.h>
#include "tag.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_AUTORITA		50
#define MAX_PROVVEDIMENTO	50

//new


void		utilPercCurrSet(int pPerc);
void		utilPercCalc(int pPerc);
void		utilPercNumBlockSet(int pNumAnnessi);
void		utilNodeDelete(xmlNodePtr pNodo);
tagTipo		utilString2TagTipo(char  *p);
void		_utilNodeCount(xmlNodePtr node);
void		utilNodeCount(xmlNodePtr parentNode);

/* Converte txtin dal formato originario (per default è iso-8859-15) al formato UTF-8 */
char		*utilConvertiText(char *txtin);

/* Questa fa il contrario: converte da UTF-8 nel formato originario (per default è iso-8859-15) */
char		*utilConvTextToIso(char *txtin);

/* Per settare e recuperare il valore del flag che indica se sono presenti annessi o meno ****/
void		SetFlagAnnessi(int flag);
int			GetFlagAnnessi(void);

/* Per settare e recuperare il valore del flag che indica se il formato originario è UTF-8 */
void		SetFlagUTF8(int flagutf);
int			GetFlagUTF8(void);

char		*utilCreaBuffer(int size);
char		*utilGetPartialText(char *ptesto,int pindexstart, int plen);
int			IsNode(xmlNodePtr pNodo,tagTipo ptipo);
void		MoveAllChildren(xmlNodePtr pFrom,xmlNodePtr pTo);
xmlNodePtr	GetFirstTextNode(xmlNodePtr pNodoParent);
xmlNodePtr	GetFirstNodebyTagTipo(xmlNodePtr pNodoParent, xmlChar *pnomeTag);
xmlNodePtr	totalGetFirstNodebyTagTipo(xmlNodePtr pNodoParent, xmlChar *pnomeTag, xmlNodePtr *savebuf, int savesize);
void		addNodeInBuffer(xmlNodePtr *buffer, xmlNodePtr node, int *size);
void		adjustBuffer(xmlNodePtr *buffer, int *size);
int			GetAllNodebyTagTipo(xmlNodePtr *bufdest, int size, xmlNodePtr pNodoParent , xmlChar *pnomeTag);
void		utilErrore2ProcessingInstruction(xmlNodePtr pNodoParent);
void		InsertXmlFile(char *memdoc,xmlNodePtr pParentNode);
//---
//int convLetteraToInt(char *s);
int convNTLetteraToInt(char *s);
//char *convIntToLettera(int num);
char *getLettera(char *lettera_ptr);

char * cercaSpazio(char *s);

void sostStr(char *stringa, char *sold, char *snew);

int contaChar(char *s, char);

//void sostChr(char *s, char c1, char c2);
//char * sostStr(char *s, char *s1, char *s2);

int latinToArabic(char *s);
char * arabicToLatin(int n);

#ifdef __cplusplus
}
#endif

#endif /* __UTIL_H__ */
