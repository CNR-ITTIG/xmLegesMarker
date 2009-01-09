/******************************************************************************
* Project:	xmLeges
* Module:	Marker
* File:		articolato.lex
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Mirco Taddei (m.taddei@ittig.cnr.it)
* 			Lorenzo Bacci (bacci@ittig.cnr.it)
******************************************************************************/

/*
ENCODING:
ISO
i caratteri windows sono trattati come entità numeriche nella tabella ISO.
Es.:
DASH - (ascii: 151 nella win1252): &#x96;

Quando il testo è veramente WIN ci sono i codici del win1252:
Esempio: &#x2019; (apostrofo)

*/

/*
*********************************************
<InLettera,InNumero,InPuntata>{PVACAPO}|{NL}	{ //<--può non esserci il ; alla fine di una lettera!?
//Il problema è che così viene rilevato questo anche in caso di PTACAPO e salta la sequenza (VERIFICARE!)

PTACAPO		(([.]{S}*{DECORAZ}*{FR})|({FR}{FR}))  // <-- vecchio PTACAPO
*********************************************
----SEZIONE (old)----
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
*********************************************
Nota su LISTE: 
sono gestite liste letterali, numeriche, puntate. Le liste letterali possono
contenere liste numeriche. A causa del sistema di controllo sequenza al massimo
si può avere un livello di nidificazione di liste gestito correttamente (non si garantisce
per esempio il caso "lista letterale che contiene lista numerica che contiene altra lista
letterale" con il *controllo sequenza*).
Altra questione: problemi di "gerarchia" dovuti a tag.h e domTagOpen() (determinati elementi
vengono attaccati seguendo uno schema preciso, forse troppo rigido in questo caso): risulta
per esempio in caso di lista numerica con dentro lista letterale che la letterale va direttamente
sotto comma (corpo), cioè sullo stesso livello della numerica.
------
<InLettera>PTACAPO -> si va sempre InPreComma
<InNumero>PTACAPO -> si controlla se segue una lettera, altrimenti InPreComma
<InPuntata>PTACAPO -> si controlla se segue una lettera, altrimenti InPreComma
<InPuntata>PTACAPO -> si controlla se segue un numero, altrimenti InPreComma (ambiguità?)
Non si controlla se segue un numero quando si è in <InLettera> (altrimenti ci sono problemi
di ambiguità con il numero dei commi successivi).
------
11/07/2007 - Aggiunte le seguenti regole per evitare ambiguità ;\n (opp. .\n in un comma)
e non termina la partizione:
<InLettera>{PVACAPO}/{LETTERA} {
<InPuntata>{PVACAPO}/{PUNTATA}	{
<InComma>{PTACAPO}/{PARTIZIONE}	{
*********************************
Fare in modo che entri in "inCommaALinea" solo se c'è un LETTERA1, NUMERO1 opp. PUNTATA
------
Previous:
------
<InComma>{DUEPTACAPO} 	{
	artpos += artleng;
	if(stacklog) puts("InCommaAlinea");
	//BEGIN(InCommaAlinea);
	yy_push_state(InCommaAlinea);
}
*********************************
<InPreComma>{FR}*{COMMA}	{   <---- Perchè {FR}* ?? Così un comma può iniziare a meta riga...
sostituito con:
<InPreComma>^{COMMA}
*********************************

//InPuntata - ambiguo con comma successivo...
<InPuntata>{PTACAPO}/{NUMERO} {
	artpos += artleng-1;
	unput('\n');
	if(stacklog) puts("pop_PTACAPO (numero found)");
	yy_pop_state();
}

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
int nvirap = 0;
int nvir2ap = 0;
int tipovir = 0;
int commiNN = 1;
int firstSave;
int isArticolato;
int co,cr;
int tipoRubriche = 1;
int numdis=0;
int numtes=0;
int art_dec=0;

int flagArt=0;
int flagCom=0;

int stacklog=0;
int countlog=0;
int maxlog=9999;

int startLineInVirgo=0; //1=considera inizio riga anche in virgolette

/******************************************************************* CHECK ****/
int check(tagTipo tipo) {
	int seq;
	char *tipoStr = strdup(tagTipoToNome(tipo));
	loggerDebug(utilConcatena(9, "CHECK: ", tipoStr, " testo=\"", yytext, "\", num=", utilItoa(numConv), "lat=", utilItoa(latConv)));
	seq = sequenzaCheck(tipo, numConv, latConv);
	if (!seq && tipo != titolo) {	//Titolo fa due controlli: ord e rom. Se uno fallisce e l'altro è ok aggiungerebbe comunque lo warning...
		
		//printf("\nCHECK:\"%s\" non in sequenza \"%s\"\n", tipoStr, yytext);
		loggerWarn(utilConcatena(5, "CHECK:", tipoStr, " non in sequenza \"", yytext, "\""));
		
		//Aggiungi un nodo/messaggio di warning?
		char *msg = (char *)malloc(sizeof(char)*1024);
		sprintf(msg, "Generic sequence warning (tipo:%s num:%d lat:%d)", 
						tagTipoToNome(tipo), numConv, latConv);
		int lastnum = sequenzaGetNum(tipo);
		
		if(tipo == comma) {
			sprintf(msg, "Marker Warning 101 (last comma:%d next:%d)", lastnum, numConv); 
		} else if(tipo == lettera) {
			sprintf(msg, "Marker Warning 102 (last lettera:%d next:%d)", lastnum, numConv); 		
		} else if(tipo == articolo) {
			sprintf(msg, "Marker Warning 103 (last articolo:%d next:%d)", lastnum, numConv); 		
		} else {
			printf("\nCHECK:\"%s\" non in sequenza \"%s\"\n", tipoStr, yytext);
		}
		domAddSequenceWarningMsg(tipo, numConv, latConv, msg);
	}
	return seq;
}

