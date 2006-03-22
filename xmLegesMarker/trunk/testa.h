#ifndef TESTA_H
#define TESTA_H


#include <libxml/tree.h>
//const char *testa(char *testo, int * p_notes);

 int testa(xmlNodePtr pTextNode, xmlNodePtr ptipodoc, xmlNodePtr pmeta,
		  xmlNodePtr pdescrittori,
		  xmlNodePtr pintestazione,
		  xmlNodePtr pformulainiziale,
		  int tdoc) ;


#endif

