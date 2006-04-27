/******************************************************************************
* Project:	xmLeges
* Module:	Marker
* File:		dom.c
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Mirco Taddei (m.taddei@ittig.cnr.it)
* 			Lorenzo Bacci (lorenzobacci@gmail.com)
******************************************************************************/
#include "dom.h"

char		*domTextBuffer;
int			domTextBufferIndex;
xmlNodePtr	mRoot;// = NULL;
xmlNodePtr	mcurrTagState[TAGTIPODIM];

//Aggiunta: ritorna l'ultimo nodo di una lista di "sibling"
xmlNodePtr getLastSibling(xmlNodePtr node) {
	xmlNodePtr tmp = node->next;
	while(tmp!=NULL) {
		node = tmp;
		tmp = tmp->next;
	}
	return node;
}

//Aggiunta: aggiunge un nodo (una lista di nodi) nella lista 'children' di pnode
//Nel caso di testi html aggancia il nodo "manualmente" (non usare xmlAddChild, xmlAddSibling, ecc...)
void addSibling(xmlNodePtr pnode, xmlNodePtr cnode) {
	
	if(configTipoInput()!=html) {
		xmlAddChild(pnode, cnode);
		return;
	}

	if(pnode->children!=NULL) {
		xmlNodePtr last = getLastSibling(pnode->children);

		//xmlAddSibling(last, cnode);	// <-- NON FUNZIONA !!
										// Se sotto 'virgolette' è già presente una lista testo/entità ed almeno
										// un nodo 'h:br', la prossima lista nodo/entità non viene attaccata 
										//correttamente: viene messo solo il primo nodo di testo!
		last->next=cnode;		//<-- facendo "a mano" va tutto come dovrebbe!
								//    (i nodi successivi nella lista sono già attaccati tramite next ma usando
								//    xmlAddSibling vengono ignorati. (Approfondire...)
	}
	else
		xmlAddChild(pnode, cnode);	// <-- xmlAddChild : se era già presente una lista nodo/entità
									//     in pnode->children allora cnode viene attaccato non alla fine della
									//     lista ma dopo il primo nodo (di testo?)  -- (Approfondire...)
}

xmlNodePtr	domGetLastTag(tagTipo ptag)
{
	return mcurrTagState[(int)ptag];
}

void domClearStateFrom(tagTipo ptag)
{
	int n;
	for (n=(int)ptag;n<TAGTIPODIM;n++)
	{
		mcurrTagState[n]=NULL;
	}
}

void domSetCurrentTagState(tagTipo ptag,xmlNodePtr pnode)
{
	mcurrTagState[(int)ptag]=pnode;
}

//Inserisce il nodo pRoot nello stato tRoot, inoltre setta la variabile globale 
//domTextBuffer a ptext
void domInit(tagTipo tRoot,xmlNodePtr	pRoot,  char *ptext)
{
	mRoot=pRoot;
	domTextBuffer=ptext;
	domTextBufferIndex=0;
	int n;

	//Azzeramento degli stati dei Tags
	for (n=0;n<TAGTIPODIM;n++)
	{
		mcurrTagState[n]=NULL;
	}
	
	//Inserisce il nodo nello stato
	domSetCurrentTagState(tRoot,pRoot);
}

xmlNodePtr domGetLastNodeParent(tagTipo ptag)
{
	
	xmlNodePtr tmp=NULL;
	int n;
	//inizia a controllare dal tag con enumerazione minore
	for(n=((int)ptag)-1;(n>=0) && (mcurrTagState[n]==NULL); n--);
	if (n>=0)tmp=mcurrTagState[n];
	return tmp;
}

//Rimuove (dal Buffer "Stato") tutti i tag con enumerazione maggiore a partire da quello specificato
void domTagCloseFrom(tagTipo t)
{
	domClearStateFrom(t);
}

//Rimuve il tag specificato dal Buffer "Stato"
void domTagClose(tagTipo t)
{
	if (mcurrTagState[(int)t]!=NULL)
	{
		//effettuare la chiusura
		mcurrTagState[(int)t]=NULL;
	}
}

char * domExtractStringBeforeIndex(int pindex)
{
	char *tmpbufferPRE=NULL;
	char *tmpbufferAFTER=NULL;
	
	int	tempbufferlen = pindex-domTextBufferIndex;
	if (tempbufferlen>0){
		//crea il buffer e lo riempie
		tmpbufferPRE=utilGetPartialText(domTextBuffer,domTextBufferIndex,tempbufferlen);

		/*puts("----------------------BUFFERPRE---------------------------------------\n");
		puts(tmpbufferPRE);*/
		int sbufpre ;
		sbufpre = strlen(tmpbufferPRE);;		

		//Conversione del testo strappato nel formato UTF-8
		tmpbufferAFTER=utilConvertiText(tmpbufferPRE);
		int sbufaft ;
		sbufaft = strlen(tmpbufferAFTER);

		/*int sbufpre = strlen(tmpbufferPRE);		
		tmpbufferAFTER =strdup(tmpbufferPRE);		
		int sbufaft = strlen(tmpbufferAFTER);*/

		/*puts("----------------------BUFFERAFTER---------------------------------------\n");
		puts(tmpbufferAFTER);
		puts("------------------FINE CONV---------------------------------\n");*/
		
		//if (tmpbufferPRE!=NULL)free(tmpbufferPRE);
		//aggiorna domTextBufferIndex per sapere fino a dove è stato estratto l'ultimo testo
		domTextBufferIndex=pindex;
	}
	return tmpbufferAFTER;
}