/************************************************************ CHECK ORDINALE ****/
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
	if(numConv == -1) return 0;
	latConv = latinToArabic(yytext);
	return check(tipo);
}

/******************************************************************** SAVE ****/
void save(tagTipo tipo) {

	register int i;
	
	//Se vi è un articolo, il documento è articolato
	if ( tipo == articolo || 
		(configGetVirgoMode()==1 && ( tipo==comma || tipo == lettera) ) )	 {
			isArticolato=1;
	}
	
	if (firstSave)
	{
		domTagOpen(tagerrore,0,0);
		firstSave=0;
	}

		domTagOpen(tipo,artpos,0);
		if(stacklog==1 && tipo==articolo) {		
			printf("\nARTICOLO num:%d lat:%d\n",numConv,latConv);
			countlog++;
			if(countlog > maxlog) {
			  printf(">> Turning off stack logs...\n");
			  stacklog = 0;
			  }
		}
		if(tipo==lettera) {
			//printf("\nLETTERA num:%d lat:%d current_lettera:%s\n",numConv,latConv,current_lettera);
			domSetIDLettera(current_lettera,latConv);
		} else {
			domSetID(tipo,numConv,latConv);
		}
			
	domTagOpen(num,artpos,artleng);

	if (tipo <= articolo)
	{
		if (configGetRubriche()!=0) {
			domTagOpen(rubrica,artpos+artleng,0);
		}
	}
	else {
		//printf("\nsave(%s) - open corpo\n", tagTipoToNome(tipo));
		domTagOpen(corpo,artpos+artleng,0);
	}
		
	for (i=tipo+1; i<=numero; i++)
		if (i != articolo)sequenzaInit(i);
	
	loggerDebug(utilConcatena(3, tagTipoToNome(tipo), ": ->", yytext));
	
	artpos+=artleng;
	
}

/*** Salva l'elemento DECORAZIONE */
void saveDec() {
	domTagOpen(decorazione,artpos,artleng);
	artpos += artleng;
}

void saveRub() {
	domTagOpen(rubrica,artpos,artleng);
	artpos+=artleng;
	artleng=0;
}

/*********************************************************** SAVE COMMA NN ****/
void saveCommaNN() {
	loggerDebug("INIZIO saveCommaNN");

	if(stacklog) printf("\nsaveCommaNN() - artpos:%d artlen:%d\n", artpos, artleng);
	domTagOpen(comma,artpos,0);
	domSetID(comma,commiNN,-1);
	//domTagOpen(num,artpos,artleng);
	domNumOpen(num);
	domTagOpen(corpo,artpos+artleng,0);

	int i;
	for(i=comma+1; i<=numero; i++)
		if (i != articolo)sequenzaInit(i);
	

//	tagAppendi(tagInit(comma, artpos, -1, commiNN, -1, -1));
//	tagAppendi(tagInit(num, artpos, artpos+artleng, -1, -1, artleng));
//	tagAppendi(tagInit(corpo, artpos+artleng, -1, -1, -1, -1));
	loggerDebug(utilConcatena(3, tagTipoToNome(comma), ": ", yytext));
	artpos += artleng;
	loggerDebug("FINE saveCommaNN");
}

