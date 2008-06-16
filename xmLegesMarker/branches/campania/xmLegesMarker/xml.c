/******************************************************************************
* Project:	xmLeges
* Module:	Marker
* File:		xml.c
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Lorenzo Bacci (bacci@ittig.cnr.it)
******************************************************************************/

/*
 * 1 - prexml: lasciare le entità non risolte, oppure risolverle tutte "manualmente"?
 * 2 - creazione del doc xml e attributi nir
 * 3 - separa il buffer: una parte è XML con l'header, l'altra è un buffer
 * senza tag con tutto il resto del documento
 * 4 - xml: mapping dei campi del db con i nodi nir SOLO PER L'HEADER
 * 5 - prosegui con il marker solo sul buffer contenente il resto del documento
 * 6 - struttura: non aggiungere di nuovo i nodi dell'header già presenti 
 * 7 - eventuale header degli allegati?
 * 
 * Note:
 * Meglio sostituire "Articolato" con "Corpo" e comprendere anche tutti gli annessi,
 * poi inserire a mano la parola ALLEGATO per separarli?
 */

/**
 * section: xmlReader
 * synopsis: Parse an XML file with an xmlReader
 * purpose: Demonstrate the use of xmlReaderForFile() to parse an XML file
 *          and dump the informations about the nodes found in the process.
 *          (Note that the XMLReader functions require libxml2 version later
 *          than 2.6.)
 * usage: reader1 <filename>
 * test: reader1 test2.xml > reader1.tmp ; diff reader1.tmp reader1.res ; rm reader1.tmp
 * author: Daniel Veillard
 * copy: see Copyright for the status of this software.
 */

#include <stdio.h>
#include <libxml/xmlreader.h>
#include "xml.h"
#include <string.h>
#include <string>

static char *	body = NULL;
static xmlNodePtr root = NULL;
static xmlNodePtr nMeta = NULL;
static xmlNodePtr nIntestazione = NULL;
static xmlNodePtr nFormulaIniziale = NULL;
static xmlNodePtr nDescrittori = NULL;
static xmlNodePtr nPubblicazione = NULL;
static xmlNodePtr nRedazione = NULL;
static xmlNodePtr nEntrata = NULL;

