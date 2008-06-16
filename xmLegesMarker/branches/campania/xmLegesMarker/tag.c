/******************************************************************************
* Project:	xmLeges
* Module:	Marker
* File:		tag.c
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Mirco Taddei (m.taddei@ittig.cnr.it)
******************************************************************************/
#include "tag.h"


/******************************************************************************/
/******************************************************** TAG TIPO TO NOME ****/
/******************************************************************************/
char *tagTipoToNome(tagTipo t) {
	switch (t) {
		case nir:			return "NIR";
		case annessi:		return "annessi";
		case annesso:		return "annesso";
		case articolato:	return "articolato";
		case libro:			return "libro";
		case parte:			return "parte";
		case titolo:		return "titolo";
		case capo:			return "capo";
		case sezione:		return "sezione";
		case articolo:		return "articolo";
		case comma:			return "comma";
		case lettera:		return "el";
		case numero:		return "en";
		case puntata:		return "ep";
		case num:			return "num";
		case corpo:			return "corpo";
		case alinea:		return "alinea";
		case rubrica:		return "rubrica";
		case virgolette:	return "virgolette";
		case mod:			return "mod";
		case nota:			return "nota";
		case datadoc:		return "datadoc";
		case tipodoc:		return "tipodoc";
		case numdoc:		return "numdoc";
		case titolodoc:		return "titoloDoc";
		case conclusione:	return "conclusione";
		case intestazione:	return "intestazione";
		case sottoscrizioni:	return "sottoscrizioni";
		case contenitore:			return "contenitore";
		case tagerrore:			return "error";
		case dataeluogo:		return "dataeluogo";
		case formulainiziale:	return "formulainiziale";
		case formulafinale:		return "formulafinale";
		case sottoscrivente:	return "sottoscrivente";
		case lavoripreparatori: return "lavoripreparatori";
		case sconosciuto:	return "sconosciuto";
		case testata:		return "testata";
		case denannesso:	return "denAnnesso";
		case titannesso:	return "titAnnesso";
		case preannesso:	return "preAnnesso";
		case ndr:			return "ndr";
		case h_p:			return "h:p";
		case h_table:		return "h:table";
		case h_tr:			return "h:tr";
		case h_td:			return "h:td";
		case h_br:			return "h:br";
		case _TipoDocumento:	return "DocumentoNonIdentificato";
		case decorazione:	return "decorazione";
		case tiporango:		return "rango";
	}
	return "TipoNonValido";
}



/******************************************************************************/
/******************************************************** TAG TIPO TO NOME ****/
/******************************************************************************/
char *tagTipoToNomeID(tagTipo t) {
	switch (t) {
		case libro:	return "lib";
		case parte:	return "prt";
		case titolo:	return "tit";
		case capo:	return "cap";
		case sezione:	return "sez";
		case articolo:	return "art";
		case comma:	return "com";
		case lettera:	return "let";
		case numero:	return "num";
		case annesso:	return"ann";
		case virgolette:	return"vir";
		case mod:	return"mod";
		case nota:	return"n";
	}
	return "???";
}




/******************************************************************************/
/********************************************************* TAG NUM STRINGA ****/
/******************************************************************************/
char * tagNumStringa(tagNum *t) {
	return utilConcatena(2, utilItoa(t->num), arabicToLatin(t->lat));
}

