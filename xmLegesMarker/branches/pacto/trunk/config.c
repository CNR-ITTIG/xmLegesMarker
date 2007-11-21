/******************************************************************************
* Project:	xmLeges
* Module:	Marker
* File:		config.c
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Mirco Taddei (m.taddei@ittig.cnr.it)
******************************************************************************/
#include <stdio.h>
#include <string.h>
#include "config.h"

//MODULI ( 0 Attivato, 1 Disattivato )
int		_configModuloTabelle=0;
int		_configModuloAnnessi=0;
//-----------------------------------

char		_configDirTmp[MAXFILENAME+1] = "/tmp";
//char		_configLogFile[MAXFILENAME+1] = "ParserStruttura.log";
//tipoLogDisable	 _configLogDisable=disableNone;

/*****************************************/
char		_configDocTestoStringa[MAXSTRINGA+1] = "Legge";		// tag contenitore dell'atto (default=legge)
tipoTesto	_configDocTestoTipo = legge;				// INIT

char		_configDocNome[MAXSTRINGA+1] = "";			// nome del DocumentoNIR (no default)

tipoStruttura	_configDocStruttura = docarticolato;			// tipo struttura (default=articolato)

/*****************************************/
int		_configNodeCount=0;			//DISATTIVATO
tipoDTD		_configDTDTipo=completo;	//INIT

char _configDTDBaseStringa[MAXSTRINGA+1] = "nirbase.dtd";
//char _configDTDBaseMeta[MAXSTRINGA+1] = "<meta><descrittori><pubblicazione norm=\"\"/><redazione nome=\"\" id=\"red1\" norm=\"\"/><urn>URN</urn></descrittori></meta>";
// meta --> lavoripreparatori? , redazionale? , proprietario* , disposizioni?

char _configDTDCompletoStringa[MAXSTRINGA+1] = "nircompleto.dtd"; 
//char _configDTDCompletoMeta[MAXSTRINGA+1] = "<meta>META</meta>";
//char *_configDTDCompletoMeta = _configDTDBaseMeta;

char _configDTDFlessibileStringa[MAXSTRINGA+1] = "nirflessibile.dtd";
//char _configDTDFlessibileMeta[MAXSTRINGA+1] = "<meta>META</meta>";
//char *_configDTDFlessibileMeta = _configDTDBaseMeta;

char _configDTDDlStringa[MAXSTRINGA+1] = "dllight.dtd";
//char _configDTDCnrStringa[MAXSTRINGA+1] = "cnr.dtd";

char _configHeaderParser[MAXSTRINGA+1] = "/usr/local/bin/HeaderParser";
char _configHeaderParserModels[MAXSTRINGA+1] = "/usr/local/lib/HeaderParser/Models";

//da file Testa2.xml
// struttura testa documento articolato
char _configTesta1[MAXSTRINGA+1]=	"<atto xmlns:h=\"html\">\n<meta>\n<descrittori>\n<pubblicazione tipo=\"GU\" num=\"\" norm=\"\"/>\n<entratainvigore norm=\"\"/>\n<redazione nome=\"\" id=\"red1\" norm=\"\"/>\n<urn valore=\"\"/>\n</descrittori>\n</meta>\n<intestazione/>\n<formulainiziale>\n<h:p/>\n</formulainiziale>\n</atto>";
// struttura testa documento semiarticolato
char _configTesta2[MAXSTRINGA+1]=	"<atto xmlns:h=\"html\">\n<meta>\n<descrittori>\n<pubblicazione tipo=\"GU\" num=\"\" norm=\"\"/>\n<entratainvigore norm=\"\"/>\n<redazione nome=\"\" id=\"red1\" norm=\"\"/>\n<urn valore=\"\"/>\n</descrittori>\n</meta>\n<intestazione/>\n<formulainiziale>\n<h:p/>\n</formulainiziale>\n</atto>";

// struttura coda documento articolato
char _configCoda1[MAXSTRINGA+1]=	"<atto xmlns:h=\"html\">\n<formulafinale>\n<h:p/>\n</formulafinale>\n<conclusione/>\n</atto>";
// struttura coda documento semiarticolato
char _configCoda2[MAXSTRINGA+1]=	"<atto xmlns:h=\"html\">\n<formulafinale/>\n<conclusione/>\n</atto>";