static void
parsePair(char * name, const xmlChar * value) {
	
	if( !strcmp(name, "REGIONE")) {
		if(	!xmlStrcmp(value, (const xmlChar *) "CM")) {
			xmlNewChild(nIntestazione, NULL, BAD_CAST "emanante", BAD_CAST "Regione Campania");
		} else {
			printf("\nparsePair() PAIR NOT FOUND! name: %s value: %s \n", name, value);
		}
	}
	if( !strcmp(name, "DATA")) {
		char *strData = getData((char *) value);
		xmlNodePtr d = xmlNewChild(nIntestazione, NULL, BAD_CAST "dataDoc", BAD_CAST strData);
		xmlNewProp(d, BAD_CAST "norm", BAD_CAST value);
	}
	if( !strcmp(name, "NUMERO")) {
		xmlNodePtr nText = xmlNewText(BAD_CAST ", n. ");
		xmlAddChild(nIntestazione, nText);
		xmlNewChild(nIntestazione, NULL, BAD_CAST "numDoc", value);
	}
	if( !strcmp(name, "TITOLO_ATTO")) {
		xmlNewChild(nIntestazione, NULL, BAD_CAST "titoloDoc", value);
	}
	if( !strcmp(name, "TIPO_LEGGE")) {
		if(	!xmlStrcmp(value, (const xmlChar *) "LR")) {
			xmlNewChild(nIntestazione, NULL, BAD_CAST "tipoDoc", BAD_CAST "Legge Regionale");
			xmlNodePtr nText = xmlNewText(BAD_CAST " ");
			xmlAddChild(nIntestazione, nText);
		} else {
			printf("\nparsePair() PAIR NOT FOUND! name: %s value: %s \n", name, value);
		}				
	}
	if( !strcmp(name, "PREMESSA")) {
		//xmlNodePtr pre = xmlNewChild(nFormulaIniziale, NULL, BAD_CAST "formulainiziale", NULL);
		//xmlNewChild(pre, NULL, BAD_CAST "h:p", value);
		
		setPremessa2(value);
	}
	if( !strcmp(name, "ANNO")) {	
	}
	if( !strcmp(name, "D_BOLL")) {
		xmlNewProp(nPubblicazione, BAD_CAST "norm", BAD_CAST value);
	}
	if( !strcmp(name, "N_BOLL")) {
		xmlNewProp(nPubblicazione, BAD_CAST "num", BAD_CAST value);	
	}
	if( !strcmp(name, "TIPO_BOLL")) {
		//Valore relativo a Regione Campania in caso di sorgente XML !!
		if(	!xmlStrcmp(value, (const xmlChar *) "BR")) {
			xmlNewProp(nPubblicazione, BAD_CAST "tipo", BAD_CAST "BURC");
		} else {
			printf("\nparsePair() PAIR NOT FOUND! name: %s value: %s \n", name, value);
		}
	}
	if( !strcmp(name, "ANNO_BOLL")) {
	}
	if( !strcmp(name, "ANNO_SUPPL")) {
	}
	if( !strcmp(name, "TIPO_SUPP")) {
	}
	if( !strcmp(name, "N_SUPP")) {
	}
	if( !strcmp(name, "CODICE_REGIONE")) {
	}
	if( !strcmp(name, "ARTICOLATO")) {
    	//THIS DOES THE TRICK !!
    	xmlChar * xValue = xmlEncodeEntitiesReentrant(NULL, value);				
    	char *strvalue = (char *) xValue;
    	size_t size = strlen(strvalue) + 1;
    	body = (char *) malloc(sizeof(char) * size);
    	if(body == NULL) {
			printf("\n>>ERROR<< malloc() could not allocate memory!\n");
			return;
    	}
    	sprintf(body, "%s", strvalue);
	}	
}

static void getTagContent(const char *buffer, char *thisName) {
    xmlTextReaderPtr reader;
    int ret;

    reader = xmlReaderForMemory(buffer, strlen(buffer), NULL, NULL, 0);
    if (reader != NULL) {
        ret = xmlTextReaderRead(reader);
        int nameFound = 0;

        while (ret == 1) {
            const xmlChar *name, *value;		    
		    name = xmlTextReaderConstName(reader);
		    value = xmlTextReaderConstValue(reader);
		    		    
		    if (name == NULL) {
				name = BAD_CAST "--";
		    }
		    
		   	if(nameFound == 1) {
		   		parsePair(thisName, value);
		   		return;
		   	}

		    if( !strcmp((char *) name, thisName) ) {
		    	nameFound = 1;
		    }

		    ret = xmlTextReaderRead(reader);
        }
        xmlFreeTextReader(reader);
    }
}

static void
streamHeader(const char *buffer) {
	
	//getTagContent(buffer, "ANNO");
	getTagContent(buffer, "REGIONE");
	getTagContent(buffer, "TIPO_LEGGE");
	getTagContent(buffer, "DATA");
	getTagContent(buffer, "NUMERO");
	getTagContent(buffer, "TITOLO_ATTO");
	getTagContent(buffer, "PREMESSA");
	getTagContent(buffer, "CODICE_REGIONE");
	getTagContent(buffer, "D_BOLL");
	getTagContent(buffer, "N_BOLL");
	getTagContent(buffer, "TIPO_BOLL");
	//getTagContent(buffer, "ANNO_BOLL");
	//getTagContent(buffer, "D_SUPP");
	//getTagContent(buffer, "N_SUPP");
	//getTagContent(buffer, "TIPO_SUPP");
	//getTagContent(buffer, "ANNO_SUPPL");

}