// Da 1 a 3 lettere, non da 1 a 2, può essere anche aaa)

void openVirgCommon() {

	if(stacklog) printf("\nVIR2A num:%d lat:%d n:%d\n",numConv,latConv,nvir2ap);	
	artpos += artleng;
	if(nvir2ap == 0) {
		sequenzaInc(virgolette);
		domTagOpen(virgolette,artpos,0);
		domSetID(virgolette,sequenzaGetNum(virgolette),sequenzaGetLat(virgolette));
	}
}

void addLetNumWarning() {

	//if(stacklog) puts("WARNING 107!");
	char *msg = strdup("Marker Warning 107");
	domAddSequenceWarningMsg(corpo, numConv, latConv, msg);
}

void addCommaNumWarning() {

	//if(stacklog) puts("WARNING 108!");
	char *msg = strdup("Marker Warning 108");
	domAddSequenceWarningMsg(corpo, numConv, latConv, msg);
}

void addArtNoCommaWarning() {

	//if(stacklog) puts("WARNING 108!");
	char *msg = strdup("Marker Warning 109");
	domAddSequenceWarningMsg(articolo, numConv, latConv, msg);
}

%}

/* Aggiunto 'A0' come carattere spaziatore (causa vecchio ddl senato) */
S	([ ]|[\xa0])
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
/* COMMA1		({S}*(1|0){PS}?{LATINO}?{S}*[).]) */
COMMA1		((({NL}1)|({S}+1)){PS}?{LATINO}?{S}*[).])
COMMA		({FR}*{S}*{NUM}{PS}?{LATINO}?{S}*[).])
COMMANN		({FR}{S}*)
COMMANN1	({FR}{1,}{S}*)
COMMANN2	({FR}*{S}*)
COMMANNFIRST	([^1\n ])
LETTERA1	({S}*(a){PS}?{LATINO}?[).])
LETTERA		({FR}*{S}*{LET}{PS}?{LATINO}?[).])
NUMERO1		({S}*(1|0){PS}?{LATINO}?[).])
NUMERO		({FR}*{S}*{NUM}{PS}?{LATINO}?[).])
PUNTATASIM	(\-|\*|\x95|\x96|\x97|\xAD)
PUNTATA		({S}*{PUNTATASIM}{PS}?)

PARTIZIONE_1 ({LIBRO}|{PARTE}|{PARTE2}|{TITOLO}|{CAPO}|{SEZIONE})
PARTIZIONE_2 ({COMMA}|{ARTBASE})
PARTIZIONE ({PARTIZIONE_1}|{PARTIZIONE_2})

