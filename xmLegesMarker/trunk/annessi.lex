/******************************************************************************
* Project:	xmLeges
* Module:	Marker
* File:		annessi.lex
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Mirco Taddei (m.taddei@ittig.cnr.it)
******************************************************************************/

/*
N	(n.|num.)
TUTTINUMERI	({S}*{N}*{S}*{NUM}|{ROMANO}|{LATINO}|{ORD})

orig:
TUTTINUMERI	({NUM}|{ROMANO}|{LATINO}|{ORD})
*/

%{
// Scanner Case-Insensitive
// compilare con flex -i -8 -Pall nomefile
#include <stdio.h>
#include <string.h>
#include "tag.h"
#include <IttigLogger.h>
#include "dom.h"
#include "config.h"
#define __DEBUG__  0

long EndOfLastAllegato=0;
long StartOfLastAllegato=0;
long ann_pos=0;
int firstAnnesso=0;
int LastIDAnnesso=0;
int disegno=0;
int testate=0;
int decretoDDL=0; //indica il caso decreto di legge allegato a un DDL

void annIncPos(void){
	ann_pos+=annleng;
}

void salva(void)
{
	LastIDAnnesso++;

	if (firstAnnesso)
	{
		domTagOpen(tagerrore,0,0);
		//domTagOpen(annessi,StartOfLastAllegato,0);
		firstAnnesso=0;
	}

	domTagOpen(annesso,StartOfLastAllegato,0);
	domSetID(annesso,LastIDAnnesso,-1);
	domTagOpen(testata,StartOfLastAllegato,0);
	domTagOpen(denannesso,StartOfLastAllegato,0);
	domTagOpen(h_p,StartOfLastAllegato,0);
	domTagOpen(titannesso,(ann_pos+annleng),0);
	domTagOpen(h_p,(ann_pos+annleng),0);
	domTagOpen(preannesso,(ann_pos+annleng),0);
	domTagOpen(h_p,(ann_pos+annleng),0);

	if(decretoDDL)
		domTagOpen(tagerrore,ann_pos,0);
	else
		domTagOpen(tagerrore,(ann_pos+annleng),0);
	loggerDebug("Allegato Trovato");
}
	
void MaybeAllegato(){
	StartOfLastAllegato=ann_pos;
}

void MaybeEndOfAllegato(){
	EndOfLastAllegato=ann_pos;
}


%}

S	([ ])
NL	(\n)
FR	({S}*{NL})
TABELLA		(t{S}*a{S}*b{S}*e{S}*l{S}*l{S}*a)
ANNESSO		(a{S}*n{S}*n{S}*e{S}*s{S}*s{S}*o)
ALL		(a{S}*l{S}*l{S}*e{S}*g{S}*a{S}*t{S}*o)
SUBALL		(s{S}*u{S}*b{S}*{ALL})
ALLEGATO	({ALL}|{SUBALL}|{ANNESSO}|{TABELLA})
DISEGNO		((disegno|progetto|proposta){S}+di{S}+legge)
DECRETO		({NL}{S}*decreto[ \-]legge)

NUM		([0-9]+)
LAT09		(un|bis|duo|ter|quater|quinquies|sexies|septies|octies|novies)
LAT10		(decies|venies)
LATINO		([- ]?{LAT09}({LAT10})?)
ORDNOSEX	(prim|second|terz|quart|quint|sest|settim|ottav|non|decim)
ORD		({ORDNOSEX}[oa])
ROMANO		([ivxl]+)
LETTERE		([a-z])

N	(n.|num.)
TUTTINUMERI	({S}*{N}*{S}*{NUM}|{ROMANO}|{LATINO}|{ORD})

%s InCorpoAllegato
%s InTestaAllegato

%%

^({S}*{DISEGNO}{S}*)$	{   // disegno di legge
						if(configGetDocTestoTipo() != disegnolegge)
							REJECT;
						//printf("\nDisegno:%d\n",disegno);
						disegno++;
						annIncPos();
					}

^({S}*{ALLEGATO}{S}*)$	{   //"ALLEGATO" (con new line)
						
						if(configGetDocTestoTipo() == disegnolegge && disegno < testate)
							REJECT;
						BEGIN(0);
						MaybeAllegato();
						salva();
						annIncPos();
					}

^({S}*{ALLEGATO}{S}*)	{   // NL c'è per forza alla fine???
						
						if(configGetDocTestoTipo() == disegnolegge && disegno < testate)
							REJECT;
						BEGIN(InTestaAllegato);
						MaybeAllegato();
						annIncPos();
					}

^({S}*{DECRETO}{S}*)	{   // DECRETO-LEGGE alla fine di un Disegno di Legge
						if(configGetDocTestoTipo() != disegnolegge)
							REJECT;
						if(disegno < testate)
							REJECT;
						MaybeAllegato();
						decretoDDL=1;
						salva();
						decretoDDL=0;
						annIncPos();
					}
							
<InTestaAllegato>{TUTTINUMERI}/([:)]?{S}*{NL})	{	//Allegato Numerico|Romano|Latino|Ordinale . Qui anche NL+?
						salva();
						annIncPos();
					}

<InTestaAllegato>{LETTERE}/([:)]?{S}*{NL})	{	//Allegato CARATTERE . Qui anche NL+?
						salva();
						annIncPos();
					}

<InTestaAllegato>{NL}			{ 
						BEGIN(0);
						MaybeEndOfAllegato();
						annIncPos();
					}

<InCorpoAllegato>{NL}			{
						MaybeEndOfAllegato();
						annIncPos();
					}

<InCorpoAllegato><<EOF>>		{

						//if (EndOfLastAllegato!=0)salva();
						BEGIN(0);
					}

[a-z0-9]+		{ annIncPos(); BEGIN(0); }
.|{NL}			{ annIncPos(); BEGIN(0); }

%%

int annwrap()
{
	return 1;
}

//Restituisce il numero di annessi trovati
int  _annessiLexStart(char *testo)
{
	testate=configDdlTestate();
	//printf("\nTestate:%d\n", testate);
	BEGIN(0);
	yy_init = 1;
	ann_pos=0;
	LastIDAnnesso=0;
	EndOfLastAllegato=0;
	StartOfLastAllegato=0;
	firstAnnesso=1;
	ann_scan_string(testo);
	annlex();
	
	return LastIDAnnesso;
}

//void ann_inizio_scansione(FILE* file_desc)
//{
//	BEGIN(0);
//	yy_init = 1;
//	ann_pos=0;
//	EndOfLastAllegato=0;
//	StartOfLastAllegato=0;
//	annrestart(file_desc);
//	annin = file_desc;
//	annout=fopen("ppp.txt","w");
//	annlex();
//}