xmlNodePtr domGetLastChild(void)
{
	int n;
	xmlNodePtr tmp=NULL;
	//ricerca il TAG (nello stato) con enumerazione + alta //Che significa nello stato???
	for (n=TAGTIPODIM-1; (n>=0)&&(mcurrTagState[n]==NULL) ;n--)	;
	
	if (n>=0)tmp=mcurrTagState[n];

	return tmp;
}


// Se vi è del testo non assegnato, viene aggiunto all'ultimo TAG (quello con enumerazione + alta)
void domClose(void)
{
	//xmlNodePtr mtext;
	
	int slen=0;
	char *strbuff;

	xmlNodePtr lastChild=domGetLastChild();
 	//Se vi è testo non assegnato a nessun TAG
	int strbuflen=strlen(domTextBuffer);

	strbuff=domExtractStringBeforeIndex(strbuflen);
	if (strbuff)
	{
		slen = strlen(strbuff);
		//mtext=xmlNewText(BAD_CAST strbuff);
		//xmlAddChild(lastChild,mtext);
		
		//Attacca una lista testo/entità piuttosto che un nodo di testo:
		//xmlAddChild(lastChild, xmlStringGetNodeList(NULL, BAD_CAST strbuff));
		addSibling(lastChild, xmlStringGetNodeList(NULL, BAD_CAST strbuff));
	}
}

//Apertura e chiusura Tag
//Da chi viene chiamata?
void domTagAdd3(tagTipo ptag,int pindex,int plen,int pnumConv,int platConv)
{
	xmlNodePtr tmp= domTagOpen( ptag, pindex, plen);
	if (tmp) domSetIDtoNode(tmp,ptag,pnumConv,platConv,NULL);
}

//tutto il testo fino a "pIndex" viene Aggiunto "all'Ultimo" nodo nello Stato
void domAppendTextToLastNode(int pIndex)
{
	xmlNodePtr lastChild=domGetLastChild();
	
 	//Se vi è testo non assegnato a nessun TAG
	char *strbuff=domExtractStringBeforeIndex(pIndex);
	if (strbuff)
	{
		//xmlNodePtr mtext=xmlNewText(BAD_CAST strbuff);
		//xmlAddChild(lastChild,mtext);
		
		//Attacca una lista testo/entità piuttosto che un nodo di testo:
		//xmlAddChild(lastChild, xmlStringGetNodeList(NULL, BAD_CAST strbuff));
		addSibling(lastChild, xmlStringGetNodeList(NULL, BAD_CAST strbuff));  //<--attacca la lista "a mano"...
		
		free(strbuff);
	}
}

//Apertura di un nodo PTAG
//Tutto il testo non assegnato fino a PINDEX viene aggiunto "all'Ultimo" nodo
xmlNodePtr domTagOpen(tagTipo ptag,int pindex,int plen)
{
	//xmlNodePtr mtext;
	xmlNodePtr currnode;
	xmlNodePtr mparent=domGetLastNodeParent(ptag);
	char *strbuff;
	int n;

	xmlNodePtr lastChild=domGetLastChild();
	
 	//Se vi è testo non assegnato a nessun TAG
	strbuff=domExtractStringBeforeIndex(pindex);
	if (strbuff)
	{
		//mtext=xmlNewText(BAD_CAST strbuff);
		//xmlAddChild(lastChild,mtext);
		
		//xmlNodePtr nlist = xmlStringGetNodeList(NULL, BAD_CAST strbuff);
		//Attacca una lista testo/entità piuttosto che un nodo di testo:
		//xmlAddChild(lastChild, nlist);
		//xmlNewChild(lastChild,NULL, nlist, BAD_CAST strbuff);
		addSibling(lastChild, xmlStringGetNodeList(NULL, BAD_CAST strbuff));
	}

	//elimina dallo STATOBUFFER tutti i nodi con enumerazione maggiore
	for(n=(int)ptag;n<TAGTIPODIM;n++) {
		domTagClose(n);
	}
	
	//creazione del nodo di tipo PTAG
	currnode = xmlNewNode(NULL, BAD_CAST tagTipoToNome(ptag));

	if(ptag == virgolette)
		addSibling(mparent,currnode);
	else
		xmlAddChild(mparent, currnode);  //<-- problema: se c'è una lista nodo/entità
										//le virgolette non sono messe alla fine della lista
										//ma subito dopo il primo nodo di testo
	
	
	//xmlNewChild(mparent, NULL, BAD_CAST tagTipoToNome(ptag), NULL);
	//currnode = xmlGetLastChild(mparent);
	
	domTextBufferIndex=pindex;

	if(plen>0) {
		strbuff=utilGetPartialText(domTextBuffer,pindex,plen);
		
		char *t=utilConvertiText(strbuff);
		//mtext=xmlNewText(BAD_CAST t);
		//xmlAddChild(currnode,mtext);
		
		//Attacca una lista testo/entità piuttosto che un nodo di testo:
		xmlAddChild(currnode, xmlStringGetNodeList(NULL, BAD_CAST t));

		domTextBufferIndex=pindex + plen;
		
	} else {		
		domSetCurrentTagState(ptag,currnode);
	}
	free(strbuff);
	return currnode;
}

