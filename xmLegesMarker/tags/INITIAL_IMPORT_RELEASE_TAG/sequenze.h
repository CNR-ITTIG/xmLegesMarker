#ifndef __SEQUENZE_H__
#define __SEQUENZE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "tag.h"

void sequenzeClear(void);
void sequenzaInit(tagTipo tipo);
int sequenzaCheck(tagTipo tipo, int num, int lat);
tagNum * sequenzaValore(tagTipo tipo);
void sequenzaInc(tagTipo tipo);
void sequenzaSet(tagTipo tipo,int num);
int sequenzaGetNum(tagTipo tipo);
int sequenzaGetLat(tagTipo tipo);
#ifdef __cplusplus
}
#endif

#endif
