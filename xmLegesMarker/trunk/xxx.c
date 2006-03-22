#include "xxx.h"

char		*xxxTextBuffer;
int			xxxTextBufferIndex;
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

xmlNodePtr	xxxGetLastTag(tagTipo ptag)
{
	return mcurrTagState[(int)ptag];
}

void xxxClearStateFrom(tagTipo ptag)
{
	int n;
	for (n=(int)ptag;n<TAGTIPODIM;n++)
	{
		mcurrTagState[n]=NULL;
	}
}

void xxxSetCurrentTagState(tagTipo ptag,xmlNodePtr pnode)
{
	mcurrTagState[(int)ptag]=pnode;
}

//Inserisce il nodo pRoot nello stato tRoot, inoltre setta la variabile globale 
//xxxTextBuffer a ptext
void xxxInit(tagTipo tRoot,xmlNodePtr	pRoot,  char *ptext)
{
	mRoot=pRoot;
	xxxTextBuffer=ptext;
	xxxTextBufferIndex=0;
	int n;

	//Azzeramento degli stati dei Tags
	for (n=0;n<TAGTIPODIM;n++)
	{
		mcurrTagState[n]=NULL;
	}
	
	//Inserisce il nodo nello stato
	xxxSetCurrentTagState(tRoot,pRoot);
}

xmlNodePtr xxxGetLastNodeParent(tagTipo ptag)
{
	
	xmlNodePtr tmp=NULL;
	int n;
	//inizia a controllare dal tag con enumerazione minore
	for(n=((int)ptag)-1;(n>=0) && (mcurrTagState[n]==NULL); n--);
	if (n>=0)tmp=mcurrTagState[n];
	return tmp;
}

//Rimuove (dal Buffer "Stato") tutti i tag con enumerazione maggiore a partire da quello specificato
void xxxTagCloseFrom(tagTipo t)
{
	xxxClearStateFrom(t);
}

//Rimuve il tag specificato dal Buffer "Stato"
void xxxTagClose(tagTipo t)
{
	if (mcurrTagState[(int)t]!=NULL)
	{
		//effettuare la chiusura
		mcurrTagState[(int)t]=NULL;
	}
}

char * xxxExtractStringBeforeIndex(int pindex)
{
	char *tmpbufferPRE=NULL;
	char *tmpbufferAFTER=NULL;
	
	int	tempbufferlen = pindex-xxxTextBufferIndex;
	if (tempbufferlen>0){
		//crea il buffer e lo riempie
		tmpbufferPRE=utilGetPartialText(xxxTextBuffer,xxxTextBufferIndex,tempbufferlen);

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
		//aggiorna xxxTextBufferIndex per sapere fino a dove è stato estratto l'ultimo testo
		xxxTextBufferIndex=pindex;
	}
	return tmpbufferAFTER;
}

xmlNodePtr xxxGetLastChild(void)
{
	int n;
	xmlNodePtr tmp=NULL;
	//ricerca il TAG (nello stato) con enumerazione + alta //Che significa nello stato???
	for (n=TAGTIPODIM-1; (n>=0)&&(mcurrTagState[n]==NULL) ;n--)	;
	
	if (n>=0)tmp=mcurrTagState[n];

	return tmp;
}


