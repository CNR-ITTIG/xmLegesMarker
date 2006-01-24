#ifndef ANNESSI_H
#define ANNESSI_H

#include "costanti.h"
#include "struttura.h"
#include "xxx.h"
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

