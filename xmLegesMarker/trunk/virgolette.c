/******************************************************************************
* Project:	xmLeges
* Module:	Marker
* File:		virgolette.c
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Mirco Taddei (m.taddei@ittig.cnr.it)
* 			Lorenzo Bacci (lorenzobacci@gmail.com)
******************************************************************************/
#include "virgolette.h"
/******************************************************************************/
/******************************************************* VIRGOLETTE IN MOD ****/
/******************************************************************************/

static int IDMOD=0;

//Aggiunta (03/03/06)
//-->true se è presente almeno un nodo di tipo 'virgolette' in corpo
int virgoletteInCorpo(xmlNodePtr corpo) {
	if(corpo==NULL) return 0;
	xmlNodePtr cur = corpo->children;

	while(cur!=NULL) {
		if(IsNode(cur, virgolette))
			return 1;
		cur=cur->next;	
	}	
	return 0;
}

// Aggiunta:
// Sostituzione "manuale" di un nodo
void replaceNode(xmlNodePtr a, xmlNodePtr b, xmlNodePtr p) {
//Non è detto che se a->prev è NULL, a->parent non sia NULL (virgolette ha entrambi NULL)
// => utilizzo l'argomento 'p' (è il vero nodo precedente a virgolette, NULL se primo della lista)

	//xmlNodePtr prev=NULL;
	xmlNodePtr next=NULL, parent=NULL, children=NULL;
	
	if(a==NULL || b==NULL) return;
	//if(a->prev!=NULL)
		//prev=a->prev;
	if(a->next!=NULL)
		next=a->next;
	if(a->parent!=NULL)
		parent=a->parent;
	if(a->children!=NULL)
		children=a->children;
		
	b->parent=parent;
	b->next=next;
	b->children=children;
	if(p!=NULL)
		p->next=b;
	else
		parent->children=b;
	
	//(si deve anche considerare l'a->next->prev che adesso punta ancora a 'b' non ad 'a'!!)
	if(next!=NULL && next->prev!=NULL)
		next->prev=b;
}

//Sostituisce b ad a, ritorna a con tutti i suoi figli (non vanno sotto b)
xmlNodePtr replaceNodeWithChildren(xmlNodePtr a, xmlNodePtr b, xmlNodePtr p) {
	xmlNodePtr next=NULL, parent=NULL;
	
	if(a==NULL || b==NULL) return;
	if(a->next!=NULL)
		next=a->next;
	if(a->parent!=NULL)
		parent=a->parent;
		
	b->parent=parent;
	b->next=next;
	if(p!=NULL)
		p->next=b;
	else
		parent->children=b;

	//(si deve anche considerare l'a->next->prev che adesso punta ancora a 'b' non ad 'a'!!)
	if(next!=NULL && next->prev!=NULL)
		next->prev=b;

	//Ritorna a
	a->next=NULL;
	a->prev=NULL;
	a->parent=NULL;
	return a;	
}

/*
 * <sostituzione> (03/03/06)
 * --> true anche se non contiene nodi di tipo 'virgolette' solo
 *  nel caso - abrogazione/soppressione -
 */ 
/*
int checkCommaMod(char *txt) {
	int ret = 0;
	if (txt!=NULL){
		if (strstr(txt, "sostituit") ||
			strstr(txt, "aggiunt") ||
			strstr(txt, "modificat") ||
			strstr(txt, "inserit") ||
			strstr(txt, "soppress") ||
			strstr(txt, "abrogat")
		) ret = 1;
	}
	return ret;
}
*/
int checkCommaMod(char *txt, xmlNodePtr corpo) {
	if (txt==NULL) return 0;
	
	//if (strstr(txt, "soppress") || strstr(txt, "abrogat") ) <-- "soppressione" solitamente non indica una mod...
	if (strstr(txt, "soppresso") || strstr(txt, "soppressa") || strstr(txt, "abrogat") ||
			strstr(txt, "soppressi") || strstr(txt, "soppresse") )
		return 1;
		
	if ( (strstr(txt, "sostituit") || strstr(txt, "aggiunt") ||
			strstr(txt, "modificat") ||	strstr(txt, "inserit") ) &&
				virgoletteInCorpo(corpo) ) //Devono anche essere presenti delle virgolette nel corpo
		return 1;

	return 0;
}
//</sostituzione>