char _configTestaVuotaBase[MAXSTRINGA+1] = "<TESTA_VUOTA_BASE></TESTA_VUOTA_BASE>";
char _configTestaVuotaCompleto[MAXSTRINGA+1] = "<TESTA_VUOTA_COMPLETO></TESTA_VUOTA_COMPLETO>";
char _configTestaVuotaFlessibile[MAXSTRINGA+1] = "<TESTA_VUOTA_FLESSIBILE></TESTA_VUOTA_FLESSIBILE>";
char _configCodaVuotaBase[MAXSTRINGA+1] = "<CODA_VUOTA_BASE></CODA_VUOTA_BASE>";
char _configCodaVuotaCompleto[MAXSTRINGA+1] = "<CODA_VUOTA_COMPLETO></CODA_VUOTA_COMPLETO>";
char _configCodaVuotaFlessibile[MAXSTRINGA+1] = "<CODA_VUOTA_FLESSIBILE></CODA_VUOTA_FLESSIBILE>";

char _configArticolatoVuoto[MAXSTRINGA+1] = "<articolato>\n<articolo id=\"art1\">\n<num>Art. 1.</num>\n<rubrica/>\n<comma id=\"com1\">\n<num>1.</num>\n<corpo/>\n</comma>\n</articolo>\n</articolato>";

// struttura descrittori
char _configDescrittoriVuoto[MAXSTRINGA+1]=	"<descrittori>\n<pubblicazione tipo=\"GU\" num=\"\" norm=\"\"/>\n<entratainvigore norm=\"\"/>\n<redazione nome=\"\" id=\"red1\" norm=\"\"/>\n<urn valore=\"\"/>\n</descrittori>";

char _configEncoding[MAXSTRINGA+1] = "iso-8859-15"; //di default il file in input viene preso come iso-8859-15

tipoCommi _tipoCommi = commiNumerati;
tipoInput _tipoInput = txt;

int _configSequenzaCheck = 1;
int _rubriche = 1;

int _disegni=0;

//MODULI ==========================================================================

void	configSetDisable(tipoModulo pmodulo)
{
	switch (pmodulo) {
		case mod_tabelle:	_configModuloTabelle=1; return;
		case mod_annessi:	_configModuloAnnessi=1; return;
	}
}

int		configIsEnabled(tipoModulo pmodulo)
{
	switch (pmodulo) {
		case mod_tabelle:		return !_configModuloTabelle;
		case mod_annessi:		return !_configModuloAnnessi;
	}
}


//---------------------------------------------------- TIPO DI RUBRICA (A CAPO / ADIACENTE)

tipoRubrica _configRubricaTipo = acapo;		//INIT

tipoRubrica configGetRubricaTipo(void)
{
	return _configRubricaTipo;
}

void configSetRubricaTipo(tipoRubrica liv)
{
	_configRubricaTipo =liv;
}

//---------------------------------------------------- NODE COUNT

void configSetNodeCount(int a)
{
	_configNodeCount=a;
}

int configGetNodeCount(void)
{
	return _configNodeCount;
}

char *configGetTestaDefault(int i){
	switch (i){
		case 1:		return &_configTesta1[0];
		case 2:		return &_configTesta2[0];
	}
	return "";
}

char *configGetCodaDefault(int i){
	switch (i){
		case 1:		return &_configCoda1[0];
		case 2:		return &_configCoda2[0];
	}
	return "";
}

