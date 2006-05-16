/******************************************************************************
* Project:	xmLeges
* Module:	Marker
* File:		articolato.lex
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Mirco Taddei (m.taddei@ittig.cnr.it)
* 			Lorenzo Bacci (lorenzobacci@gmail.com)
******************************************************************************/

/*
PTACAPO		(([.]{S}*{DECORAZ}*{FR})|({FR}{FR}))  // <-- vecchio PTACAPO
*/


%{
#include <string.h>
#include <IttigUtil.h>
#include <IttigLogger.h>

#include "config.h"

#include "sequenze.h"
#include "articolato.h"
#include "tag.h"
#include "util.h"
#include "dom.h"

//tag *tagTmp = NULL;
long artpos = 0;
static int numConv;
static int latConv;
char *current_lettera = NULL;
int nvir2ap = 1;
int commiNN = 1;
int firstSave;
int isArticolato;
int co,cr;
int tipoRubriche = 1;
int numdis=0;
int numtes=0;
int art_dec=0;

/******************************************************************* CHECK ****/
int check(tagTipo tipo) {
	int seq;
	char *tipoStr = strdup(tagTipoToNome(tipo));
	loggerDebug(utilConcatena(9, "CHECK: ", tipoStr, " testo=\"", yytext, "\", num=", utilItoa(numConv), "lat=", utilItoa(latConv)));
	seq = sequenzaCheck(tipo, numConv, latConv);
	if (!seq) {
		loggerWarn(utilConcatena(5, "CHECK:", tipoStr, " non in sequenza \"", yytext, "\""));
		//Aggiungi un nodo/messaggio di warning?
		domAddSequenceWarning(tipo);
	}
	return seq;
}

/************************************************************ CHECK ROMANO ****/
int checkOrd(tagTipo tipo) {
	numConv = utilOrdToArabic(cercaSpazio(utilCercaCarattere(yytext)));
	latConv = 0;//latinToArabic(yytext);
	return check(tipo);
}

/************************************************************ CHECK ROMANO ****/
int checkRomano(tagTipo tipo) {
	numConv = utilRomanToArabic(cercaSpazio(utilCercaCarattere(yytext)));
	latConv = latinToArabic(yytext);
	return check(tipo);
}

/********************************************************* CHECK CARDINALE ****/
int checkCardinale(tagTipo tipo) {
	numConv = atoi(utilCercaCifra(yytext));
	latConv = latinToArabic(yytext);
	return check(tipo);
}

/*********************************************************** CHECK LETTERA ****/
int checkLettera(tagTipo tipo) {
	//numConv = convLetteraToInt((char*)utilCercaCarattere(yytext));
	current_lettera = getLettera((char*)utilCercaCarattere(yytext));
	numConv = convNTLetteraToInt(current_lettera);
	latConv = latinToArabic(yytext);
	return check(tipo);
}

/******************************************************************** SAVE ****/
void save(tagTipo tipo) {
	
	register int i;
	
	//Se vi è un articolo, il documento è articolato
	if (tipo==articolo)	isArticolato=1;
	
	if (firstSave)
	{
		domTagOpen(tagerrore,0,0);
		firstSave=0;
	}

		domTagOpen(tipo,artpos,0);
		if(tipo==lettera) {
			//printf("\nLETTERA num:%d lat:%d current_lettera:%s\n",numConv,latConv,current_lettera);
			domSetIDLettera(current_lettera,latConv);
		} else
			domSetID(tipo,numConv,latConv);
		domTagOpen(num,artpos,artleng);

	if (tipo <= articolo)
	{
		if (configGetRubriche()!=0)
			domTagOpen(rubrica,artpos+artleng,0);
	}
	else
		domTagOpen(corpo,artpos+artleng,0);
		
	for (i=tipo+1; i<=numero; i++)
		if (i != articolo)sequenzaInit(i);
	
	loggerDebug(utilConcatena(3, tagTipoToNome(tipo), ": ->", yytext));
	
	artpos+=artleng;
	
}
/*
void save(tagTipo tipo) {

	register int i;

	if (firstSave)
	{
		firstSave=0;
		tagAppendi(tagInit(articolato, pos, -1, -1, -1, -1));
	}
	tagAppendi(tagInit(tipo, artpos, -1, numConv, latConv, -1));	//Apertura Tag
	tagAppendi(tagInit(num, artpos, artpos+artleng, -1, -1, artleng));	//Apertura e chiusura Tag
	if (tipo <= articolo)
		tagAppendi(tagInit(rubrica, artpos+artleng, -1, -1, -1, -1));	//Apertura Tag
	else
		tagAppendi(tagInit(corpo, artpos+artleng, -1, -1, -1, -1));		//Apertura Tag
	for (i=tipo+1; i<=numero; i++)
		if (i != articolo)
			sequenzaInit(i);
	loggerDebug(utilConcatena(3, tagTipoToNome(tipo), ": ", yytext));
	artpos+=artleng;

}*/

/*** Salva l'elemento DECORAZIONE */
void saveDec() {
	domTagOpen(decorazione,artpos,artleng);
	artpos += artleng;
}

/*********************************************************** SAVE COMMA NN ****/
void saveCommaNN() {
	loggerDebug("INIZIO saveCommaNN");
	domTagOpen(comma,artpos,0);
	domSetID(comma,commiNN,-1);
	domTagOpen(num,artpos,artleng);
	domTagOpen(corpo,artpos+artleng,0);

//	tagAppendi(tagInit(comma, artpos, -1, commiNN, -1, -1));
//	tagAppendi(tagInit(num, artpos, artpos+artleng, -1, -1, artleng));
//	tagAppendi(tagInit(corpo, artpos+artleng, -1, -1, -1, -1));
	loggerDebug(utilConcatena(3, tagTipoToNome(comma), ": ", yytext));
	artpos += artleng;
	loggerDebug("FINE saveCommaNN");
}

/*void saveCommaNN() {
	loggerDebug("INIZIO saveCommaNN");
	tagAppendi(tagInit(comma, artpos, -1, commiNN, -1, -1));
	tagAppendi(tagInit(num, artpos, artpos+artleng, -1, -1, artleng));
	tagAppendi(tagInit(corpo, artpos+artleng, -1, -1, -1, -1));
	loggerDebug(utilConcatena(3, tagTipoToNome(comma), ": ", yytext));
	artpos += artleng;
	loggerDebug("FINE saveCommaNN");
}*/

// Da 1 a 3 lettere, non da 1 a 2, può essere anche aaa)

%}

