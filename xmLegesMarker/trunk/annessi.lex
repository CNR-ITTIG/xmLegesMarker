%{
// Scanner Case-Insensitive
// compilare con flex -i -8 -Pall nomefile
#include <stdio.h>
#include <string.h>
#include "tag.h"
#include <IttigLogger.h>
#include "xxx.h"
#include "config.h"
#define __DEBUG__  0

long EndOfLastAllegato=0;
long StartOfLastAllegato=0;
long ann_pos=0;
int firstAnnesso=0;
int LastIDAnnesso=0;
int disegno=0;
int testate=0;

void annIncPos(void){
	ann_pos+=annleng;
}

void salva(void)
{
	LastIDAnnesso++;

	if (firstAnnesso)
	{
		xxxTagOpen(tagerrore,0,0);
		//xxxTagOpen(annessi,StartOfLastAllegato,0);
		firstAnnesso=0;
	}

	xxxTagOpen(annesso,StartOfLastAllegato,0);
	xxxSetID(annesso,LastIDAnnesso,-1);
	xxxTagOpen(testata,StartOfLastAllegato,0);
	xxxTagOpen(denannesso,StartOfLastAllegato,0);
	xxxTagOpen(h_p,StartOfLastAllegato,0);
	xxxTagOpen(titannesso,(ann_pos+annleng),0);
	xxxTagOpen(h_p,(ann_pos+annleng),0);
	xxxTagOpen(preannesso,(ann_pos+annleng),0);
	xxxTagOpen(h_p,(ann_pos+annleng),0);

	xxxTagOpen(tagerrore,(ann_pos+annleng),0);
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
TABELLA		(t{S}*a{S}*b{S}*e{S}*l{S}*l{S}*a)
ANNESSO		(a{S}*n{S}*n{S}*e{S}*s{S}*s{S}*o)
ALL		(a{S}*l{S}*l{S}*e{S}*g{S}*a{S}*t{S}*o)
SUBALL		(s{S}*u{S}*b{S}*{ALL})
ALLEGATO	({ALL}|{SUBALL}|{ANNESSO}|{TABELLA})
DISEGNO		((disegno|progetto|proposta){S}+di{S}+legge)

NUM		([0-9]+)
LAT09		(un|bis|duo|ter|quater|quinquies|sexies|septies|octies|novies)
LAT10		(decies|venies)
LATINO		([- ]?{LAT09}({LAT10})?)
ORDNOSEX	(prim|second|terz|quart|quint|sest|settim|ottav|non|decim)
ORD		({ORDNOSEX}[oa])
ROMANO		([ivxl]+)
LETTERE		([a-z])

TUTTINUMERI	({NUM}|{ROMANO}|{LATINO}|{ORD})

%s InCorpoAllegato
%s InTestaAllegato

%%

^({S}*{DISEGNO}{S}*)$	{   // disegno di legge
						if(configGetDocTestoTipo() != disegnolegge)
							REJECT;
						disegno++;
						annIncPos();
					}

^({S}*{ALLEGATO}{S}*)	{   // NL c'è per forza alla fine???
						
						if(configGetDocTestoTipo() == disegnolegge && disegno < testate)
							REJECT;
						BEGIN(InTestaAllegato);
						MaybeAllegato();
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