//Crea un nodo di tipo TNODE e lo aggiunge a TPARENT
//Tutto il testo non assegnato fino a PINDEX viene aggiunto "all'Ultimo" nodo
void domTagInsertEmpty(tagTipo tparent,tagTipo tcur,int pindex)
{
	xmlNodePtr p=mcurrTagState[(int)tparent];
	if (p!=NULL)
	{
		domAppendTextToLastNode(pindex);

		xmlNodePtr ncur = xmlNewNode(NULL, BAD_CAST tagTipoToNome(tcur));

		//xmlAddChild(p,ncur);
		addSibling(p,ncur);
	}
}

char * domNum2String(int pnum,int plat) {
	return (char *) utilConcatena(2, utilItoa(pnum), arabicToLatin(plat));
}


void domSetIDtoNode(xmlNodePtr pnodo, tagTipo ptag,int pnumConv,int platConv,char *pnomeattr)
{
	if (pnomeattr==NULL)pnomeattr=ATTRIB_ID;

	if (pnodo!=NULL)
	{
		xmlAttrPtr	newattr;
		newattr = xmlNewProp (pnodo, pnomeattr, BAD_CAST utilConcatena(2,tagTipoToNomeID(ptag),domNum2String(pnumConv,platConv)));
	}
}

//Solo ID=PREFISSO+NUMERO
void domSetID(tagTipo ptag,int numConv,int latConv)
{
	xmlNodePtr	nodo;
	if(ptag == puntata) return;  //In caso di elemento 'ep' non deve essere settato l'attributo ID
	nodo=mcurrTagState[(int)ptag];
	
	domSetIDtoNode(nodo,ptag,numConv,latConv,NULL);
}

//Solo ID=NUMERO
void domSetID2(tagTipo ptag,char *pnomeattr,int pnumConv,int platConv)
{
	xmlNodePtr	nodo;
	nodo=mcurrTagState[(int)ptag];
	
	if (nodo!=NULL)
		{
			xmlAttrPtr	newattr;
			newattr = xmlNewProp (nodo, pnomeattr, BAD_CAST domNum2String(pnumConv,platConv));
		}
}

void domAttributeIDUpdate(xmlNodePtr node,char * pParentID, char * pAnnessoParentID)
{
	xmlNodePtr cur;
	cur = node->xmlChildrenNode;
	
	if (cur==NULL)return;
	char *idval=NULL, *newid=NULL; //, *numval=NULL;
	
	while (cur != NULL) {
		idval=xmlGetProp(cur,ATTRIB_ID);

		/*
		if (!xmlStrcmp(cur->name, tagTipoToNome(ndr)))
			numval=xmlGetProp(cur, BAD_CAST "num"); //'NDR' usa 'num', non 'id'
			
		//cur è un 'NDR', se siamo in un annesso il valore di 'num' deve essere aggiornato
		if(numval && pAnnessoParentID)
			xmlSetProp(cur, BAD_CAST "num", 
				BAD_CAST (char *)utilConcatena(3,pAnnessoParentID,ATTRIB_ID_SEP,numval));
		*/
	
		if (idval) { // Se il nodo ha impostato l'attributo ID
			//Se si tratta di un ANNESSO
			if (!xmlStrcmp(cur->name, tagTipoToNome(annesso)))	{
				pAnnessoParentID=idval;//strdup(idval);???
			}
			//Se si tratta di un ARTICOLO
			if (!xmlStrcmp(cur->name, tagTipoToNome(articolo)))	{
				pParentID=pAnnessoParentID;
			}

			if (pParentID)
				newid=(char *)utilConcatena(3,pParentID,ATTRIB_ID_SEP,idval);
			else
				newid=idval;

			xmlSetProp(cur,ATTRIB_ID,newid);
			domAttributeIDUpdate(cur,newid,pAnnessoParentID);
		} else {
			domAttributeIDUpdate(cur,pParentID,pAnnessoParentID);
		}
		cur = cur->next; //next fratello
	}
}

//Risale fino ad un nodo che ha ->prev=NULL e ->parent=NULL
//(non è detto che sia il root del documento...)
xmlNodePtr domGetFirstNode(xmlNodePtr node) {
	xmlNodePtr prev=NULL, tmp=NULL, root=NULL;
	
	while(node!=NULL) {
		root=node;
		tmp=node;
		prev=node->prev;
		while(prev!=NULL) {
			tmp=prev;
			prev=tmp->prev;
		}
		node=tmp->parent;	
	}
	return root;
}

