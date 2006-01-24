#ifndef VIRGOLETTE_H
#define VIRGOLETTE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include "xxx.h"
#include "tag.h"

#include <libxml/tree.h>

void virgoletteInMod(xmlNodePtr pParentNode);
int checkCommaMod(char *txt);

#ifdef __cplusplus
}
#endif

#endif
