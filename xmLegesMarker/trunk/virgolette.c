
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

/*
 * <sostituzione> (03/03/06)
 * --> true anche se non contiene nodi di tipo 'virgolette' solo
 *  nei casi -abrogazione- e -soppressione-
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
				virgoletteInCorpo(corpo) )
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

    //DA FARE: se non si inserisce il mod???? che diavolo deve fare???
	//deve convertire tutte le virgolette in ERROR?
	
	xmlNodePtr cur=pNodoCorpo->children;//firstVirgo=GetFirstNodebyTagTipo(pNodoCorpo,BAD_CAST tagTipoToNome(virgolette));
	
	
	int areInMod=0;
	char *desc=(char *)xmlNodeListGetRawString(NULL,pNodoCorpo->children,1);
	if(checkCommaMod(desc, pNodoCorpo))
	{
		IDMOD++;
		xmlNodePtr newNodoMod=xmlNewNode(NULL, BAD_CAST tagTipoToNome(mod));
		xxxSetIDtoNode(newNodoMod, mod,IDMOD,0,NULL);
		//Sposta tutti i figli del CORPO nel nuovo nodo MOD
		MoveAllChildren(pNodoCorpo,newNodoMod);
				
		//Aggancia il MOD al CORPO
		//Prossima linea: SegFault se si utilizzano liste testo/entità per il testo
		//e sono presenti entità (testo html)
		xmlAddChild(pNodoCorpo,newNodoMod); 
		areInMod=1;
	}

	if (!areInMod)
	while (cur != NULL) {
		 if (IsNode(cur,virgolette) ){
			//se è presente una VIRGOLETTA
			//nel caso in cui NON sia un comma di modifica sostituisce VIRGOLETTE con ERRORE
			//Crea un nodo errore PRIMA

			//Messaggio di verifica temporaneamente tolto:
			//xmlNodePtr newNodoErrPre=xmlNewNode(NULL, BAD_CAST tagTipoToNome(tagerrore));
			//xmlAddChild(newNodoErrPre,xmlNewText("--- Partizione con virgolette: controllare se modificativa ---"));
			
			//xmlNodePtr newNodoErrAfter=xmlNewNode(NULL, BAD_CAST tagTipoToNome(tagerrore));
			xmlChar *allText=xmlNodeListGetRawString(NULL,cur->children,1);
			xmlNodePtr newNodoTxt=xmlNewText(allText);
			xmlReplaceNode(cur,newNodoTxt);
			
			//Messaggio di verifica temporaneamente tolto:
			//xmlAddPrevSibling(newNodoTxt,newNodoErrPre);
			
			//xmlAddNextSibling(newNodoTxt,newNodoErrAfter);
			
			cur=newNodoTxt;
			//DA FARE : rimuovere i vecchio nodo VIRGOLETTE
			
			//Virgolette2Errore(pNodoCorpo);
			}
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