RUBRICASEP	([ \-(])
DASH		(-|\x2D|&#x2d;|\x20\x13|&#x2013;|&#x96;|\x20\x14|&#x2014;|&#x97;)
OPENBRAC	(\()
CLOSEBRAC	(\))
RUBRICALIGHT 	({S}*[^10][a-z]+.*{FR})
RUBRICALIGHTNN 	({S}*[^10\n][a-z]+.*{FR})

/* Va dato un limite altrimenti se separatori non bilanciati la rubrica genera un overflow */
CORPORUB	((.)*{NL}?(.)*{NL}?(.)*{NL}?(.)*)
/* Temp.Fix: Tolto {FR}+ a fine rstrict3 (caso comma non num singolo in virgolette) */
RSTRICT1 	({FR}*{S}*{DASH}{S}*{OPENBRAC}{CORPORUB}{CLOSEBRAC}{S}*[\.]?{S}*{DASH}{S}*{FR}*{S}*)
RSTRICT2	({FR}*{S}*{DASH}{CORPORUB}{DASH}{S}*{FR}+)
RSTRICT3 	({FR}*{S}*{OPENBRAC}{CORPORUB}{CLOSEBRAC}{S}*{FR}+)
RUBRICASTRICT 	({RSTRICT1}|{RSTRICT2}|{RSTRICT3})

/* Evitare il "Dangerous trailing context" warning del flex (dovuto agli {S}*):  */
/* PTACAPODEC	(([.]{FR})|({NL}{FR})) */
PTACAPODEC	(([.]{FR})|{NL})
DUEPT		([:])
DUEPTACAPO	([:]{FR}*)

/* PTACAPO e PVACAPO anche se due FR consecutivi e *non* c'è il carattere ':' prima */
/* Alla fine di una lettera/numero può esserci una ione (dopo il ; o il .) */
/* PTACAPO		(([.]{FR})|({FR}{FR})) */
/* PVACAPO		(([;]{FR})|({FR}{FR})) */
PUNTO		(([.])|([.]{S}*{DECORAZ}{FR}))
PUNTOVIR	(([;])|([;]{S}*{DECORAZ}{FR}))
PTACAPO		(([.]{FR})|({FR}{FR})|([.]{S}*{DECORAZ}{FR}))
PVACAPO		(([;]{FR})|({FR}{FR})|([;]{S}*{DECORAZ}{FR}))

VIRGO		([\"]|\x22|&#x34;)
V2A1		([«]|\xAB|&#xAB;)
V2C1		([»]|\xBB|&#xBB;)
V2A2		(<<|&lt;&lt;|&#x3c;&#x3c;|&#60;&#60;)
V2C2		(>>|&gt;&gt;|&#x3e;&#x3e;|&#62;&#62;)
V2A3		(&#x201c;|&#x93;)
V2C3		(&#x201d;|&#x94;)
/* VIRG2A		([«]|\xAB|\xe0\x80\x9c|<<|&#60;&#60;|&lt;&lt;|&#x201c;|&#x93;|&#xAB;) */
/* VIRG2C		([»]|\xe0\x80\x9d|>>|\xBB|&#62;&#62;|&gt;&gt;|&#x201d;|&#x94;|&#xBB;) */

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


%s InCapo InArticolo
%x InVirgolette InVirgoDoppie
%s InPreComma InComma InCommaAlinea InCommaDec
%s InLettera InNumero InPuntata
%s InLetteraAlinea InNumeroAlinea InPuntataAlinea
%s InPreLet InPreNum InPrePunt
%s InArtRubNum InArtRubNN InArtRubVirgo
%s InNota
%x Disegno

%option stack

%%

<Disegno>^({S}*{DISEGNO}{S}*)$	{
									numdis++;
									if(stacklog) printf("\nDisegno numdis:%d numtes:%d\n", numdis, numtes);
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
		if(stacklog) printf("\nTITOLO num:%d lat:%d\n",numConv,latConv);
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

^{SEZIONE}/{NL}		{
	if (!checkRomano(sezione))
		REJECT;
	save(sezione);
}

^{SEZIONE}/{RUBRICASEP}		{
	if (configGetRubricaTipo()==adiacente)
	{
		if (!checkRomano(sezione))
			REJECT;
		save(sezione);
	}
	else REJECT;
}

^{NUMARTICOLOUNICO}		{
	//sequenzaSet(articolo,1); // se attivata identifica eventuali Articolo 2, Articolo 3
	numConv = 1;
	latConv	=	0;
	save(articolo);
	numConv = 1;
	commiNN = 1;		

	if(configTipoCommi() == commiNumerati) {
		BEGIN(InArtRubNum);
	} else {
		BEGIN(InArtRubNN);
	}
}

^{ARTBASE}	{
	if(configGetVirgoMode()) { REJECT; }
	if(stacklog) printf("\nArticolo -%s-",yytext);
	if (!checkCardinale(articolo))
		REJECT;
		
	save(articolo);

	if( flagArt && !flagCom) {
		addArtNoCommaWarning();
	}
	flagArt = 1;
	flagCom = 0;
	
	numConv = 1;
	commiNN = 1;
	art_dec = 0;
	
	if(configTipoCommi() == commiNumerati) {
		BEGIN(InArtRubNum);
	} else {
		BEGIN(InArtRubNN);
	}
}

^{ARTBASE}		{
	if(!configGetVirgoMode()) { REJECT; }
	if(stacklog) printf("\n--- ARTICOLOVIRGO --\n");	

	save(articolo);
	if( flagArt && !flagCom) {
		addArtNoCommaWarning();
	}
	flagArt = 1;
	flagCom = 0;

	if(configTipoCommi() == commiNumerati) {
		BEGIN(InArtRubNum);
	} else {
		BEGIN(InArtRubNN);
	}
}

^{COMMA} {
	if (!configGetVirgoMode()) { REJECT; }
	
	//int state = yy_top_state();
	//if(stacklog) printf("TOP_STATE:%d.\n", state);
	//Non rilevare nuovi commi se stai analizzando liste
	//if( state > 8 && state < 20) { REJECT; }
	
	if(artpos != 0) { REJECT; }
	save(comma);
	if(stacklog) puts("VIRGO MODE - IN COMMA");
	BEGIN(InComma);
}

^{LETTERA} {
	if (!configGetVirgoMode()) { REJECT; }
	if(artpos != 0) { REJECT; }
	save(lettera);
	if(stacklog) puts("VIRGO MODE - IN LETTERA START");
	BEGIN(InLettera);
}


<InArtRubNum>{RUBRICASTRICT}/{COMMANNFIRST}	{
	/* Fix temporaneo per gestire il comma singolo non numerato nelle virgolette */
	if( !configGetVirgoMode() ) { REJECT; }
	if(stacklog) printf("RubNum - RUBRICASTRICT in COMMA NN - leng:%d\n", artleng);
	saveRub();
	flagCom = 1;
	saveCommaNN();
	commiNN++;
	BEGIN(InComma);
}

<InArtRubNum>{COMMA1}	{
	if (configTipoCommi() != commiNumerati)
		REJECT;
	if (!checkCardinale(comma))
		REJECT;

	if(stacklog) printf("RUBRICA dopo: artpos=%d, artleng=%d\n", artpos, artleng);
	flagCom = 1;
	save(comma);
	if(stacklog) puts("IN ARTICOLO - IN COMMA 1");
	BEGIN(InComma);
}

<InArtRubNum>{DECORAZ}	{
	if(art_dec==1)
		REJECT;
	if(stacklog) printf("\nHERE ART DEC pos:%d len:%d txt:%s\n",artpos,artleng,yytext);
	art_dec=1;
	saveDec();
}

<InArtRubNN>{RUBRICALIGHTNN}	{
	if(configGetVirgoMode()) {REJECT;} //Nelle virgolette non ci sono gli \n...? vai al comma subito
	if(stacklog) printf("\nRubNN - RUBRICALIGHTNN");
	saveRub();
	flagCom = 1;
	saveCommaNN();
	commiNN++;
	BEGIN(InComma);
}

<InArtRubNN>{RUBRICASTRICT}	{
	if(stacklog) printf("\nRubNN - RUBRICASTRICT");
	saveRub();
	flagCom = 1;
	saveCommaNN();
	commiNN++;
	BEGIN(InComma);
}

<InArtRubNN>{COMMANN}	{
	if (stacklog) puts("\nCOMMANN rilevato.\n");
	if (configTipoCommi() == commiNumerati) 
		REJECT;
	flagCom = 1;
	saveCommaNN();
	if(stacklog) puts("IN ARTICOLO - IN COMMA NN");
	commiNN++;
	BEGIN(InComma);
}

<InPreComma>^{COMMA}	{
    if(stacklog) printf("\nInPreComma->Comma\n");
	if (configTipoCommi() != commiNumerati)
		REJECT;
	if (!checkCardinale(comma))
		REJECT;
    save(comma);
	yy_pop_state();
	BEGIN(InComma);
}

<InPreComma>{COMMA}	{
	if (!configGetVirgoMode()) {	REJECT; }
	save(comma);
	yy_pop_state();
	if(stacklog) puts("IN COMMA (FR)");
	BEGIN(InComma);
}

<InPreComma>{COMMANN1}/[a-z]	{
	if (configTipoCommi() != commiNNLineeVuote)
		REJECT;
	saveCommaNN();
	commiNN++;
	yy_pop_state();
	if(stacklog) puts("IN COMMA (NN1)");
	BEGIN(InComma);
}

<InPreComma>{COMMANN2}/[a-z]	{
	if (configTipoCommi() != commiNNSenzaLinea)
		REJECT;
	saveCommaNN();
	commiNN++;
	yy_pop_state();
	if(stacklog) puts("IN COMMA (NN2)");
	BEGIN(InComma);
}

<InComma>{DECORAZ}/{PTACAPODEC}	{
	//printf("DECORAZ: artpos=%d, artleng=%d, yytext='%s'\n", artpos, artleng, yytext);
	saveDec();
	//<InCommaDec> serve per gestire il caso "1. fine. (L) \n 2. Inizio..."
	if(stacklog) puts("IN COMMADEC");
	yy_push_state(InCommaDec);
}

<InComma>{DUEPTACAPO}/{LETTERA1}	|
<InComma>{DUEPTACAPO}/{NUMERO1}	|
<InComma>{DUEPTACAPO}/{PUNTATA}	{
	artpos += artleng;
	if(stacklog) puts("InCommaAlinea");
	yy_push_state(InCommaAlinea);
}

<InCommaAlinea>{LETTERA1}	{
	if (!checkLettera(lettera))
		REJECT;
	if(stacklog) puts("LETTERA1");
	save(lettera);
	yy_push_state(InLettera);
}

<InCommaAlinea>{NUMERO1}	{
	//Nella flessibile le liste possono essere subito numeriche
	if(configGetDTDTipo() != flessibile) {
		addCommaNumWarning();
		REJECT;
	}
	if (!checkCardinale(numero))
		REJECT;
	if(stacklog) puts("NUMERO 1");
	save(numero);
	yy_push_state(InNumero);
}

<InCommaAlinea>{PUNTATA} {
	if(configGetDTDTipo() != flessibile)
		REJECT;
	save(puntata);
	yy_push_state(InPuntata);
}

<InLettera>{DUEPT}/{NUMERO} {
	artpos += artleng;
	if(stacklog) puts("IN LETTERA ALINEA");
	yy_push_state(InLetteraAlinea);
}

<InLettera>{DUEPT}/{PUNTATA} {
	artpos += artleng;
	if(stacklog) puts("IN LETTERA ALINEA");
	yy_push_state(InLetteraAlinea);
}

<InNumero>{DUEPT}/{LETTERA} {
	artpos += artleng;
	if(configGetDTDTipo() == flessibile) {
		if(stacklog) puts("IN NUMERO ALINEA");
		yy_push_state(InNumeroAlinea);
	} else {
		addLetNumWarning();
		if(stacklog) puts("Avoiding LETTERA inside NUMERO (dtd non flessibile)");
	}
}

<InNumero>{DUEPT}/{PUNTATA} {
	artpos += artleng;
	if(stacklog) puts("IN NUMERO ALINEA");
	yy_push_state(InNumeroAlinea);
}

<InPuntata>{DUEPT} {
	artpos += artleng;
	if(stacklog) puts("IN PUNTATA ALINEA");
	yy_push_state(InPuntataAlinea);
}

<InLetteraAlinea>{NUMERO1}	{
	if (!checkCardinale(numero))
		REJECT;
	if(stacklog) puts("NUMERO 1");
	save(numero);
	yy_push_state(InNumero);
}

<InLetteraAlinea>{LETTERA}	 {
	if(!checkLettera(lettera))
		REJECT;
	//Senza controllo sequenza potrebbe scambiare anche "Art." per una lettera (vedi modifica in util.c)
	save(lettera);
	if(stacklog) puts("pop_InLetteraAlinea");
	yy_pop_state();
}

<InLetteraAlinea>{PUNTATA}	{
	if(stacklog) puts("PUNTATA 1");
	save(puntata);
	yy_push_state(InPuntata);
}

<InNumeroAlinea>{LETTERA1}	{
	if (!checkLettera(lettera))
		REJECT;
	if(stacklog) puts("LETTERA 1");
	save(lettera);
	yy_push_state(InLettera);
}

<InNumeroAlinea>{NUMERO} {
	if(!checkCardinale(numero))
		REJECT;
	save(numero);
	if(stacklog) puts("pop_InNumeroAlinea");
	yy_pop_state();
}

<InNumeroAlinea>{PUNTATA}	{
	if(stacklog) puts("PUNTATA 1");
	save(puntata);
	yy_push_state(InPuntata);
}

<InPuntataAlinea>{LETTERA1}	{
	if (!checkLettera(lettera))
		REJECT;
	if(stacklog) puts("LETTERA 1");
	save(lettera);
	yy_push_state(InLettera);
}

<InPuntataAlinea>{NUMERO} {
	if(!checkCardinale(numero))
		REJECT;
	save(numero);
	if(stacklog) puts("pop_InNumeroAlinea");
	yy_pop_state();
}

<InPuntataAlinea>{PUNTATA}	{
	if(stacklog) puts("PUNTATA 1");
	save(puntata);
	yy_push_state(InPuntata);
}

<InLettera>{PUNTOVIR}/{LETTERA} {
	artpos += artleng;
	if(stacklog) puts("IN PRELET");
	yy_push_state(InPreLet);
}

<InNumero>{PUNTOVIR}/{NUMERO}	{
	artpos += artleng;
	if(stacklog) puts("IN PRENUM");
	yy_push_state(InPreNum);
}

<InNumero>{PUNTOVIR}/{LETTERA}	{
	artpos += artleng;
	//if(stacklog) puts("IN PRELET");
	//yy_push_state(InPreLet);
	if(stacklog) puts("In Num: PUNTOVIR - Popping to Lettera anyway!");
	yy_pop_state();
}

<InPuntata>{PUNTOVIR}/{PUNTATA}	{
	artpos += artleng-1;
	unput('\n');
	if(stacklog) puts("IN PREPUNT");
	yy_push_state(InPrePunt);
}

<InPreLet>{LETTERA}	{
	if (!checkLettera(lettera))
		REJECT;
	save(lettera);
	if(stacklog) puts("pop_InPreLet");
	yy_pop_state();
}

<InPreNum>{NUMERO}	{
	if (!checkCardinale(numero))
		REJECT;
	save(numero);
	if(stacklog) puts("pop_InPreNum");
	yy_pop_state();
}

<InPrePunt>{PUNTATA}	{
	save(puntata);
	if(stacklog) puts("pop_InPrePunt");
	yy_pop_state();
}

<InNumero>{PTACAPO}/{LETTERA} {
	artpos += artleng-1;
	unput('\n');
	if(stacklog) puts("pop_PUNTO (lettera found)");
	yy_pop_state();
}

<InPuntata>{PTACAPO}/{LETTERA} {
	artpos += artleng-1;
	unput('\n');
	if(stacklog) puts("pop_PUNTO (lettera found)");
	yy_pop_state();
}

<InLettera,InNumero,InPuntata>{PTACAPO}/{PARTIZIONE}	{
	artpos += artleng-1;
	unput('\n');
	//printf("\nPTACAPO: artpos=%d, artleng=%d, yytext='%s'", artpos, artleng, yytext);
	//printf("\nPTACAPO: artpos=%d, artleng=%d\n", artpos, artleng);
	if(stacklog) puts("pop_PTACAPO(PARTIZIONE FOUND)->InPreComma");
	yy_pop_state();
	yy_push_state(InPreComma);
}

<InLettera,InNumero,InPuntata>{PTACAPO} {
	if( configTipoCommi() == commiNumerati ) {
		REJECT;
	}	
	artpos += artleng-1;
	unput('\n');
	if(stacklog) puts("pop_PTACAPO(commaNN)->InPreComma");
	yy_pop_state();
	yy_push_state(InPreComma);

}

<InComma>{PTACAPO}/{PARTIZIONE} {
	artpos += artleng-1;
	unput('\n');
	if(stacklog) puts("PARTIZIONE found, pushing PRE COMMA");
	yy_push_state(InPreComma);
}

<InComma>{PTACAPO}/{COMMANN1} {
	if( configTipoCommi() != commiNNLineeVuote ) {
		REJECT;
	}	
	artpos += artleng-1;
	unput('\n');
	if(stacklog) puts("PRE COMMA nn1");
	yy_push_state(InPreComma);
}

<InComma>{PTACAPO}/{COMMANN2} {
	if( configTipoCommi() != commiNNSenzaLinea ) {
		REJECT;
	}	
	artpos += artleng-1;
	unput('\n');
	if(stacklog) puts("PRE COMMA nn2");
	yy_push_state(InPreComma);
}

<InCommaDec>{NL}	{
	artpos += artleng-1;
	unput('\n');
	if(stacklog) puts("commaDec - PRE COMMA");
	yy_push_state(InPreComma);
}

<InPreComma,InNota>{NOTAVV}	{
	sequenzaInc(nota);
	//printf("\nNOTA: artpos=%d, artleng=%d, yytext='%s'", artpos, artleng, yytext);
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

<InComma,InLettera,InNumero>{DUEPT}?{VIRGO}	{
	if(configGetVirgoMode()) { REJECT; }
	nvirap = 1;	
	artpos += artleng;
	if(stacklog) printf("\nVIRGO AP num:%d lat:%d n:%d\n",numConv,latConv,nvirap);
	sequenzaInc(virgolette);	//Incrementa l'ID Vir
	domTagOpen(virgolette,artpos,0);
	domSetID(virgolette,sequenzaGetNum(virgolette),sequenzaGetLat(virgolette));
	yy_push_state(YYSTATE);
	BEGIN(InVirgolette);
}

<InVirgolette>{VIRGO}	{
	nvirap--;	
	if(stacklog) printf("\nVIRGO CL num:%d lat:%d n:%d\n",numConv,latConv,nvirap);
	if (nvirap==0) {
		domAppendTextToLastNode(artpos);
		domTagCloseFrom(virgolette);
		yy_pop_state();
	}
	artpos+=artleng;
}

<InComma,InLettera,InNumero>{DUEPT}?{V2A1}	{
	if(configGetVirgoMode()) { REJECT; }
	openVirgCommon();
	if(nvir2ap == 0) {
		if(stacklog) puts("V2A1");
		nvir2ap = 1;
		tipovir = 1;
		yy_push_state(YYSTATE);
		BEGIN(InVirgoDoppie);
	}
}
<InComma,InLettera,InNumero>{DUEPT}?{V2A2}	{
	if(configGetVirgoMode()) { REJECT; }
	openVirgCommon();
	if(nvir2ap == 0) {
		if(stacklog) puts("V2A2");
		nvir2ap = 1;
		tipovir = 2;
		yy_push_state(YYSTATE);
		BEGIN(InVirgoDoppie);
	}
}
<InComma,InLettera,InNumero>{DUEPT}?{V2A3}	{
	if(configGetVirgoMode()) { REJECT; }
	openVirgCommon();
	if(nvir2ap == 0) {
		if(stacklog) puts("V2A3");
		nvir2ap = 1;
		tipovir = 3;
		yy_push_state(YYSTATE);
		BEGIN(InVirgoDoppie);
	}
}

<InVirgoDoppie>{V2A1}		{
	if(tipovir == 1) nvir2ap++;
	artpos+=artleng;
}
<InVirgoDoppie>{V2A2}		{
	if(tipovir == 2) nvir2ap++;
	artpos+=artleng;
}
<InVirgoDoppie>{V2A3}		{
	if(tipovir == 3) nvir2ap++;
	artpos+=artleng;
}

<InVirgoDoppie>{V2C1}		{
	if(tipovir == 1) nvir2ap--;
	if(stacklog) printf("\nVIR2C1 num:%d lat:%d n:%d\n",numConv,latConv,nvir2ap);	
	if (nvir2ap == 0) {
		domAppendTextToLastNode(artpos);
		domTagCloseFrom(virgolette);
		yy_pop_state();
	}
	artpos+=artleng;

}
<InVirgoDoppie>{V2C2}		{
	if(tipovir == 2) nvir2ap--;
	if(stacklog) printf("\nVIR2C2 num:%d lat:%d n:%d\n",numConv,latConv,nvir2ap);	
	if (nvir2ap == 0) {
		domAppendTextToLastNode(artpos);
		domTagCloseFrom(virgolette);
		yy_pop_state();
	}
	artpos+=artleng;
}
<InVirgoDoppie>{V2C3}		{
	if(tipovir == 3) nvir2ap--;
	if(stacklog) printf("\nVIR2C3 num:%d lat:%d n:%d\n",numConv,latConv,nvir2ap);	
	if (nvir2ap == 0) {
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
	//printf("\nBUFFER:\n%s", buf);
	BEGIN(0);
	if(configGetDocTestoTipo() == disegnolegge && !configGetVirgoMode() )	{
		numtes=configDdlTestate();
		if(stacklog) printf("\nDis numdis:%d numtes:%d\n", numdis, numtes);
		if (numtes)	
			BEGIN(Disegno);
		else
			BEGIN(0);
	}

	numConv=0;
	latConv=0;
	nvir2ap = 0;
	tipovir = 0;
	commiNN = 1;
	firstSave=1;
	isArticolato=0;
	artpos=0;
	flagArt = 0;
	flagCom = 0;
	//domInitStates();
	
	yy_init = 1;
	
	sequenzaSet(nota,999);	//Imposta l'inizio della sequenza ad un valore Elevato
	
	if(stacklog) printf("\n STARTING len:%d ------------------------------------\n", strlen(buf));
	//printf("\nBUFFER:\n%s\n", buf);
	
	art_scan_string(buf);
	artlex();

	if(stacklog) printf("\n ENDING ------------------------------------\n");
	
	return isArticolato;
}