/******************************************************************************/
/**************************************************** CONFIG CONFIG IN LOG ****/
/******************************************************************************/
void configConfigInLog(void) {
//	DA FARE
	loggerInfo("CONFIG configurazione usata per l'analisi");
	loggerInfo(utilConcatena(2, "CONFIG DirTmp=", configDirTmp()));
	loggerInfo(utilConcatena(2, "CONFIG LogFile=", loggerGetFile()));
//	loggerInfo(utilConcatena(2, "CONFIG DTDStringa(base)=", configDTDStringa(base)));
//	loggerInfo(utilConcatena(2, "CONFIG DTDStringa(completo)=", configDTDStringa(completo)));
//	loggerInfo(utilConcatena(2, "CONFIG DTDStringa(flessibile)=", configDTDStringa(flessibile)));
//	loggerInfo(utilConcatena(2, "CONFIG DirDTDMeta(base)=", configDTDMeta(base)));
	loggerInfo(utilConcatena(2, "CONFIG HeaderParserModels=", configHeaderParserModels()));
	loggerInfo(utilConcatena(2, "CONFIG SequenzaCheck=", utilItoa(configSequenzaCheck())));
	loggerInfo(utilConcatena(2, "CONFIG Encoding=", configEncoding()));
}

/******************************************************************************/
/************************************************************ CONFIG LEGGI ****/
/******************************************************************************/
int configLeggi(char *fn) {
	FILE *fp;

	if (!(fp = fopen(fn, "r")))
		return 0;
	while (!feof(fp)) {
		char s[MAXLINE];
		char *r;
		fgets(s, MAXLINE, fp);
		
		if (s[0] == '#') continue;
		r = strstr(s, "\n"); if (r) *r=0;
		r = strstr(s, "\r"); if (r) *r=0;

		if (strstr(s,"dirtmp="))
			strncpy(_configDirTmp, strstr(s, "=")+1, MAXFILENAME);
		else if (strstr(s,"logfile="))
			loggerSetFile(strstr(s, "=")+1);
		else if (strstr(s,"dtdbase="))
			strncpy(_configDTDBaseStringa, strstr(s, "=")+1, MAXSTRINGA);
		else if (strstr(s,"dtdcompleto="))
			strncpy(_configDTDCompletoStringa, strstr(s, "=")+1, MAXSTRINGA);
		else if (strstr(s,"dtdflessibile="))
			strncpy(_configDTDFlessibileStringa, strstr(s, "=")+1, MAXSTRINGA);
		else if (strstr(s, "HeaderParser="))
			strncpy(_configHeaderParser, strstr(s, "=")+1, MAXSTRINGA);
		else if (strstr(s, "HeaderParserModels="))
			strncpy(_configHeaderParserModels, strstr(s, "=")+1, MAXSTRINGA);
		else if (strstr(s, "TestaVuotaBase="))
			strncpy(_configTestaVuotaBase, strstr(s, "=")+1, MAXSTRINGA);
		else if (strstr(s, "TestaVuotaCompleto="))
			strncpy(_configTestaVuotaCompleto, strstr(s, "=")+1, MAXSTRINGA);
		else if (strstr(s, "TestaVuotaFlessibile="))
			strncpy(_configTestaVuotaFlessibile, strstr(s, "=")+1, MAXSTRINGA);
		else if (strstr(s, "CodaVuotaBase="))
			strncpy(_configCodaVuotaBase, strstr(s, "=")+1, MAXSTRINGA);
		else if (strstr(s, "CodaVuotaCompleto="))
			strncpy(_configCodaVuotaCompleto, strstr(s, "=")+1, MAXSTRINGA);
		else if (strstr(s, "CodaVuotaFlessibile="))
			strncpy(_configCodaVuotaFlessibile, strstr(s, "=")+1, MAXSTRINGA);
		else if (strstr(s, "Encoding="))
			strncpy(_configEncoding, strstr(s, "=")+1, MAXSTRINGA);
	}
	fclose(fp);
	return 1;
}

/******************************************************************************/
/********************************************************** CONFIG DIR TMP ****/
/******************************************************************************/
char *configDirTmp() {
	return _configDirTmp;
}