S	([ ])
ST	({S}*)
PS	({S}*\.{S}*)
NUM	([0-9]+)
LET 	([a-z]{1,3})   
NL	(\n)
FR	({S}*{NL})
DECORAZ	([(]{S}*[lr]({S}*-?[lr])?{S}*[)]{S}*[(]*(not)*(a|e)*{S}*[)]*)

DISEGNO		((disegno|progetto|proposta){S}+di{S}+legge)

TESTOLIBRO	(libro)
TESTOPARTE	(parte)
TESTOTITOLO	(titolo)
TESTOCAPO	(capo)
TESTOSEZIONE	(sez(\.?|ione))
TESTOARTICOLO	(art(\.?|icolo)\.?)

NUMLIBRO	({TESTOLIBRO}{S}+{ROMANO}{PS}*{LATINO}?{PS}?)
NUMPARTE	({TESTOPARTE}{S}+{ROMANO}{PS}*{LATINO}?{PS}?)
NUMPARTE2	({TESTOPARTE}{S}+{ORD}{PS}?)
NUMTITOLO	({TESTOTITOLO}{S}+({ORD}|{ROMANO}){PS}*{LATINO}?{PS}?)
NUMTITOLOOLD	({TESTOTITOLO}{S}+{ROMANO}{PS}*{LATINO}?{PS}?)
NUMCAPO		({TESTOCAPO}{S}+{ROMANO}{PS}*{LATINO}?{PS}?)
NUMSEZIONE	({TESTOSEZIONE}{S}+{ROMANO}{PS}*{LATINO}?{PS}?)
NUMARTICOLO	({TESTOARTICOLO}{S}*{NUM}{PS}*{LATINO}?{PS}?)
NUMARTICOLOUNICO	({TESTOARTICOLO}{S}*unico{PS}?)

LIBRO		({S}*{NUMLIBRO})
PARTE		({S}*{NUMPARTE})
PARTE2		({S}*{NUMPARTE2})
TITOLO		({S}*{NUMTITOLO})
CAPO		({S}*{NUMCAPO})
SEZIONE		({S}*{NUMSEZIONE})

