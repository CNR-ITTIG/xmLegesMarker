/******************************************************************************
* Project:	xmLeges
* Module:	Marker
* File:		sequenze.c
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Mirco Taddei (m.taddei@ittig.cnr.it)
******************************************************************************/
#include <math.h>
#include <stdio.h>

#include "config.h"
#include "tag.h"
#include "util.h"

#include <IttigUtil.h>
#include <IttigLogger.h>

tagNum seq[TAGTIPODIM];
int sequenzaGetNum(tagTipo tipo)
{
	return seq[tipo].num;
}
int sequenzaGetLat(tagTipo tipo)
{
	return seq[tipo].lat;
}


/******************************************************************************/
/******************************************************** SEQUENZA INC ********/
/******************************************************************************/
void sequenzaInc(tagTipo tipo) {
	seq[tipo].num++;	
}

/******************************************************************************/
/******************************************************** SEQUENZA STRINGA ****/
/******************************************************************************/
char *sequenzaStringa(tagTipo tipo) {
	return utilConcatena(4, "sequenza:", utilItoa(seq[tipo].num), "." , utilItoa(seq[tipo].lat));
}



/******************************************************************************/
/*********************************************************** SEQUENZA INIT ****/
/******************************************************************************/
void sequenzaInit(tagTipo tipo) {
	seq[tipo].num = 0;
	seq[tipo].lat = 0;
}

void sequenzaSet(tagTipo tipo,int num){
	seq[tipo].num = num;
	seq[tipo].lat = 0;
}

/******************************************************************************/
/********************************************************** SEQUENZA CHECK ****/
/******************************************************************************/
int sequenzaCheck(tagTipo tipo, int num, int lat) {
int ii;
	if (!configSequenzaCheck()) {
		loggerDebug("sequenzaCheck disabilitato");
		seq[tipo].num = num;
		seq[tipo].lat = lat;
		return 1;
	}
	loggerDebug(utilConcatena(8, "sequenzaCheck(tipo,num,lat):arg=", utilItoa(num), ",", utilItoa(lat), ",seq=", utilItoa(seq[tipo].num), ",", utilItoa(seq[tipo].lat)));
		// ??? RIVEDERE CON CONDIZIONI PER LATINI

	//printf("sequenza numero %d\n",seq[tipo].num);
	//printf("num %d\n",num);

	ii = (num/27);

		if (tipo == lettera && seq[tipo].num == (9+ii*26) && num == (12+ii*26)) {
			loggerInfo("SALTO DI SEQUENZA PER LETTERE J e K");
			seq[tipo].num = num;
			seq[tipo].lat = lat;
			return 1;
		}

		//Inserito
		if (tipo == lettera && seq[tipo].num == (22+ii*26) && num == (26+ii*26)) {
			loggerInfo("SALTO DI SEQUENZA PER LETTERE W X e Y");
			seq[tipo].num = num;
			seq[tipo].lat = lat;
			return 1;
		}
	

	if (seq[tipo].num + 1 == num) {
		seq[tipo].num = num;
		seq[tipo].lat = 0;
		return 1;
	}
	if (seq[tipo].num == num && (seq[tipo].lat + 1 == lat || (seq[tipo].lat == 0 && lat == 2))) {
		seq[tipo].lat = lat;
		return 1;
	}
	return 0;
}



/******************************************************************************/
/********************************************************* SEQUENZA VALORE ****/
/******************************************************************************/
tagNum * sequenzaValore(tagTipo tipo) {
	return &(seq[tipo]);
}



/******************************************************************************/
/********************************************************** SEQUENZA CLEAR ****/
/******************************************************************************/
void sequenzeClear(void){
	register int n;
	for (n=0;n<TAGTIPODIM;n++)
	{
		sequenzaInit(n);
	}
}