/******************************************************************************/
/**************************************** CONFIG DOC: testo, nome, struttura **/
/******************************************************************************/
char * tipoTestoStringa(tipoTesto testo) {
	switch (testo) 
	{
	case legge: 					return "Legge";
	case leggecostituzionale: 			return "LeggeCostituzionale";
	case decretolegge: 				return "DecretoLegge";
	case decretolegislativo: 			return "DecretoLegislativo";
	case regiodecreto: 				return "RegioDecreto";
	case decretopresidenterepubblica: 		return "Dpr";
	case decretopresidenterepubblicaNN: 		return "DprNN";
	case decretopresedenteconsiglioministri:	return "Dpcm";
	case decretopresedenteconsiglioministriNN:	return "DpcmNN";
	case decretoministeriale: 			return "DecretoMinisteriale";
	case decretoministerialeNN: 			return "DecretoMinisterialeNN";
	case leggeregionale: 				return "LeggeRegionale";
	case attoauthority: 				return "AttoDiAuthority";
	case comunicato: 				return "Comunicato";		// vers. 2
	case disegnolegge: 				return "DisegnoLegge";		// vers. 2
	case provCNR:					return "DocumentoNIR";
	case documentoNIR:				return "DocumentoNIR";		// doc. generico (articolato e no)
	case statutocomunale:			return "DocumentoNIR";
	case regolamentocomunale:		return "DocumentoNIR";
	}
	return "DocumentoNIR";
}

//---------------------------------------------------- DOCUMENTO TESTO TIPO O STRINGA

void configSetDocTesto(tipoTesto t){				// Imposta il tag del contenitore
	_configDocTestoTipo = t;
	strcpy(_configDocTestoStringa,tipoTestoStringa(t));
	_configDocStruttura=tipoTestoStruttura(t);
}

char *configGetDocTestoString(void)
{
	return 	_configDocTestoStringa;
}

tipoTesto configGetDocTestoTipo(void)
{
	return 	_configDocTestoTipo;
}

//---------------------------------------------------- DOCUMENTO NOME

char *configGetDocNome(void)
{
	return _configDocNome;
}

void configSetDocNome(char *docNome)   // Imposta l'attributo nome a partire da una stringa qualunque
{
	strcpy(_configDocNome,docNome);
}

//---------------------------------------------------- DOCUMENTO STRUTTURA

tipoStruttura tipoTestoStruttura(tipoTesto testo) {
	switch (testo) 
	{
	case legge: 					return docarticolato;
	case leggecostituzionale: 			return docarticolato;
	case decretolegge: 				return docarticolato;
	case decretolegislativo: 			return docarticolato;
	case regiodecreto: 				return docarticolato;
	case decretopresidenterepubblica: 		return docarticolato;
	case decretopresedenteconsiglioministri:	return docarticolato;
	case decretoministeriale: 			return docarticolato;
	case leggeregionale: 				return docarticolato;
	case disegnolegge: 				return docarticolato;
	case statutocomunale:			return docarticolato;
	case regolamentocomunale:		return docarticolato;
	}
	return docsemiarticolato;
}

tipoStruttura configGetDocStruttura(void)
{
	return (_configDocStruttura);
}


/******************************************************************************/
/********************************************************* CONFIG LOG *********/
/******************************************************************************/

//void configSetLogDisable(tipoLogDisable t){
//	_configLogDisable=t;
//}
//
//int configGetLogDisable(tipoLogDisable t){
//	if (t==_configLogDisable)return 1;
//	return 0;
//}
//
//int configLogDisable(void) {
//	return _configLogDisable;
//}
//

///******************************************************************************/
///********************************************************* CONFIG LOG FILE ****/
///******************************************************************************/
//char *configLogFile(void) {
//	return _configLogFile;
//}


/******************************************************************************/
/****************************************************** CONFIG DTD STRINGA ****/
/******************************************************************************/

void configSetDTDTipo(tipoDTD tipo)
{
	_configDTDTipo=tipo;
}

tipoDTD	configGetDTDTipo(void)
{
	return _configDTDTipo;
}

char *configGetDTDTipoStringa(void){
	switch (_configDTDTipo) {
		case base:			return _configDTDBaseStringa;
		case completo:		return _configDTDCompletoStringa;
		case flessibile:	return _configDTDFlessibileStringa;
		case dl:			return _configDTDDlStringa;		
		//case cnr:			return _configDTDCnrStringa;
		default:			return "";
	}
}


/******************************************************************************/
/********************************************************* CONFIG DTD META ****/
/******************************************************************************/
/*char *configDTDMeta(tipoDTD tipo) {
	switch (tipo) {
		case base:		return _configDTDBaseMeta;
		case completo:		return _configDTDCompletoMeta;
		case flessibile:	return _configDTDFlessibileMeta;
		default:		return "";DdlTestate
	}
}
*/

