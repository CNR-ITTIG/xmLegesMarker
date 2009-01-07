/******************************************************************************
* Project:	xmLeges
* Module:	Marker
* File:		virgolette.c
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Mirco Taddei (m.taddei@ittig.cnr.it)
* 			Lorenzo Bacci (bacci@ittig.cnr.it)
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
// Sostituzione "manuale" di un nodo (sostituisce "a" con "b")
void replaceNode(xmlNodePtr a, xmlNodePtr b, xmlNodePtr p) {
//Non è detto che se a->prev è NULL, a->parent non sia NULL (virgolette ha entrambi NULL)
// => utilizzo l'argomento 'p' (è il vero nodo precedente a virgolette, NULL se primo della lista)

	//xmlNodePtr prev=NULL;
	xmlNodePtr next=NULL, parent=NULL, children=NULL, last=NULL;

	if(a==NULL || b==NULL) return;
//	if(a->prev!=NULL) //vedi nota sopra.
//		prev=a->prev;
	if(a->next!=NULL)
		next=a->next;
	if(a->parent!=NULL)
		parent=a->parent;
	if(a->children!=NULL)
		children=a->children;
	if(a->last!=NULL)
		last=a->children;

	b->parent=parent;
	b->next=next;
	b->children=children;
	b->last=last;
	if(p!=NULL) // && p->next==a ??
		p->next=b;
	//else
	//	parent->children=b;
	if(parent!=NULL && parent->children==a)
		parent->children=b;

	//si deve anche considerare l'a->next->prev...
	if(next!=NULL && next->prev!=NULL)
		next->prev=b;

	//parent->children->last
	if(parent !=NULL && parent->children!=NULL
		&& parent->children->last!=NULL
		&& parent->children->last==a)
		parent->children->last=b;

	//children->parent ?
	if(children!=NULL && children->parent==a)
		children->parent=b;
}

//Sostituisce b ad a, ritorna a con tutti i suoi figli (non vanno sotto b)
xmlNodePtr replaceNodeWithChildren(xmlNodePtr a, xmlNodePtr b, xmlNodePtr p) {

	xmlNodePtr next=NULL, parent=NULL, last=NULL;

	if(a==NULL || b==NULL) return NULL;
	if(a->next!=NULL)
		next=a->next;
	if(a->parent!=NULL)
		parent=a->parent;
	if(a->last!=NULL)
		last=a->last;

	b->parent=parent;
	b->next=next;
	b->last=last;
	if(p!=NULL)
		p->next=b;
	//else
	//	parent->children=b;
	if(parent->children==a)
		parent->children=b;

	//(si deve anche considerare l'a->next->prev che adesso punta ancora a 'b' non ad 'a'!!)
	if(next!=NULL && next->prev!=NULL)
		next->prev=b;

	//parent->children->last
	if(parent!=NULL && parent->children!=NULL
		&& parent->children->last!=NULL
		&& parent->children->last==a)
		parent->children->last=b;

	//Ritorna a
	a->next=NULL;
	a->prev=NULL;
	a->parent=NULL;
	a->last=NULL;
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
/*
 * Controlla se il comma è "modificativo".
 */
