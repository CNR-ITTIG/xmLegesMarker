#ifndef PREHTML_H
#define PREHTML_H

#ifdef __cplusplus
extern "C" {
#endif

void prehtmlAppendString(char *s);
void prehtmlAppendChars(int n, int c) ;
char *prehtmlAnalizza(char *testo);
char prehtmlExtractChar(int elemento);
//char *utilIsolaNum(char *s);


#ifdef __cplusplus
}
#endif

#endif /* PREHTML_H */
