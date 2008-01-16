/******************************************************************************
* Project:	xmLeges
* Module:	Marker
* File:		tag.h
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Mirco Taddei (m.taddei@ittig.cnr.it)
******************************************************************************/
#ifndef __TAG_H__
#define __TAG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/tree.h>
#include <IttigUtil.h>

typedef enum ParserStruttura_tagTipo {
	nir=0,
	annessi,
	annesso,
	_TipoDocumento,
	articolato,
	libro,
	parte,
	titolo,
	capo,
	sezione,
	articolo,
	comma,
	lettera,
	numero,
	puntata,
	num,
	corpo,
	alinea,
	rubrica,
	tipodoc,
	datadoc,
	numdoc,
	titolodoc,
	formulainiziale,
	formulafinale,
	dataeluogo,
	sottoscrizioni,
	pubblicazione,
	emanante,
	intestazione,
	conclusione,
	varie,
	sottoscrivente,
	visto,
	sconosciuto,
	preambolo,
	registrazione,
	nota,
	lavoripreparatori,
	mod,
	virgolette,
	datapubbl,
	numpubbl,
	sopubbl,
	contenitore,
	tagerrore,		
	testata,		//TagErrore ha priorit� su questo tag
	preannesso,		//TagErrore ha priorit� su questo tag
	titannesso,		//TagErrore ha priorit� su questo tag
	denannesso,		//TagErrore ha priorit� su questo tag
	ndr,
	h_p,
	h_table,
	h_tr,
	h_td,
	h_br,
	decorazione,
	tiporango
} tagTipo;

#define TAGTIPODIM 58 //Numero totale tag

typedef struct {
	int num;
	int lat;
} tagNum;


/*typedef struct {
	int inizio;
	int fine;
	tagTipo tipo;
	tagNum num;
	int lenPattern;
} tag;
*/
//int GetAllTagsbyTagTipo(tag *bufAllegati, tagTipo ptipo);

//tag *tagInit(tagTipo tipo, int inizio, int fine, int num, int lat, int lenPattern);
//void tagInserisci(tag *t, int pos);
//void tagAppendi(tag * t);
//void tagCancella(int n);
//tag *tagClona(tag *t, tagTipo tipo);
//tag *tagClonaFinePattern(tag *t, tagTipo tipo);
//char *tagStringa(tag *t);
char *tagTipoToNome(tagTipo t);
char *tagTipoToNomeID(tagTipo t);
char *tagNumStringa(tagNum *t);
//char *tagApri(tag *t);
//char *tagChiudi(tag *t);
//void tagApertiAzzera(void);

//void tagSort(void);
//tag *tagFineSort(void);
//tag * tagNumero(int i);
//int tagN(void);

//void tagChiusura(void);
//void tagAlinee();
//void tagControlloArticoloInizioEl();
//void tagSetFine(int fine);

#ifdef __cplusplus
}
#endif

#endif /* __TAG_H__ */