/******************************************************************************
* Project:	xmLeges
* Module:	Marker
* File:		disposto.c
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Mirco Taddei (m.taddei@ittig.cnr.it)
******************************************************************************/
#include "disposto.h"
#include "util.h"
#include <IttigLogger.h>

static char *buffer;
static int pos, size;
static int inizio;


/******************************************************************************/
/***************************************************************** DIS INIT ***/
/******************************************************************************/
void disInit(dim) {
	//buffer=(char *)malloc(sizeof(char)*dim);
	buffer = malloc(dim * sizeof(char));
	if(buffer==NULL)
		printf("\n>>ERROR<< disInit() - malloc() could not allocate memory!\n");
	
	*buffer=0;
	size=dim;
	pos=0;
	inizio=0;
}


/******************************************************************************/
/************************************************************* DIS ANALIZZA ***/
/******************************************************************************/
// Restituisce:
// - in buffer: il disposto (se trovato)
// - in testo : la parte precedente il disposto
// in entrambi: converto in paragrafi
void dispostoAnalizza(ruoloDoc ruolo, char *testo, char** disposto, char** prima, int dim) {

	disInit(dim);

	loggerInfo("INIZIO Disposto");
	disAppendString("<h:p>\n");		// apro tag
	
	dis_scan_string(testo);
	dislex();

	if (inizio)
		loggerDebug("Disposto Trovato");
	else	
		loggerDebug("Disposto NON Trovato");

	disAppendString("</h:p>\n");		// chiudo tag
	*(buffer+pos)=0;			// chiudo stringa
	if (ruolo == principale && inizio)	// spezzo disposto
		{
		*prima=strdup(buffer);
		*(*prima+inizio)=0;
		buffer+=inizio;		// elimino prima da disposto
		*disposto=strdup(buffer);
		}
	else
		{
		*prima=strdup(buffer);
		*disposto=NULL;
		}
	
	loggerInfo("FINE Disposto");
}


/******************************************************************************/
/***************************************************************** DIS SAVE ***/
/******************************************************************************/
void disSaveIni(void) 
{
	inizio = pos;
}


/******************************************************************************/
/******************************************************** DIS APPEND STRING ***/
/******************************************************************************/
void disAppendString(char *s) {
	int len = strlen(s);
	if (len + pos <= size) {
		memcpy(buffer + pos, s, len);
		pos += len;
	}
}


/******************************************************************************/
/********************************************************* DIS APPEND CHARS ***/
/******************************************************************************/
void disAppendChars(int n, int c) {
	register int i;
	if (n + pos <= size) {
		for (i = 0; i < n; i++)
			*(buffer + pos + i) = c;
		pos += n;
	}
}
