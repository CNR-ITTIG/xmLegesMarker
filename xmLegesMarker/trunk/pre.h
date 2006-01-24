#ifndef PRE_H
#define PRE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <malloc.h>
#include <string.h>

char *preAnalizza(char *testo);
void preAppendString(char *s);
void preAppendChars(int n, int c);

#ifdef __cplusplus
}
#endif

#endif