// NON RICORSIVA
void Virgolette2Errore(xmlNodePtr pParentNode){

	//Passo base
	if (pParentNode==NULL)return;

	xmlNodePtr cur=pParentNode->children; //FirstChild
	
	while (cur != NULL) {
		if (IsNode(cur,virgolette)){
				xmlNodeSetName(cur,BAD_CAST tagTipoToNome(tagerrore));
		}
		cur = cur->next; //next fratello
	}
	
}

void ModificaVirgolette(xmlNodePtr pNodoCorpo)
{
	xmlNodePtr cur=pNodoCorpo->children;//firstVirgo=GetFirstNodebyTagTipo(pNodoCorpo,BAD_CAST tagTipoToNome(virgolette));	
	
	int areInMod=0;
	char *desc=(char *)xmlNodeListGetRawString(NULL,pNodoCorpo->children,0);
	if(checkCommaMod(desc, pNodoCorpo))
	{
		IDMOD++;
		xmlNodePtr newNodoMod=xmlNewNode(NULL, BAD_CAST tagTipoToNome(mod));
		domSetIDtoNode(newNodoMod, mod,IDMOD,0,NULL);
		//Sposta tutti i figli del CORPO nel nuovo nodo MOD
		MoveAllChildren(pNodoCorpo,newNodoMod);
				
		//Aggancia il MOD al CORPO
		//xmlAddChild(pNodoCorpo,newNodoMod); //<--??rimangono eventuali nodi (uno solo?) di testo già presenti sotto 'corpo'
		pNodoCorpo->children=newNodoMod; //sostituisce "manualmente" il contenuto di 'corpo'
		
		areInMod=1;
	}

	//Togli il tag 'virgolette' se non si trova all'interno di un 'mod'
	xmlNodePtr prevnode = NULL; //il "vero nodo precedente"
	if (!areInMod)
	while (cur != NULL) {
		 if (IsNode(cur,virgolette) ){
			//se è presente una VIRGOLETTA
			//nel caso in cui NON sia un comma di modifica sostituisce VIRGOLETTE con ERRORE
			//Crea un nodo errore PRIMA

			//--- Messaggio di verifica temporaneamente tolto: ---
			//xmlNodePtr newNodoErrPre=xmlNewNode(NULL, BAD_CAST tagTipoToNome(tagerrore));
			//xmlAddChild(newNodoErrPre,xmlNewText("--- Partizione con virgolette: controllare se modificativa ---"));
			//xmlNodePtr newNodoErrAfter=xmlNewNode(NULL, BAD_CAST tagTipoToNome(tagerrore));
			
			xmlChar *allText=xmlNodeListGetRawString(NULL,cur->children,1);
			//La riga precedente potrebbe aver trasformato entità in caratteri non validi (> <), esegui sstring():
			//xmlNodePtr newNodoTxt=xmlNewText(sstring((char *)allText));
			xmlNodePtr newNodoTxt=xmlNewText(allText);
			
			//xmlReplaceNode(cur,newNodoTxt);
			replaceNode(cur,newNodoTxt, prevnode);
			
			//--- Messaggio di verifica temporaneamente tolto: ---
			//xmlAddPrevSibling(newNodoTxt,newNodoErrPre);
			//xmlAddNextSibling(newNodoTxt,newNodoErrAfter);
			
			cur=newNodoTxt;
			//DA FARE : rimuovere il vecchio nodo VIRGOLETTE  // <--ok
			
			//Virgolette2Errore(pNodoCorpo);
			}
		prevnode = cur;
		cur=cur->next;
	}
	
}

//RICORSIVA
void virgoletteInMod(xmlNodePtr pParentNode)
{
	if (pParentNode==NULL)return ;

	xmlNodePtr cur= pParentNode->children;	//FirstChild

	while (cur != NULL) {
		//Se il nodo corrente è una CORPO, cerca di individuare le VIRGOLETTE al suo interno e vi inserisce un MOD
		if (IsNode(cur,corpo)){
			ModificaVirgolette(cur);
		}else
		{
			//prosegue la ricorsione solo se non è un CORPO
			virgoletteInMod(cur);
		}
		cur = cur->next; //NextChild
	}
}