COMMADEC	([.]?{S}*{DECORAZ}{S}*[.]?{FR})
ARTBASE		({S}*{NUMARTICOLO}{S}*)
ARTICOLO	({S}*{NUMARTICOLO}{S}*{DECORAZ}?{FR})
COMMA1		({S}*(1|0){PS}?{LATINO}?{S}*[).])
COMMA		({FR}*{S}*{NUM}{PS}?{LATINO}?{S}*[).])
COMMANN1	({FR}{1,}{S}*)
COMMANN2	({FR}*{S}*)
LETTERA1	({FR}*{S}*(a){PS}?{LATINO}?[).])
LETTERA		({FR}*{S}*{LET}{PS}?{LATINO}?[).])
NUMERO1		({FR}*{S}*(1|0){PS}?{LATINO}?[).])
NUMERO		({FR}*{S}*{NUM}{PS}?{LATINO}?[).])
PUNTATASIM	(\-|\*|\x95|\x96|\x97|\xAD)
PUNTATA		({FR}*{S}*{PUNTATASIM}{PS}?)

ARTICOLORUB	({S}*{NUMARTICOLO}{S}*{DECORAZ}?)
ARTICOLOAGO	({S}*{NUM}{PS}?{LATINO}?{PS}?{S}*{DECORAZ}?)
GINO	 	({S}*[.]?{S}*\(.*\)\.?({S}*-)?)
RUBRICASEP	([ \-(])
RUBRICA 	({S}*[^10].*{FR})


/* Alla fine di una lettera/numero può esserci una decorazione (dopo il ; o il .) : */
/* PTACAPO		(([.]{FR})|({FR}{FR})) */
/* PVACAPO		(([;]{FR})|({FR}{FR})) */
PTACAPO		(([.]{FR})|({FR}{FR})|([.]{S}*{DECORAZ}{FR}))
PVACAPO		(([;]{FR})|({FR}{FR})|([;]{S}*{DECORAZ}{FR}))

/* Evitare il "Dangerous trailing context" warning del flex (dovuto agli {S}*):  */
/* PTACAPODEC	(([.]{FR})|({NL}{FR})) */
PTACAPODEC	(([.]{FR})|{NL})
DUEPTACAPO	([:]{FR})

VIRGO		(["])
/* 						virgolette sx: win e utf-8 */
VIRG2A		([«\x93]|\xe0\x80\x9c|<<|\xab)	
/* 						virgolette dx: win e utf-8 */
VIRG2C		([»\x94]|\xe0\x80\x9d|>>|\xbb)	

NOTAVVTESTO1	(n{S}*o{S}*t{S}*[ea])
NOTAVVTESTO2	(a{S}*v{S}*v{S}*e{S}*r{S}*t{S}*e{S}*n{S}*z{S}*[ea])
NOTAVVTESTO3	(\({NUM}\))
NOTPREAMBOLO   (p{S}*r{S}*e{S}*a{S}*m{S}*b{S}*o{S}*l{S}*o{S}*)
NOTPREMESSA    (p{S}*r{S}*e{S}*m{S}*e{S}*s{S}*s{S}*a{S}*)
NOTAVV		({FR}+{S}*({NOTAVVTESTO1}|{NOTAVVTESTO2}|{NOTAVVTESTO3}){S}*(all{S}*[']{S}*{NUMARTICOLO})?{S}*([:]|{NL})?(al{S}*{NOTPREAMBOLO})?(alla{S}*{NOTPREMESSA})?)

LAT09		(un|bis|duo|ter|quater|quinquies|sexies|septies|octies|novies)
LAT10		(decies|venies)
LATINO		([- ]*{LAT09}({LAT10})?)
ORDNOSEX	(prim|second|terz|quart|quint|sest|settim|ottav|non|decim)
ORD			({ORDNOSEX}[oa])

ROMANO		([ivxl]+{S}*)


%s InCapo InArticolo InArticoloRubrica
%s InPreComma InComma InCommaAlinea InCommaDec
%s InPreLetNum
%s InLettera InLetteraAlinea
%s InNumero
%s InPuntata
%s InNota
%x InVirgolette InVirgoDoppie
%x Disegno

%option stack

%%

<Disegno>^({S}*{DISEGNO}{S}*)$	{
									numdis++;
									if (numdis==numtes) {
										BEGIN(0);
										//configSetDdlTestate(0); //Altrimenti disturba eventuali allegati...
									}
									artpos+=artleng;
								}

<Disegno>[a-z]{0,1000}	artpos+=artleng;
<Disegno>.|{NL}			artpos+=artleng;


^{LIBRO}/{NL}	{
		if (!checkRomano(libro))
			REJECT;
		save(libro);
		BEGIN(0);
}

^{LIBRO}/{RUBRICASEP}	{
	if (configGetRubricaTipo()==adiacente)
	{
		if (!checkRomano(libro))
			REJECT;
		save(libro);
		BEGIN(0);
	}else 
		REJECT;
}

^{PARTE}/{NL}	{
		if (!checkRomano(parte))
			REJECT;
		save(parte);
		BEGIN(0);
}

^{PARTE}/{RUBRICASEP}	{
	if (configGetRubricaTipo()==adiacente)
	{
		if (!checkRomano(parte))
			REJECT;
		save(parte);
		BEGIN(0);
	}else 
		REJECT;
}

^{PARTE2}/{NL}	{
		if (!checkOrd(parte))
			REJECT;
		save(parte);
		BEGIN(0);
}

^{PARTE2}/{RUBRICASEP}	{
	if (configGetRubricaTipo()==adiacente)
	{
		if (!checkOrd(parte))
			REJECT;
		save(parte);
		BEGIN(0);
	}else 
		REJECT;
}

^{TITOLO}/{NL}	{
		co=checkOrd(titolo);
		cr=checkRomano(titolo);
		if (!(co || cr))
			REJECT;
		save(titolo);
		BEGIN(0);
}

^{TITOLO}/{RUBRICASEP}	{
	if (configGetRubricaTipo()==adiacente)
	{
		if (!(checkOrd(titolo) || checkRomano(titolo)))
			REJECT;
		save(titolo);
		BEGIN(0);
	}else 
		REJECT;
}

^{CAPO}/{NL}	{
	if (!checkRomano(capo))
		REJECT;
	save(capo);
	BEGIN(InCapo);
}

^{CAPO}/{RUBRICASEP}	{
	if (configGetRubricaTipo()==adiacente)
	{
		if (!checkRomano(capo))
			REJECT;
		save(capo);
		BEGIN(InCapo);
	}else 
		REJECT;
}

<InCapo,InComma,InLettera,InNumero>^{SEZIONE}/{NL}	{
	if (!checkRomano(sezione))
		REJECT;
	save(sezione);
}

<InCapo,InComma,InLettera,InNumero>^{SEZIONE}/{RUBRICASEP}	{
	if (configGetRubricaTipo()==adiacente)
	{
		if (!checkRomano(sezione))
			REJECT;
		save(sezione);
	}
	else REJECT;
}

^{ARTICOLORUB}/{RUBRICASEP}		{
	if (configGetRubricaTipo() != adiacente)
		REJECT;
	if (!checkCardinale(articolo))
		REJECT;
	//printf("ARTICOLORUB: artpos=%d, artleng=%d, yytext='%s'\n", artpos, artleng, yytext);
	save(articolo);
	BEGIN(InArticoloRubrica);
}

<InArticoloRubrica>{RUBRICA}	{
	//printf("RUBRICA: artpos=%d, artleng=%d, yytext='%s'\n", artpos, artleng, yytext);
	if (configGetRubricaTipo() != adiacente &&
		configGetRubricaTipo() != deagostini )
		REJECT;
	numConv = 1;
	artpos += artleng;
	////tagNumero(tagN()-1)->fine = artpos;

	//n=tagN();
	//puts(utilItoa(n));
	//for (i=0; i<n; i++)
	//	puts(tagStringa(tagNumero(i)));
	//unput('\n');
	//unput('\n');
	//printf("RUBRICA dopo: artpos=%d, artleng=%d\n", artpos, artleng);
	commiNN = 1;
	BEGIN(InArticolo);
}

^{ARTICOLOAGO}/{RUBRICASEP}		{
	if (configGetRubricaTipo() != deagostini)
		REJECT;
	if (!checkCardinale(articolo))
		REJECT;
	//printf("ARTICOLORUB: artpos=%d, artleng=%d, yytext='%s'\n", artpos, artleng, yytext);
	save(articolo);
	BEGIN(InArticoloRubrica);
}

^{NUMARTICOLOUNICO}		{
	//sequenzaSet(articolo,1); // se attivata identifica eventuali Articolo 2, Articolo 3
	numConv = 1;
	latConv	=	0;
	save(articolo);
	numConv = 1;
	commiNN = 1;
	
	//if (configGetRubriche()==0)
	//	BEGIN(InComma);
		
	BEGIN(InArticolo);
}

^{ARTBASE}	{
	//printf("Articolo -%s-\n",yytext);
	if (!checkCardinale(articolo))
		REJECT;
		
	save(articolo);
	numConv = 1;
	commiNN = 1;
	art_dec = 0;
	BEGIN(InArticolo);
}

<InArticolo>{DECORAZ}	{
	if(art_dec==1)
		REJECT;
	//printf("\nHERE ART DEC pos:%d len:%d txt:%s\n",artpos,artleng,yytext);
	art_dec=1;
	saveDec();
}

<InArticolo>^{COMMA1}	{
	if (configTipoCommi() != commiNumerati)
		REJECT;
	if (!checkCardinale(comma))
		REJECT;
	save(comma);
	//puts("IN COMMA");
	BEGIN(InComma);
}

<InArticolo>{COMMANN1}/[a-z]	{
	if (configTipoCommi() != commiNNLineeVuote) 
		REJECT;
	//printf("COMMA NN 1: artpos='%d' yytext='%s'", artpos, yytext);
	saveCommaNN();
	commiNN++;
	BEGIN(InComma);
}

<InArticolo>{COMMANN2}/[a-z]	{
	if (configGetRubriche() != 0)
		REJECT;
	if (configTipoCommi() != commiNNLineeVuote) 
		REJECT;
	//printf("COMMA NN 1: artpos='%d' yytext='%s'", artpos, yytext);
	saveCommaNN();
	commiNN++;
	BEGIN(InComma);
}

<InPreComma>{FR}*{COMMA}	{
	if (configTipoCommi() != commiNumerati)
		REJECT;
	if (!checkCardinale(comma))
		REJECT;
	save(comma);
	yy_pop_state();
	BEGIN(InComma);
}

<InPreComma>{COMMANN1}/[a-z]	{
	if (configTipoCommi() != commiNNLineeVuote)
		REJECT;
	saveCommaNN();
	commiNN++;
	yy_pop_state();
	BEGIN(InComma);
}

<InPreComma>{COMMANN2}/[a-z]	{
	if (configGetRubriche() != 0)
		REJECT;
	if (configTipoCommi() != commiNNLineeVuote)
		REJECT;
	saveCommaNN();
	commiNN++;
	yy_pop_state();
	BEGIN(InComma);
}

<InComma>{DECORAZ}/{PTACAPODEC}	{
	//printf("DECORAZ: artpos=%d, artleng=%d, yytext='%s'\n", artpos, artleng, yytext);
	saveDec();
	//<InCommaDec> serve per gestire il caso "1. fine. (L) \n 2. Inizio..."
	yy_push_state(InCommaDec);
}

<InComma>{DUEPTACAPO}	{
	artpos += artleng;
	//puts("InCommaAlinea");
	yy_push_state(InCommaAlinea);
}

<InCommaAlinea>^{PUNTATASIM}	{
	if(configGetDTDTipo() != flessibile)
		REJECT;
	save(puntata);
	yy_pop_state();
	BEGIN(InPuntata);
}

<InCommaAlinea>^{LETTERA1}	{
	if (!checkLettera(lettera))
		REJECT;
	//puts("LETTERA1");
	save(lettera);
	yy_pop_state();
	BEGIN(InLettera);
}

<InCommaAlinea>^{NUMERO1}	{
	//Nella flessibile le liste possono essere subito numeriche
	if(configGetDTDTipo() != flessibile)
		REJECT;
	if (!checkCardinale(numero))
		REJECT;
	//puts("NUMERO 1");
	save(numero);
	yy_pop_state();
	BEGIN(InNumero);
}

<InPreLetNum>^{LETTERA}		{
	if (!checkLettera(lettera))
		REJECT;
	//puts("LETTERA");
	save(lettera);
}

<InLettera>{DUEPTACAPO}		{
	artpos += artleng;
	//puts("IN LETTERA ALINEA");
	yy_push_state(InLetteraAlinea);
}

<InLetteraAlinea>{NUMERO1}	{
	if (!checkCardinale(numero))
		REJECT;
	//puts("NUMERO 1");
	save(numero);
	yy_pop_state();
	BEGIN(InNumero);
}

<InPreLetNum>{NUMERO}		{
	if (!checkCardinale(numero))
		REJECT;
	save(numero);
	//puts("NUMERO");
	yy_pop_state();
	BEGIN(InNumero);
}

<InPreLetNum>{PUNTATASIM}		{
	//if (!checkCardinale(numero))
		//REJECT;
	save(puntata);
	yy_pop_state();
	BEGIN(InPuntata);
}

<InComma,InLettera,InNumero,InPuntata>{PTACAPO}	{
	artpos += artleng-1;
	unput('\n');
	//puts("PRE COMMA");
	yy_push_state(InPreComma);
}

<InCommaDec>{NL}	{
	artpos += artleng-1;
	unput('\n');
	//puts("PRE COMMA");
	yy_push_state(InPreComma);
}

<InLettera,InNumero,InPuntata>{PVACAPO}|{NL}	{
	artpos += artleng-1;
	unput('\n');
	//puts("IN PRELETNUM");
	yy_push_state(InPreLetNum);
}

<InPreComma,InPreLetNum,InCommaAlinea,InLetteraAlinea>.		{
	//puts("NO PRECOMMA / COMMA ALINEA / LETTERA ALINEA");
	unput(yytext[0]);
	yy_pop_state();
}

<InPreComma,InNota>{NOTAVV}	{
	sequenzaInc(nota);
		
	domTagOpen(nota,artpos,0);
	domTagOpen(h_p,artpos,0);
	domSetID2(nota,ATTRIB_ID,sequenzaGetNum(nota),0);
		
	//printf("->	%s eon \n",yytext);
	//tagAppendi(tagInit(nota, artpos, -1, -1, -1, -1));
	artpos += artleng;
	//puts("NOTAX");
	//puts(yytext);
	//puts("\nFine nota \n");
	// nota a pie' articolo, si esce dallo stato su un nuovo articolo
	BEGIN(InNota);
}

<InComma,InLettera,InNumero>{VIRGO}		{
		artpos += artleng;
		sequenzaInc(virgolette);	//Incrementa l'ID Vir
		domTagOpen(virgolette,artpos,0);
		domSetID(virgolette,sequenzaGetNum(virgolette),sequenzaGetLat(virgolette));
		yy_push_state(YYSTATE);
		BEGIN(InVirgolette);
}

<InVirgolette>{VIRGO}	{
	domAppendTextToLastNode(artpos);
	domTagCloseFrom(virgolette);
	artpos+=artleng;
	yy_pop_state();
}

<InComma,InLettera,InNumero>{VIRG2A}		{
	nvir2ap = 1;
	artpos += artleng;
	sequenzaInc(virgolette);
	domTagOpen(virgolette,artpos,0);
	domSetID(virgolette,sequenzaGetNum(virgolette),sequenzaGetLat(virgolette));
	yy_push_state(YYSTATE);
	BEGIN(InVirgoDoppie);
}

<InVirgoDoppie>{VIRG2A}		{
	nvir2ap++;
	artpos+=artleng;
}

<InVirgoDoppie>{VIRG2C}		{
	nvir2ap--;
	
	if (nvir2ap==0) {
		domAppendTextToLastNode(artpos);
		domTagCloseFrom(virgolette);
		yy_pop_state();
	}
	artpos+=artleng;
}

<InVirgolette,InVirgoDoppie>{NL}		{
	//La dtd dei DDL non permette 'h:br' all'interno di 'virgolette' (nemmeno la base)
	if(configGetDocTestoTipo() != disegnolegge && configGetDTDTipo() != base)  
		domTagInsertEmpty(virgolette,h_br,artpos);	
	artpos+=artleng;
}

<InVirgolette,InVirgoDoppie>.			{
	artpos+=artleng;
}

#{0,1000}		artpos+=artleng;
[a-z]{0,1000}	artpos+=artleng;
.|{NL}			artpos+=artleng;


%%

int artwrap() {
	return 1;
}


//Avvia la scansione del Buffer e ritorna "1" se ha individuato almeno un ARTICOLO 
int _ArticolatoLexStart(  char * buf)
{
	BEGIN(0);
	if(configGetDocTestoTipo() == disegnolegge)	{
		numtes=configDdlTestate();
		if (numtes)	
			BEGIN(Disegno);
		else
			BEGIN(0);
		}

	numConv=0;
	latConv=0;
	nvir2ap = 1;
	commiNN = 1;
	firstSave=1;
	isArticolato=0;
	artpos=0;
	
	yy_init = 1;
	
	sequenzaSet(nota,999);	//Imposta l'inizio della sequenza ad un valore Elevato
	
	art_scan_string(buf);
	artlex();

	return isArticolato;
}