// Se vi è del testo non assegnato, viene aggiunto all'ultimo TAG (quello con enumerazione + alta)
void xxxClose(void)
{
	//xmlNodePtr mtext;
	
	int slen=0;
	char *strbuff;

	xmlNodePtr lastChild=xxxGetLastChild();
 	//Se vi è testo non assegnato a nessun TAG
	int strbuflen=strlen(xxxTextBuffer);

	strbuff=xxxExtractStringBeforeIndex(strbuflen);
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
void xxxTagAdd3(tagTipo ptag,int pindex,int plen,int pnumConv,int platConv)
{
	xmlNodePtr tmp= xxxTagOpen( ptag, pindex, plen);
	if (tmp) xxxSetIDtoNode(tmp,ptag,pnumConv,platConv,NULL);
}

//tutto il testo fino a "pIndex" viene Aggiunto "all'Ultimo" nodo nello Stato
void xxxAppendTextToLastNode(int pIndex)
{
	xmlNodePtr lastChild=xxxGetLastChild();
	
 	//Se vi è testo non assegnato a nessun TAG
	char *strbuff=xxxExtractStringBeforeIndex(pIndex);
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
xmlNodePtr xxxTagOpen(tagTipo ptag,int pindex,int plen)
{
	//xmlNodePtr mtext;
	xmlNodePtr currnode;
	xmlNodePtr mparent=xxxGetLastNodeParent(ptag);
	char *strbuff;
	int n;

	xmlNodePtr lastChild=xxxGetLastChild();
	
 	//Se vi è testo non assegnato a nessun TAG
	strbuff=xxxExtractStringBeforeIndex(pindex);
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
		xxxTagClose(n);
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
	
	xxxTextBufferIndex=pindex;

	if(plen>0) {
		strbuff=utilGetPartialText(xxxTextBuffer,pindex,plen);
		
		char *t=utilConvertiText(strbuff);
		//mtext=xmlNewText(BAD_CAST t);
		//xmlAddChild(currnode,mtext);
		
		//Attacca una lista testo/entità piuttosto che un nodo di testo:
		xmlAddChild(currnode, xmlStringGetNodeList(NULL, BAD_CAST t));

		xxxTextBufferIndex=pindex + plen;
		
	} else {		
		xxxSetCurrentTagState(ptag,currnode);
	}
	free(strbuff);
	return currnode;
}

//Crea un nodo di tipo TNODE e lo aggiunge a TPARENT
//Tutto il testo non assegnato fino a PINDEX viene aggiunto "all'Ultimo" nodo
void xxxTagInsertEmpty(tagTipo tparent,tagTipo tcur,int pindex)
{
	xmlNodePtr p=mcurrTagState[(int)tparent];
	if (p!=NULL)
	{
		xxxAppendTextToLastNode(pindex);

		xmlNodePtr ncur = xmlNewNode(NULL, BAD_CAST tagTipoToNome(tcur));

		//xmlAddChild(p,ncur);
		addSibling(p,ncur);
	}
}

char * xxxNum2String(int pnum,int plat) {
	return (char *) utilConcatena(2, utilItoa(pnum), arabicToLatin(plat));
}


void xxxSetIDtoNode(xmlNodePtr	pnodo, tagTipo ptag,int pnumConv,int platConv,char *pnomeattr)
{
	if (pnomeattr==NULL)pnomeattr=ATTRIB_ID;

	if (pnodo!=NULL)
	{
		xmlAttrPtr	newattr;
		newattr = xmlNewProp (pnodo, pnomeattr, BAD_CAST utilConcatena(2,tagTipoToNomeID(ptag),xxxNum2String(pnumConv,platConv)));
	}
}

//Solo ID=PREFISSO+NUMERO
void xxxSetID(tagTipo ptag,int numConv,int latConv)
{
	xmlNodePtr	nodo;
	if(ptag == puntata) return;  //In caso di elemento 'ep' non deve essere settato l'attributo ID
	nodo=mcurrTagState[(int)ptag];
	
	xxxSetIDtoNode(nodo,ptag,numConv,latConv,NULL);
}

//Solo ID=NUMERO
void xxxSetID2(tagTipo ptag,char *pnomeattr,int pnumConv,int platConv)
{
	xmlNodePtr	nodo;
	nodo=mcurrTagState[(int)ptag];
	
	if (nodo!=NULL)
		{
			xmlAttrPtr	newattr;
			newattr = xmlNewProp (nodo, pnomeattr, BAD_CAST xxxNum2String(pnumConv,platConv));
		}
}


void xxxAttributeIDUpdate(xmlNodePtr node,char * pParentID, char * pAnnessoParentID)
{
	xmlNodePtr cur;
	cur = node->xmlChildrenNode;
	
	if (cur==NULL)return;
	char *idval;
	char *newid;
	while (cur != NULL) {
		idval=xmlGetProp(cur,ATTRIB_ID);

		if (idval){		// Se il nodo ha impostato l'attributo ID	
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
			xxxAttributeIDUpdate(cur,newid,pAnnessoParentID);
		}else {
			xxxAttributeIDUpdate(cur,pParentID,pAnnessoParentID);
		}
		cur = cur->next; //next fratello
	}
}