int checkCommaMod(char *txt, xmlNodePtr corpo) {
	/*
	 * Per essere "modificativo" deve contenere le parole chiave
	 * della "soppressione", oppure deve contenere le parole chiave
	 * dell'"inserimento" (o "sostituzione") E contenere almeno un
	 * elemento VIRGOLETTE.
	 */
	if (txt==NULL) return 0;

	//Compara con caratteri minuscoli!
	utilStringToLower(txt);

	//if (strstr(txt, "soppress") || strstr(txt, "abrogat") ) <-- "soppressione" solitamente non indica una mod...
	if (strstr(txt, "soppresso") || strstr(txt, "soppressa") || strstr(txt, "abrogat") ||
			strstr(txt, "soppressi") || strstr(txt, "soppresse") )
		return 1;

	//Aggiunto caso: "sono premessi i seguenti..." DDL 3328 art.34
	//Devono anche essere presenti delle virgolette nel corpo
	if ( (strstr(txt, "sostituit") || strstr(txt, "aggiunt") ||
			strstr(txt, "modificat") ||	strstr(txt, "inserit")  ||	strstr(txt, "premess") ) &&
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
	xmlNodePtr cur=pNodoCorpo->children;//firstVirgo=GetFirstNodebyTagTipo(pNodoCorpo,BAD_CAST tagTipoToNome(virgolette));

	int areInMod=0;
	//char *desc=(char *)xmlNodeListGetRawString(NULL,pNodoCorpo->children,0);
	char *desc=(char *)xmlNodeListGetRawString(NULL,cur,0);

	//printf("\nPossibile MOD:\n%s\n", desc);

	if(checkCommaMod(desc,pNodoCorpo))
	{
		//printf("\nIdentified MOD:\n%s\n", desc);
		IDMOD++;
		xmlNodePtr newNodoMod = xmlNewNode(NULL, BAD_CAST tagTipoToNome(mod));
		domSetIDtoNode(newNodoMod,mod,IDMOD,0,NULL);

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
			replaceNode(cur,newNodoTxt,prevnode);

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

	//Try to analyze virgolette content
	xmlNodePtr modNode = GetFirstNodebyTagTipo(pNodoCorpo, BAD_CAST "mod"); //Può esserci soltanto un mod per comma!?
	if(modNode == NULL) {
		//printf("\n ERROR ! ModificaVirgolette() - modNode is NULL! name: %s \n", pNodoCorpo->children->name);
		return;
	}

	//xmlNodePtr virgoNode = GetFirstNodebyTagTipo(modNode, BAD_CAST "virgolette"); //possono esserci più virgo nello stesso mod

	xmlNodePtr cNode = modNode->children;
	while (cNode != NULL) {
		if (!xmlStrcmp(cNode->name, BAD_CAST "virgolette")) {
			//Analizza struttura interna di queste virgolette
			virgoletteStructure(cNode);
		}
		cNode = cNode->next;
	}

}

//RICORSIVA
void virgoletteInMod(xmlNodePtr pParentNode)
{
	if (pParentNode==NULL) return ;


	xmlNodePtr cur= pParentNode->children;	//FirstChild

	while (cur != NULL) {
		//Se il nodo corrente è una CORPO, cerca di individuare le VIRGOLETTE al suo interno e vi inserisce un MOD
		if (IsNode(cur,corpo)){
			ModificaVirgolette(cur);
		} else {
			//prosegue la ricorsione solo se non è un CORPO
			virgoletteInMod(cur);
		}
		cur = cur->next; //NextChild
	}

}

void virgoletteStructure(xmlNodePtr virgoNode) {

	if(virgoNode == NULL) {
		//printf("\n ERROR! ModificaVirgolette() - virgoNode is null! \n");  // <-- caso ABROGATIVA ??
		return;
	}

	//char *virgoContent = (char *) xmlNodeListGetRawString(NULL, virgoNode->children, 1); //Enc sbagliato con Legge080598
	char *virgoContent = (char *) xmlNodeListGetString(NULL, virgoNode->children, 0);  //Così è OK con Legge080598...

	if(strlen(virgoContent) < 64) {
		//Input too short, that should be a text virgo...
		return;
	}

	xmlNodePtr resNode = ArticolatoAnalizzaVirgo(virgoContent);

	//xmlNodePtr resNode = virgoNode;

	if(resNode == NULL) {

			//resNode is null means articolato.lex didn't found any 'articolato' ! <-- Tipo= PAROLA...
			//printf("\n ModificaVirgolette() - resNode is null! buffer:\n%s\n", virgoContent);

			xmlNewProp(virgoNode, BAD_CAST "tipo", BAD_CAST "parola");
	} else {

		//Articolato found, there can be error tag too

		//printf("\n ModificaVirgolette() - resNode is NOT NULL! buffer:\n%s\n", virgoContent);

		xmlNodePtr mFirstError = GetFirstNodebyTagTipo(resNode, BAD_CAST tagTipoToNome(tagerrore));

		int fullMarked = 1;
		if(mFirstError != NULL) { //Ce ne può essere più di uno di nodi errore?

			//Check if error is empty, else discard resNode or just warn??
			char *errContent = (char *) xmlNodeGetContent(mFirstError);
			errContent = trim(errContent);
			if(strlen(errContent) > 0) {
				//printf("\nWARNING!! Non empty error in resNode ! \nvirgoContent:%s\n errorContent:\n%s\n", virgoContent, errContent);
				//printf("\nDo Nothing / Skipping...\n");
				fullMarked = 0;
				//return;
			} else {
				//Delete empty error node
				xmlUnlinkNode(mFirstError);
				xmlFreeNode(mFirstError);
			}

		} else {

			//printf("\nNo Error in resNode - name:%s\n", resNode->name);
		}

		//printf("\nAdding resNode to virgolette - name:%s\n", resNode->name);

		//Very ugly and dangerous operation, unlinks actual childs and replace them with resNode
		DeleteAllChildren(virgoNode);
		xmlAddChild(virgoNode, resNode);

		if(!fullMarked) {
			xmlNodePtr warning = xmlNewPI(BAD_CAST tagTipoToNome(tagerrore), BAD_CAST "Marker Warning 106");
			xmlAddSibling(virgoNode, warning);
		}

		xmlNewProp(virgoNode, BAD_CAST "tipo", BAD_CAST "struttura");

		if(IsNode(resNode, articolato)) {
			MoveAllChildren(resNode, virgoNode);
			xmlUnlinkNode(resNode);
			xmlFreeNode(resNode);

		} else {
			printf("\nERROR - resNode is not articolato: %s\n", resNode->name);
		}
	}
}

