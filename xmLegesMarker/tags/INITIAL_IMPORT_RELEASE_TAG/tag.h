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
	testata,		//TagErrore ha priorità su questo tag
	preannesso,		//TagErrore ha priorità su questo tag
	titannesso,		//TagErrore ha priorità su questo tag
	denannesso,		//TagErrore ha priorità su questo tag
	ndr,
	h_p,
	h_table,
	h_tr,
	h_td,
	h_br,
	
} tagTipo;

#define TAGTIPODIM 55 //I tagTipo sono in tutto 55

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
