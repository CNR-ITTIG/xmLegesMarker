#ifndef XML_H_
#define XML_H_

#endif /*XML_H_*/

void xmlAnalyzeFile(char *filename);

void xmlAnalyzeHeader(char *buffer, xmlNodePtr pRootNode);

char * xmlGetBody(char *buffer);

void xmlAdjustHeader(xmlNodePtr parentNode);

xmlNodePtr xmlGetMeta();

xmlNodePtr xmlGetIntestazione();

xmlNodePtr xmlGetDescrittori();

xmlNodePtr xmlGetFormulaIniziale();

char * getData(char *value);

char * getMeseEsteso(char *mese);

void setPremessa(const xmlChar * value);
void setPremessa2(const xmlChar * value);
