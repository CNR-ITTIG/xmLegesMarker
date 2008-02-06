/******************************************************************************
* Project:	xmLeges
* Module:	Marker
* File:		config.h
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Mirco Taddei (m.taddei@ittig.cnr.it)
******************************************************************************/
#ifndef __CONFIG_H__
#define __CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "costanti.h"
#include "util.h"
#include <IttigUtil.h>
#include <IttigLogger.h>

typedef enum _tipoModulo{
	mod_tabelle=0,
	mod_annessi,
}tipoModulo;

typedef enum _tipoTesto {
	legge,
	leggecostituzionale,
	decretolegge,
	decretolegislativo,
	regiodecreto,
	decretopresidenterepubblica,
	decretopresidenterepubblicaNN,
	decretopresedenteconsiglioministri,
	decretopresedenteconsiglioministriNN,
	decretoministeriale,
	decretoministerialeNN,
	leggeregionale,
	attoauthority,
	comunicato,
	disegnolegge,
	provCNR,
	unknown,
	documentoNIR,
	statutocomunale,
	regolamentocomunale,
	deliberacomunale
} tipoTesto;

typedef enum _tipoNome {
	circolare,
	regolamento,
	provvedimento,
	regolamentoregionale
} tipoNome;

typedef enum _tipoStruttura {
	docarticolato,
	docsemiarticolato
} tipoStruttura;

typedef enum _tipoRubrica {
	acapo,
	adiacente,
	deagostini
} tipoRubrica;

typedef enum _tipoDTD {
	base,
	completo,
	flessibile,
	dl,
	cnr
} tipoDTD;


typedef enum _tipoInput {
	txt,
	html,
	doc,
	ted
} tipoInput;

typedef enum _tipoCommi {
	commiNumerati,
	commiNNLineeVuote,   //CommiNN1
	commiNNSenzaLinea    //CommiNN2
} tipoCommi;

//Disattivazione Moduli
void	configSetDisable(tipoModulo pmodulo);
int		configIsEnabled(tipoModulo pmodulo);

//TESTA
char *configGetTestaDefault(int i);
char *configGetCodaDefault(int i);

void	configSetNodeCount(int a);
int		configGetNodeCount(void);

void	configConfigInLog(void);

int		configLeggi(char *fn);

char *configDirTmp(void);
//LOG
//char *configLogFile(void);
//void configSetLogDisable(tipoLogDisable t);
//int configGetLogDisable(tipoLogDisable t);

//Testo Documento
void configSetDocTesto(tipoTesto t);
tipoTesto configGetDocTestoTipo(void);
char *configGetDocTestoString(void);

//Nome Documento
void configSetDocNome(char *docNome);
char *configGetDocNome(void);

//Testate DDL
int  configDdlTestate(void);
void configSetDdlTestate(int numero);

//Struttura del documento
void configSetDocStruttura(tipoStruttura t);
tipoStruttura configGetDocStruttura(void);

char *configEncoding();
void configSetEncoding(char *enc);

void configSetDTDTipo(tipoDTD tipo);
tipoDTD	configGetDTDTipo(void);
char *configGetDTDTipoStringa(void);

//char *configDTDStringa(tipoDTD tipo);
//char *configDTDMeta(tipoDTD tipo);

char *configHeaderParser();
void configSetHeaderParser(char *parser);
char *configHeaderParserModels();
void configSetHeaderParserModels(char *);

tipoInput configTipoInput(); // E' un GetTipoInput()
void configSetTipoInput(tipoInput input);

tipoCommi configTipoCommi();
void configSetTipoCommi(tipoCommi tipo);

char *configArticolatoVuoto();
void configSetArticolatoVuoto(char *art);
char *configDescrittoriVuoto();

//Presenza di Rubrica
int configGetRubriche();
void configSetRubriche(int sn);

//Tipo Rubrica
tipoRubrica configGetRubricaTipo(void);
void	configSetRubricaTipo(tipoRubrica liv);

int configSequenzaCheck();
void configSetSequenzaCheck(int s);

char *configTestaVuota(tipoDTD tipo);
char *configCodaVuota(tipoDTD tipo);

#ifdef __cplusplus
}
#endif

#endif