void xmlAnalyzeHeader(char *buffer, xmlNodePtr pRootNode) {

	//init
	root = pRootNode;
	nMeta = xmlNewNode(NULL, BAD_CAST "meta");
	nIntestazione = xmlNewNode(NULL, BAD_CAST "intestazione");
	nFormulaIniziale = xmlNewNode(NULL, BAD_CAST "formulainiziale");
	nDescrittori = xmlNewChild(nMeta, NULL, BAD_CAST "descrittori", NULL);	
	nPubblicazione = xmlNewChild(nDescrittori, NULL, BAD_CAST "pubblicazione", NULL);
	nEntrata = xmlNewChild(nDescrittori, NULL, BAD_CAST "entratainvigore", NULL);
	xmlNewProp(nEntrata, BAD_CAST "norm", BAD_CAST "");
	nRedazione = xmlNewChild(nDescrittori, NULL, BAD_CAST "redazione", NULL);
	xmlNewProp(nRedazione, BAD_CAST "nome", BAD_CAST "Regione Campania");
	xmlNewProp(nRedazione, BAD_CAST "id", BAD_CAST "red1");
	xmlNewProp(nRedazione, BAD_CAST "norm", BAD_CAST "20080501");
	xmlNewProp(nRedazione, BAD_CAST "url", BAD_CAST "http://www.regione.campania.it");
	
	printf("\nxmlAnalyzeHeader() buffer size: %d\n", strlen(buffer));
	
    streamHeader(buffer);

    /*
     * Cleanup function for the XML library.
     */
    //xmlCleanupParser(); ??
    /*
     * this is to debug memory for regression tests
     */
    //xmlMemoryDump(); ??
    //return(0);
    
}

char * xmlGetBody(char *buffer) {
	
 	//streamBody(buffer);
 	printf("\nxmlGetBody() buffer size: %d\n", strlen(buffer));
 	getTagContent(buffer, "ARTICOLATO");
	return body;
}

void xmlAdjustHeader(xmlNodePtr parentNode) {
	//Complete header...
	xmlAddChild(parentNode, nMeta);
	xmlAddChild(parentNode, nIntestazione);
	xmlAddChild(parentNode, nFormulaIniziale);
}

xmlNodePtr xmlGetMeta() {
	
	return nMeta;
}

xmlNodePtr xmlGetIntestazione() {
	
	return nIntestazione;
}

xmlNodePtr xmlGetDescrittori() {
	
	return nDescrittori;
}

xmlNodePtr xmlGetFormulaIniziale() {
	
	return nFormulaIniziale;
}

void setPremessa(const xmlChar * value) {
	/*
	 * Implementazione C string...
	 * cervellotica e ovviamente bacata!!
	 */
	
	char *splitter = ".\n";
	char *prevSplit = (char *) value;
	char *split = strstr((char *) value, splitter);
	
	int splitterLen = strlen(splitter);
	int prevLen = strlen(prevSplit);
	int splitLen = 0;
	int lineLen = 0;
	
	if(split != NULL) {
		char *newSplit = NULL;
		
		while(split != NULL) {

			splitLen = strlen(split);
			printf("\nSIZE: %d SPLIT:%s\n", splitLen, split); 
			if(splitLen == splitterLen) {
				//skip last empty match
				printf("\nSkipping last empty match...\n");
				break;
			}
			
			lineLen = prevLen - splitLen;
			//Prendi anche il "."

			int offset = 0;				
			if(!strncmp(prevSplit, splitter, splitterLen)) {
				offset = splitterLen;
			}				
			char *actualLine = (char *)malloc(sizeof(char) * lineLen + 2 - offset);
			int i;
			for(i = 0; i < lineLen + 2 - offset; i++) {
				actualLine[i] = prevSplit[i+offset];
			}
			actualLine[lineLen + 2 - offset] = '\0';
			
			//strncpy(actualLine, prevSplit, lineLen + 1);
			printf("\nSIZE: %d ActualLine:%s\n", lineLen + 1, actualLine);
			
			xmlNewChild(nFormulaIniziale, NULL, BAD_CAST "h:p", BAD_CAST actualLine);
							
			prevSplit = split;
			//Set new split: skip ".\n"
			newSplit = (char *)malloc(sizeof(char) * splitLen - splitterLen + 1);
			
			for(i = 0; i < splitLen - splitterLen; i++) {
				newSplit[i] = split[i+splitterLen];
			}
			printf("\nSIZE: %d NewSplit:%s\n", splitLen - splitterLen, newSplit); 
			
			prevLen = strlen(split);
			split = strstr((char *) newSplit, splitter);
		}
		
		if(newSplit != NULL) {
			int newSplitLen = strlen(newSplit);
			if(newSplitLen > 0) {
				xmlNewChild(nFormulaIniziale, NULL, BAD_CAST "h:p", BAD_CAST newSplit);				
			}
		}
					
	} else {		
		xmlNewChild(nFormulaIniziale, NULL, BAD_CAST "h:p", value);
	}	
}

