
#include "virgolette.h"
/******************************************************************************/
/******************************************************* VIRGOLETTE IN MOD ****/
/******************************************************************************/

static int IDMOD=0;

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
	if(checkCommaMod(desc))
	{
		IDMOD++;
		xmlNodePtr newNodoMod=xmlNewNode(NULL, BAD_CAST tagTipoToNome(mod));
		xxxSetIDtoNode(newNodoMod, mod,IDMOD,0,NULL);
		//Sposta tutti i figli del CORPO nel nuovo nodo MOD
		MoveAllChildren(pNodoCorpo,newNodoMod);
		//Aggancia il MOD al CORPO
		xmlAddChild(pNodoCorpo,newNodoMod);
		areInMod=1;
	}

	if (!areInMod)
	while (cur != NULL) {
		 if (IsNode(cur,virgolette) ){
			//se è presente una VIRGOLETTA
			//nel caso in cui NON sia un comma di modifica sostituisce VIRGOLETTE con ERRORE
			//Crea un nodo errore PRIMA
			xmlNodePtr newNodoErrPre=xmlNewNode(NULL, BAD_CAST tagTipoToNome(tagerrore));
			xmlAddChild(newNodoErrPre,xmlNewText("--- Partizione con virgolette: controllare se modificativa ---"));
			//xmlNodePtr newNodoErrAfter=xmlNewNode(NULL, BAD_CAST tagTipoToNome(tagerrore));

			xmlChar *allText=xmlNodeListGetRawString(NULL,cur->children,1);
			xmlNodePtr newNodoTxt=xmlNewText(allText);
			xmlReplaceNode(cur,newNodoTxt);
			xmlAddPrevSibling(newNodoTxt,newNodoErrPre);
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


