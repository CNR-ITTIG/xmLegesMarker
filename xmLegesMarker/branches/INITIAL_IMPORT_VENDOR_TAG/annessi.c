#include <stdio.h>

#include "annessi.h"
#include "struttura.h"

//extern int flagAnnessi;

const void AnnessiAnalizza( char *testo, xmlNodePtr pParentNode, tagTipo pTipoParentNode){
	
	//extern int flagAnnessi;
	
	int NumeroAnnessi=0;
	xmlNodePtr mNodoAnnessi=NULL, CurrStruttura=NULL;
	
	if (configIsEnabled(mod_annessi))
	{
		loggerInfo("INIZIO Annessi");

		mNodoAnnessi = xmlNewNode(NULL, BAD_CAST tagTipoToNome(annessi));

		//Inserisce il nodo mNodoAnnessi nello stato "annessi", inoltre passa al buffer di testo 
		//l'intero documento
		xxxInit(annessi,mNodoAnnessi,testo);

		//Chiamata alla funzione LEX
		NumeroAnnessi = _annessiLexStart(testo);

		utilPercNumBlockSet(NumeroAnnessi);

		xxxClose();
		loggerInfo("FINE Annessi");
	}
	else
	{
		loggerDebug("Modulo Annessi Disattivato");
	}

	if (NumeroAnnessi)
	{
		SetFlagAnnessi(1);			//Ci sono degli annessi
		CurrStruttura = GerStrutturaPreAnnessi(mNodoAnnessi);
		
		AnnessiCicla(mNodoAnnessi);
		if (CurrStruttura){
			//Aggancia la STRUTTURA al NIR
			xmlAddChild(pParentNode,CurrStruttura);
			//Aggancia gli ANNESSI alla STRUTTURA
			xmlAddChild(CurrStruttura,mNodoAnnessi);
		}else
		{
			//nel caso che non esista una STRUTTURA prima degli ANNESSI
			//Aggancia gli ANNESSI al NIR
			xmlAddChild(pParentNode,mNodoAnnessi);
		}
	}
	else
	{
		SetFlagAnnessi(0);			//Non ci sono annessi
		//Se non sono stati individuati ANNESSI si cerca un eventuale STRUTTURA
		if (*testo)
			CurrStruttura = StrutturaAnalizza(testo, principale);
		//Aggancia STRUTTURA al pPARENTNODE
		if (CurrStruttura)
			xmlAddChild(pParentNode,CurrStruttura);
	}
	

}

int CountAllNodebyTagTipo(xmlNodePtr pNodoParent , xmlChar *pnomeTag){
	int count=0;

	xmlNodePtr cur;
	cur = pNodoParent->xmlChildrenNode;	//FirstChild

	while (cur != NULL) {
		if (!xmlStrcmp(cur->name, pnomeTag))	{
			count++;
		}
		cur = cur->next; //next fratello
	}
	return count;
}