/******************************************************************************/
/**************************************************** CONFIG HEADER PARSER ****/
/******************************************************************************/
//char *configHeaderParser(HeaderParserFile file) {
//	switch (file) {
		/*case base:		return _configDTDBaseMeta;
		case completo:		return _configDTDCompletoMeta;
		case flessibile:	return _configDTDFlessibileMeta;
		*/
//	}
//}


/******************************************************************************/
/**************************************************** CONFIG HEADER PARSER ****/
/******************************************************************************/
char *configHeaderParser() {
	return _configHeaderParser;
}

void configSetHeaderParser(char *parser) {
	strncpy(_configHeaderParser, parser, MAXSTRINGA);
}


/******************************************************************************/
/********************************************* CONFIG HEADER PARSER MODELS ****/
/******************************************************************************/
char *configHeaderParserModels() {
	return _configHeaderParserModels;
}

void configSetHeaderParserModels(char *models) {
	strncpy(_configHeaderParserModels, models, MAXSTRINGA);
}


/******************************************************************************/
/****************************************************** CONFIG DDL TESTATE ****/
/******************************************************************************/
int configDdlTestate() {
	return _disegni;
}

void configSetDdlTestate(int numero) {
	_disegni = numero;
}


/******************************************************************************/
/******************************************************* CONFIG TIPO COMMI ****/
/******************************************************************************/
tipoCommi configTipoCommi() {
	return _tipoCommi;
}

void configSetTipoCommi(tipoCommi tipo) {
	_tipoCommi = tipo;
}


/******************************************************************************/
/******************************************************* CONFIG TIPO INPUT ****/
/******************************************************************************/
void configSetTipoInput(tipoInput input) {
	_tipoInput = input;
}

tipoInput configTipoInput() {
	return _tipoInput;
}


/******************************************************************************/
/************************************************* CONFIG ARTICOLATO VUOTO ****/
/******************************************************************************/
char *configArticolatoVuoto() {
	return _configArticolatoVuoto;
}

void configSetArticolatoVuoto(char *art) {
	strncpy(_configArticolatoVuoto, art, MAXSTRINGA);
	_configArticolatoVuoto[MAXSTRINGA] = 0;

}

char *configDescrittoriVuoto() {
	return _configDescrittoriVuoto;
}


/******************************************************************************/
/********************************************************* CONFIG RUBRICHE ****/
/******************************************************************************/
int configGetRubriche() {  // 0 allora articoli senza rubrica
	return _rubriche;
}

void configSetRubriche(int sn) {
	_rubriche = sn;
}


/******************************************************************************/
/*************************************************** CONFIG SEQUENZA CHECK ****/
/******************************************************************************/
int configSequenzaCheck() {
	return _configSequenzaCheck;
}

void configSetSequenzaCheck(int s) {
	_configSequenzaCheck = s;
}


/******************************************************************************/
/****************************************************** CONFIG TESTA VUOTA ****/
/******************************************************************************/
char *configTestaVuota(tipoDTD tipo) {
	switch (tipo) {
		case base:		return _configTestaVuotaBase;
		case completo:		return _configTestaVuotaCompleto;
		case flessibile:	return _configTestaVuotaFlessibile;
		case dl:			return _configDTDDlStringa;  //stesso valore di ritorno
		//case cnr:			return _configDTDCnrStringa; //di configGetDTDTipoStringa()... 
	}
}


/******************************************************************************/
/******************************************************* CONFIG CODA VUOTA ****/
/******************************************************************************/
char *configCodaVuota(tipoDTD tipo) {
	switch (tipo) {
		case base:		return _configCodaVuotaBase;
		case completo:		return _configCodaVuotaCompleto;
		case flessibile:	return _configCodaVuotaFlessibile;
	}
}


/******************************************************************************/
/********************************************************* CONFIG ENCODING ****/
/******************************************************************************/
char *configEncoding() {
	return _configEncoding;
}

void configSetEncoding(char *enc) {
	strncpy(_configEncoding, enc, MAXSTRINGA);
	_configEncoding[MAXSTRINGA] = 0;
}

