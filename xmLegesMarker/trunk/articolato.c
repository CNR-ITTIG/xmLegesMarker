/******************************************************************************
* Project:	xmLeges
* Module:	Marker
* File:		articolato.c
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Mirco Taddei (m.taddei@ittig.cnr.it)
******************************************************************************/
#include "articolato.h"
//extern int flagAnnessi;


/******************************************************************************/
/******************************************************** STRUTTURA BUFFER ****/
/******************************************************************************/
xmlNodePtr ArticolatoAnalizza(char *testo) {
	char *testoIso=NULL;
	size_t lin,lout;
	//extern int flagAnnessi;
	int flagAnn=0;


	//pAbsoluteIndexStart serve per riposizionare il Pos del Lex nel giusto offset
	int ret=0;
	loggerInfo("INIZIO Articolato");
	
	//Creazione del nodo ARTICOLATO
	xmlNodePtr mNodoArticolato=xmlNewNode(NULL, BAD_CAST tagTipoToNome(articolato));

	flagAnn = GetFlagAnnessi();
	
	//printf("\nann:%d\n", flagAnn);
	//printf("\nTESTO:\n%s\n", testo);
	
	//Se il flag è 1 allora ci sono degli annessi, quindi è necessaria la conversione del testo in iso
	if (flagAnn!=0)
	{	
		//testoIso = utilConvTextToIso(testo);  //a che serve qui?
		testoIso = testo;
		//Qui alla variabile globale xxxTextBuffer verrà assegnato il testo nel formato originario
		domInit(articolato,mNodoArticolato,testoIso);
		ret=_ArticolatoLexStart(testoIso); //IL TESTO CHE VIENE PASSATO E' GIA' IN UTF-8!!!
	}
	else 
	{
		domInit(articolato,mNodoArticolato,testo);
		ret=_ArticolatoLexStart(testo); //IL TESTO CHE VIENE PASSATO NON E' UTF-8
	}

	sequenzeClear();	//Inizializzazione di tutte le Sequenze
	
	//avvio scansione articolato.lex *************************************************
	/*puts("-------------------------------------------------------------------");
	puts(testo);*/

	//testoIso = utilConvTextToIso(testo);
	
	/*lin=strlen(testo);
	lout=lin*2;
	testoIso=( char *)malloc(lout);
	memset( testoIso, 0, lout );
	UTF8Toisolat1(testoIso,lout,testo,lin);*/
	
	/*puts("-------------------------In ISO-----------------------------");
	puts(testoIso);*/

	domClose();

	if (ret)		// ------------------------------------ caso di Documento Articolato
	{	
	   loggerInfo("INIZIO cercaCommiVuoti");
		cercaCommiVuoti(mNodoArticolato);
		loggerInfo("FINE cercaCommiVuoti");

		loggerInfo("INIZIO cercaArticoliVuoti");
		cercaArticoliVuoti(mNodoArticolato);
		loggerInfo("FINE cercaArticoliVuoti");
	}
	else			// ------------------------------------ caso di Documento NON Articolato
	{	
		utilNodeDelete(mNodoArticolato);
		mNodoArticolato=NULL;
	}

	loggerInfo("FINE Articolato");
	utilPercCalc(4);

	utilPercCalc(2);

	return mNodoArticolato;
}


/******************************************************************************/
/**************************************************** CERCA ARTICOLI VUOTI ****/
/******************************************************************************/

//Cerca se esiste almeno un Comma, altrimenti lo crea
void adjustArticolo(xmlNodePtr pNodoArticolo)
{
	xmlNodePtr	nComma=GetFirstNodebyTagTipo(pNodoArticolo,tagTipoToNome(comma));
	xmlNodePtr  newComma;

	if(!nComma)
	{
		newComma=xmlNewNode(NULL, BAD_CAST tagTipoToNome(comma));
		xmlAddChild(pNodoArticolo,newComma);
		domSetIDtoNode(newComma,comma,1,0,NULL);

		xmlAddChild(newComma,xmlNewNode(NULL, BAD_CAST tagTipoToNome(num)));
		xmlAddChild(newComma,xmlNewNode(NULL, BAD_CAST tagTipoToNome(corpo)));
	}
}
//RICORSIVA
void cercaArticoliVuoti(xmlNodePtr pArticolato) {
	if (pArticolato==NULL)return;

	xmlNodePtr cur = pArticolato->xmlChildrenNode;	//FirstChild

	while (cur != NULL) {
		if (IsNode(cur,articolo))	{
			adjustArticolo(cur);
		}
		else{
			cercaArticoliVuoti(cur);
		}
		cur = cur->next; //next fratello
	}

}

int IsEmptyString(char *str)
{
	int n,ret=1;
	if (str==NULL)return ret;

	for (n=0;n<strlen(str);n++){
		if ((str[n]!=' ')&&(str[n]!='\t')&&(str[n]!='\n')&&(str[n]!='\r')){
			ret=0;
			break;
		}
	}
	//if (ret ==1 )	printf("stringa : '%s'\n",str);
	return ret;
}

// RICORSIVA
// viene considerata PIENA solo se i NODI TESTO contengono testo
int AreAllNodesEmpty(xmlNodePtr pNodo){
	
	int ret=1;
	if (pNodo==NULL)return ret;
	if (xmlNodeIsText(pNodo))
		if(!IsEmptyString(pNodo->content))ret=0;

	xmlNodePtr cur= pNodo->xmlChildrenNode;	//FirstChild
	while (cur != NULL) {
		ret= ret & AreAllNodesEmpty(cur);
		cur = cur->next; //next fratello
	}
	return ret;
}

//RICORSIVA
//Rimuove i commi->num:corpo vuoti che non contengo testo
void cercaCommiVuoti(xmlNodePtr pNodo) {
	if (pNodo==NULL)return;
	
	xmlNodePtr tmp=NULL;
	xmlNodePtr cur= pNodo->xmlChildrenNode;	//FirstChild

	while (cur != NULL) {
		if (IsNode(cur,comma))	{
			if (AreAllNodesEmpty(cur)){
				tmp=cur;
				//Riaggiorno cur prima di eliminare il nodo
				cur = cur->next; //next fratello

				utilNodeDelete(tmp);
				loggerDebug("Eliminato Comma Vuoto");
			}else
			{
				cur = cur->next; //next fratello
				cercaCommiVuoti(cur);
			}
		}
		else{
			cercaCommiVuoti(cur);
			cur = cur->next; //next fratello
		}
		
	}

}