void AnnessiCicla(xmlNodePtr pNodoParent){
	int n,m;
	xmlNodePtr currAnnesso;
	char *tmpbuff;
	xmlNodePtr	ErrorChildren[100]; //XXX da fare DINAMICO in funzione del numero di figli del nodo
	xmlNodePtr	CurrError,CurrErrorText,CurrStruttura;
	xmlNodePtr	vectAnnessi[MAXANNESSI];
	int			tmpNumErrorChild;

	loggerInfo("INIZIO Ciclo Annessi");
	//Vettore di tutti i nodi con TAG ANNESSO del nodo
	int numAnnessi=GetAllNodebyTagTipo(&vectAnnessi[0],pNodoParent,BAD_CAST tagTipoToNome(annesso));
	
	//Ciclo su tutti i nodi "Annessi"
	for (n=0;n<numAnnessi;n++)
	{
		currAnnesso=vectAnnessi[n];
		//Vettore di tutti i nodi con TAG ERRORE del nodo
		tmpNumErrorChild=GetAllNodebyTagTipo(&ErrorChildren[0],currAnnesso,BAD_CAST tagTipoToNome(tagerrore));
		//Ciclo sui nodi Errore '?errore?'
		for (m=0;m<tmpNumErrorChild;m++)
		{
			CurrError=ErrorChildren[m];
			CurrErrorText=CurrError->children;
			CurrStruttura=NULL;
			//Estrazione del Nodo Testo
			if (xmlNodeIsText(CurrErrorText)==1) //Se si tratta di un nodo testo
			{
				if (CurrErrorText->content)
				{
					//Duplicazione del testo contenuto nel nodo
					tmpbuff=(char *)strdup((char *)CurrErrorText->content);
				//puts("---------------------tmpBuffer-------------------------------");
				//puts(tmpbuff);// *****E' UTF-8!!!!!!
				//puts("---------------------------fine tmpBuffer-------------------------------");

					if (tmpbuff)
					{
						if (configGetDTDTipo() != base)
						{
							CurrStruttura=StrutturaAnalizza(tmpbuff, allegato); // *****tmp buff E' UTF-8!!!!!!
							if (CurrStruttura!=NULL)
							{
								//Se � stata individuata una STRUTTURA elimina il nodo ERRORE
								//e aggancia la STRUTTURA all'ANNESSO
								xmlAddChild(currAnnesso,CurrStruttura);
								utilNodeDelete(CurrError);
							}
						}
						else						// base: non ammessi allegati
						{
							xmlNewChild(currAnnesso, NULL, BAD_CAST "rifesterno", NULL);
							xmlAddPrevSibling(CurrErrorText, xmlNewText(BAD_CAST "===== DTD BASE: CORPO ALLEGATO NON AMMESSO =====\n\n")); 
						}
					}
					free(tmpbuff);
				}
			}
		}
	}
	loggerInfo("FINE Ciclo Annessi");
}

xmlNodePtr GerStrutturaPreAnnessi(xmlNodePtr pParentNode)
{
	char *tmpbuff;
	xmlNodePtr	CurrError,CurrErrorText,CurrStruttura;

	loggerInfo("INIZIO PreAnnessi");
	
	//Individua il primo nodo con Tag ERRORE
	CurrError=GetFirstNodebyTagTipo(pParentNode,BAD_CAST tagTipoToNome(tagerrore));
	//Individua il primo nodo Testo
	CurrErrorText=GetFirstTextNode(CurrError);

	CurrStruttura=NULL;

	if (CurrErrorText) 
	{
		if (CurrErrorText->content)
		{
			//Duplicazione del testo contenuto nel nodo
			tmpbuff=(char *)strdup((char *)CurrErrorText->content);
			if (tmpbuff)
			{
				CurrStruttura=StrutturaAnalizza(tmpbuff, principale); //tmpbuff � in UTF-8!!!!!!!!!!!!!1
				if (CurrStruttura!=NULL){
					//Se � stata individuata una struttura elimina il nodo ERRORE
					//e aggancia la STRUTTURA al PARENT
					////xmlAddChild(pParentNode,CurrStruttura);
					utilNodeDelete(CurrError);
				}
				free(tmpbuff);
			}
		}
	}

	loggerInfo("FINE PreAnnessi");
	return CurrStruttura;
}

void EliminaErrori(xmlNodePtr pParentNode, tagTipo pTipoParentNode)
{
	int m;
	xmlNodePtr	ErrorChildren[100]; //XXX da fare DINAMICO in funzione del numero di figli del nodo
	xmlNodePtr	CurrError;
	int			tmpNumErrorChild;

	loggerInfo("INIZIO EliminaErrori");
	
	//Vettore di tutti i nodi con TAG ERRORE del nodo 'pParentNode'
	tmpNumErrorChild=GetAllNodebyTagTipo(&ErrorChildren[0],pParentNode,BAD_CAST tagTipoToNome(tagerrore));
	//Ciclo sui nodi Errore '?errore?'
	for (m=0;m<tmpNumErrorChild;m++)
	{
		CurrError=ErrorChildren[m];

		//Libera i nodi con Tag ERRORE individuati
		utilNodeDelete(CurrError);
	}
	loggerInfo("FINE EliminaErrori");
}