void setPremessa2(const xmlChar * value) {
	/*
	 * Implementazione con std::string
	 */
	
	std::string premessa = (char *) value;
	unsigned int index = 0;
	
	//printf("\nsetPremessa - size: %d - premessa:\n%s\n", premessa.size(), premessa.c_str());
	
	while(index < premessa.size() - 1 ) {
		
		std::string item;				
		
		std::string::size_type splitter = premessa.find(".\n", index);
		
		if(splitter != std::string::npos) {
			
			item = premessa.substr(index, splitter - index + 1);
			index = splitter + 2;
			//printf("\nRilevato splitting - splitter: %d - index: %d - item: %s\n", splitter, index, item.c_str());
					
		} else {
			item = premessa.substr(index);
			index = premessa.size() + 2; //last loop execution
			//printf("\nRilevato fine premessa - index: %d - item: %s\n", index, item.c_str());
		}
		
		//Aggiungi la premessa
		//printf("\nAdding premessa item: %s\n", item.c_str());
		xmlNewChild(nFormulaIniziale, NULL, BAD_CAST "h:p", BAD_CAST item.c_str());		
	}	
	
}

char * getData(char *value) {
	
	char *anno = (char *) malloc(sizeof(char) * 5);
	char *mese = (char *) malloc(sizeof(char) * 3);
	char *gg = (char *) malloc(sizeof(char) * 3);
	
	char *data = (char *) malloc(sizeof(char) * 81);
	
	unsigned int i = 0;
	for(i = 0; i < strlen(value); i++) {
		if(i < 4) {
			anno[i] = value[i];
		} else if(i < 6) {
			mese[i-4] = value[i];
		} else if(i < 8) {
			gg[i-6] = value[i];
		}
	}
	
	sprintf(data,"%s %s %s", gg, getMeseEsteso(mese), anno);
	return data;
}

char * getMeseEsteso(char *mese) {
	
	if(!strcmp(mese, "01")) {
		return "gennaio";
	} else if(!strcmp(mese, "02")) {
		return "febbraio";
	} else if(!strcmp(mese, "03")) {
		return "marzo";
	} else if(!strcmp(mese, "04")) {
		return "aprile";
	} else if(!strcmp(mese, "05")) {
		return "maggio";
	} else if(!strcmp(mese, "06")) {
		return "giugno";
	} else if(!strcmp(mese, "07")) {
		return "luglio";
	} else if(!strcmp(mese, "08")) {
		return "agosto";
	} else if(!strcmp(mese, "09")) {
		return "settembre";
	} else if(!strcmp(mese, "10")) {
		return "ottobre";
	} else if(!strcmp(mese, "11")) {
		return "novembre";
	} else if(!strcmp(mese, "12")) {
		return "dicembre";
	}
	printf("\nERROR getMeseEsteso() - mese: %s\n", mese);		
	return mese;
}

