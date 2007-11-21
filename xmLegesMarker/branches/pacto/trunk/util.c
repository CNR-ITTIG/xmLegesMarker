/******************************************************************************
* Project:	xmLeges
* Module:	Marker
* File:		util.c
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Mirco Taddei (m.taddei@ittig.cnr.it)
* 			Lorenzo Bacci (bacci@ittig.cnr.it)
******************************************************************************/
#include "util.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>


//char *numeriOrdinali[] = { "" /* 0 */,
//	"prim", "second", "terz", "quart", "quint",
//	"sest", "settim", "ottav", "non", "decim",
//        "undicesim", "dodicesim", "tredicesim", "quattordicesim", "quindicesim",
//	"sedicesim", "diciassettesim", "diciottesim", "diciannovesim", "ventesim"
//	};

//-----------------------FUNZIONI USATE PER IL CALCOLO DELLE PERCENTUALI------------------

static float mPercSingoloElemento=100;
static float mCurrPerc=0;	//Percentuale di esecuzione Corrente

// -------------------------------FLAG----------------------------------------------------

// Flag che determina l'opportunità di effettuare la conversione in iso 
static int	 globSetAnnessi = 1;

// Se è a 1 indica che il file di origine è in formato UTF-8
static int	 flagUTF8 = 0;


//Imposta la percentuale corrente
void utilPercCurrSet(int pPerc)
{
	mCurrPerc=pPerc;
	loggerPerc(utilItoa((int)mCurrPerc));
}

void utilPercCalc(int pPerc)
{
	mCurrPerc+= (mPercSingoloElemento/100.0f)*pPerc;
	//if (mCurrPerc>100.0f )mCurrPerc=100.0f;
	loggerPerc(utilItoa((int)mCurrPerc));
}

void utilPercNumBlockSet(int pNumAnnessi)
{
	mPercSingoloElemento= 100.0f /(pNumAnnessi + 1);
}

//----------------------------------------------------------------------------------------


//Sgancia il nodo e lo elimina
void utilNodeDelete(xmlNodePtr pNodo)
{
	if (pNodo!=NULL){
		xmlUnlinkNode(pNodo);
		xmlFreeNode(pNodo);
	}
}

//Crea un buffer di dimensioni indicate e lo azzera
//NEW
char *utilCreaBuffer(int size){
	char *tmpbuff=(char *)malloc( sizeof(char)*(size + 1) );
	memset( tmpbuff, 0, sizeof(char)*(size + 1) );
	return tmpbuff;
}

//-----------------	CONTEGGIO NODI (-n) -------------------

static int _utilNodeCountVect[TAGTIPODIM];

void utilNodeCount(xmlNodePtr parentNode){
	int n;
	//Init
	for (n=0;n<TAGTIPODIM;n++)_utilNodeCountVect[n]=0;

	_utilNodeCount(parentNode);
	for (n=0;n<TAGTIPODIM;n++){
		loggerInfo(utilConcatena(4,"NODECOUNT ",tagTipoToNome(n),"=",utilItoa(_utilNodeCountVect[n])));
	}
}

//RICORSIVA
void _utilNodeCount(xmlNodePtr node)
{
	if(node==NULL)return;
	int k=(int)	utilString2TagTipo((char *)node->name);	//Conteggio del nodo
	_utilNodeCountVect[k]++;
	xmlNodePtr cur=node->children;

	while(cur!=NULL)
	{
		if (!xmlNodeIsText(cur))_utilNodeCount(cur);
		cur=cur->next;
	}
}

//---------------------------------------------------------


tagTipo utilString2TagTipo(char  *p)
{
	int n;
	for (n=0;n<TAGTIPODIM;n++){
		if (xmlStrEqual(p,BAD_CAST tagTipoToNome(n))){
			return n;
		}
	}
	//printf("%s\n",p);
	return sconosciuto;
}



//--------------------- ICONV ---------------------
static iconv_t  ConvDescr=NULL;
static iconv_t  ConvDescr2=NULL;

char * utilConvertiText(char *txtin)
{
	char *inizio;

	if ( GetFlagUTF8() )
		inizio = txtin;

	else //converti solo se il testo originario non è UTF-8
	{
	
		size_t lin=strlen(txtin),lout=lin*2,v;
		
		char *bufout=( char *)malloc(lout);
		memset( bufout, 0, lout );
		inizio=bufout;
		const char * por=(const char *)txtin;

		if (!ConvDescr)	
			//if (strcmpi ((const char *)configEncoding(),"UTF-8") !=0 )
			ConvDescr=iconv_open("UTF-8",(char *)configEncoding());
			//ConvDescr=iconv_open("UTF-8","iso-8859-15");
		

		if (ConvDescr==(iconv_t)-1){
			loggerError("iconv Errore iconv_open()");
			exit(0);
		}
		
		v=iconv(ConvDescr,&por,&lin,&bufout,&lout);

		if (v== (size_t) -1){
			if (errno==EILSEQ)loggerError("iconv Errore EILSEQ.");
			if (errno==E2BIG)loggerError("iconv Buffer insufficiente.");
			exit(0);
		}
	}

	//iclose = iconv_close(ConvDescr);
	//if (iclose != 0)
	//{
	//	loggerError("iconv_close() error");
	//	exit(0);
	//}
	
	return inizio;
}


//------------ PER CONVERTIRE DA UTF-8 NEL FORMATO ORIGINARIO, CHE PER DEFAULT E' ISO-8859-15 ************

char * utilConvTextToIso(char *txtin)
{
	char *inizio;

	if ( GetFlagUTF8() )
		inizio = txtin;

	else //converti solo se il testo originario non è UTF-8
	{
		size_t lin=strlen(txtin),lout=lin*2,v;
		
		char *bufout=( char *)malloc(lout);
		memset( bufout, 0, lout );
		inizio=bufout;
		const char * por=(const char *)txtin;

		//UTF8Toisolat1(bufout,lout,txtin,lin);
		
		if (!ConvDescr2)	
			ConvDescr2=iconv_open((char *)configEncoding(),"UTF-8");
			//ConvDescr2=iconv_open("iso-8859-15","UTF-8");


		if (ConvDescr2==(iconv_t)-1){
			loggerError("iconv Errore iconv_open()");
			exit(0);
		}

		v=iconv(ConvDescr2,&por,&lin,&bufout,&lout);

		if (v== (size_t) -1){
			if (errno==EILSEQ)loggerError("iconv Errore EILSEQ.");
			if (errno==E2BIG)loggerError("iconv Buffer insufficiente.");
			exit(0);
		}
	}

	return inizio;
}


//-----------------------------------------------

//----- Setta il valore di globSetAnnessi
void SetFlagAnnessi(int flag)
{
	globSetAnnessi = flag;
}

//----- Recupera il valore di globSetAnnessi
int GetFlagAnnessi(void)
{
	return globSetAnnessi;
}

//---------------------------------------------------

//------Setta il valore di flagUTF8
void SetFlagUTF8(int flagutf)
{
	flagUTF8 = flagutf;
}

//----- Recupera il valore di flagUTF8
int GetFlagUTF8(void)
{
	return flagUTF8;
}

//----------------------------------------------------------
//Se bufdest è NULL si limita a contare i nodi di tipo pnomeTag
//Aggiunto parametro "size" (dimensione del buffer)
int GetAllNodebyTagTipo(xmlNodePtr *bufdest, int size, xmlNodePtr pNodoParent , xmlChar *pnomeTag) {
	int count=0;

	xmlNodePtr cur;
	cur = pNodoParent->xmlChildrenNode;	//FirstChild

	while (cur != NULL) {
		if (!xmlStrcmp(cur->name, pnomeTag)){
			if(bufdest!=NULL) {
				if(count>size-1) {
					printf("\n>>WARNING<< - GetAllNodebyTagTipo() - buffer is full(%d) !!",size);
					return count;
				}
				bufdest[count]=cur;
			}
			count++;
		}
		cur = cur->next; //next fratello
	}
	return count;
}

//Verifica se pNodo è del tipo ptipo
int IsNode(xmlNodePtr pNodo,tagTipo ptipo)
{
	if (pNodo!=NULL)
		if (!xmlStrcmp(pNodo->name, tagTipoToNome(ptipo)))
			return 1;
	return 0;
}

/* <sostituzione> 16-02-06 
 * Sostituita a causa di DDL3621.htm (art.4 comma 3 assente)
 * in quel caso provoca un non corretto spostamento e unlink
 * dei figli che causa un seg.fault nella successiva addChild
 * in virgolette.c.
 */
/*
void MoveAllChildren(xmlNodePtr pFrom,xmlNodePtr pTo)
{
	
	xmlNodePtr cur= pFrom->children;	//FirstChild
	xmlNodePtr tmp;
	while (cur != NULL) {
	
		tmp=cur;
		//NON SPOSTARE: altrimenti si perde l'informazione sul fratello
		cur = cur->next; //NextChild
		
		xmlUnlinkNode(tmp);
		xmlAddChild(pTo,tmp);
	}
}
*/
void MoveAllChildren(xmlNodePtr pFrom,xmlNodePtr pTo)
{
	xmlNodePtr cur = pFrom->children;	//FirstChild
	xmlNodePtr tmp = NULL;
	if(cur == NULL) return;
	
	//Utilizzo una NodeList (addChild nodo per nodo aggiorna sempre il 
	//puntatore al parent e questo non è corretto in caso di liste nodo-testo/nodo-entità)
	xmlNodePtr nlist = xmlCopyNodeList(cur);
	//Unlink e FreeNode a partire dall'ultimo nodo della lista:
	while (cur != NULL) {
		tmp = cur;
		cur = cur->next;
	}
	while (tmp != NULL) {
		cur = tmp;
		tmp = tmp->prev;
		xmlUnlinkNode(cur);
		xmlFreeNode(cur);
	}
	xmlAddChild(pTo, nlist);  //xmlAddChildList() ??
}
/* </sostituzione> */

//NON RICORSIVA
//Restituisce il primo nodo testo figlio di pNodoParent
xmlNodePtr GetFirstTextNode(xmlNodePtr pNodoParent)
{
	if (pNodoParent!=NULL){
		xmlNodePtr cur= pNodoParent->xmlChildrenNode;	//FirstChild

		while (cur != NULL) {
			if (xmlNodeIsText(cur)){
				return cur;
			}
			cur = cur->next; //NextChild
		}
	}
	return NULL;
}

//RICORSIVA
//Converte tutti i nodi TAGERRORE in PI
//Se il nodo è TAGERRORE e non è un PI, lo sostituisce con una nuova PI
void utilErrore2ProcessingInstruction(xmlNodePtr pNodoParent )
{
	char * xmlCont=NULL; //è come xmlchar *
	xmlNodePtr cur,txtnode,pinode;
	cur = pNodoParent->xmlChildrenNode;	//FirstChild
	//printf("%s\n",pNodoParent->name);
	while (cur != NULL) {
		if ((IsNode(cur,tagerrore))&&(cur->type!=XML_PI_NODE)){
			txtnode=GetFirstTextNode(cur);
	
			if (txtnode){
				//Trattare il testo come una lista testo/entità:
				//xmlCont= xmlNodeGetContent(txtnode);				
				xmlCont= xmlNodeListGetString(NULL, txtnode, 0); 
				//Con la precedente riga si perdono le entità per avere un unico nodo di testo
				//(può dare problemi in fase di visualizzazione...)
								
				/*puts("---------------------------------------------------------------INIZIO PI NODE-----------------------------------------------\n");
				puts(xmlCont);
				puts("---------------------------------------------------------------FINE PI NODE-----------------------------------------------\n");			*/
				pinode=xmlNewPI(BAD_CAST tagTipoToNome(tagerrore),xmlCont);
			}else
			{
				//ERRORE VUOTO
				pinode=xmlNewPI(BAD_CAST tagTipoToNome(tagerrore),BAD_CAST "");
			}
			//sostituisco la PI al nodo corrente e poi libero il nodo vecchio
			xmlFreeNode(xmlReplaceNode(cur,pinode));
			cur=pinode;//riaggiorno il puntatore corrente alla nuova PI			}
		}
		utilErrore2ProcessingInstruction(cur);
		cur = cur->next; //NextChild
	}
}

//Di un nodo "pNodoParent" restituisce il primo nodo figlio che possiede il nome "pnomeTag"
//NON RICORSIVA
xmlNodePtr GetFirstNodebyTagTipo(xmlNodePtr pNodoParent , xmlChar *pnomeTag) {
	
	xmlNodePtr cur;
	cur = pNodoParent->xmlChildrenNode;	//FirstChild

	while (cur != NULL) {
		if (!xmlStrcmp(cur->name, pnomeTag)) {
			return cur;
		}
		cur = cur->next; //NextChild
	}
	return NULL;
}

//Controlla *tutto* il sotto-albero (ricerca in larghezza, usa buffer FIFO)
//Possibile implementazione migliore: usare una lista e aggiornare solo il puntatore
//all'elemento di testa invece di riorganizzare tutto il buffer (vedi adjustBuffer())
//Se savebuf!=NULL non termina il processo quando viene ritrovato un nodo di tipo nomeTag
//e inserisce il puntatore a quel nodo in savebuf (savesize è la dimensione di savebuf).
xmlNodePtr totalGetFirstNodebyTagTipo(xmlNodePtr root, xmlChar *nomeTag, xmlNodePtr *savebuf, int savesize) {
	int size = 0; //attuale numero di elementi nel buffer
	const int max = 8192; //Max elementi nel buffer durante la ricerca? 
	//(vedi dlgs n.259 01-07-2003 (oltre 1900!))
	//D.Lgs. 30 aprile 1992, n. 285 <--- 3761 !!!!!!
	int max_filling = 0; //Massimo valore di riempimento raggiunto (debug)
	xmlNodePtr buffer[max]; 
	xmlNodePtr current, child;
	int count = 0;
	
	addNodeInBuffer(buffer, root, &size);
	while(size > 0) {
		if(size>(max-3)) { // (può sbagliare, ma non crashare!)
			printf(">>>WARNING<<< -- totalGetFirstNodebyTagTipo() -- size is %d !! Stopping...\n", size);
			return NULL;
		}
		if(size>max_filling)
			max_filling = size;

		current = buffer[0];
		adjustBuffer(buffer, &size);
		//printf("\n - %s - %d [%d]", (char *)current->name, size, max_filling);
		if (!xmlStrcmp(current->name, nomeTag)) {
			if(savebuf!=NULL) {
				if(count>savesize-1) {
					printf("\n>>WARNING<< - totalGetFirstNodebyTagTipo() - savebuf is full! (%d)!!",savesize);
					//printf("\n - %s - %d [%d]", (char *)current->name, size, max_filling);
					return *savebuf;
				} 
				savebuf[count]=current;
				count++;
			} else {
				//printf("\n - %s - %d [%d]", (char *)current->name, size, max_filling);
				return current; //Trovato! Esci...
			}
		} else {
			child = current->children;
			while(child!=NULL) { //Aggiungi tutti figli del nodo nel buffer
				addNodeInBuffer(buffer, child, &size);
				child = child->next;
			}
		}
	}
	//printf("\n - %d [%d]", size, max_filling);
	return NULL; //Tag non trovato
}

//Aggiungi il nodo nella prima posizione libera del buffer
void addNodeInBuffer(xmlNodePtr *buffer, xmlNodePtr node, int *size) {
	buffer[*size] = node;
	(*size)++;
}

//Shifta tutti gli elementi del buffer di una posizione - Se troppo pesante implementare
//una ricerca con lista: dovrebbe bastare aggiornare il puntatore al nodo di testa.
void adjustBuffer(xmlNodePtr *buffer, int *size) {
	int i = 0;
	(*size)--;
	/*
	while(i<*size)
		buffer[i]=buffer[++i]; //warning: operation on `i' may be undefined
	*/
	for(i=0;i<*size;i++)
		buffer[i]=buffer[i+1];
}

void InsertXmlFile(char *memdoc,xmlNodePtr pParentNode) {
	xmlDocPtr doc;
	xmlNodePtr cur;
	
	doc = xmlParseMemory(memdoc,strlen(memdoc));
//	doc = xmlParseFile(docname);
	if (doc == NULL ) {
		return;
	}
	cur = xmlDocGetRootElement(doc);
	if (cur == NULL) {
		xmlFreeDoc(doc);
		return;
	}

	//Accoda tutti i figli della radice a pParentNode
	xmlNodePtr ntmp;
	ntmp = cur->xmlChildrenNode;	//FirstChild

	while (ntmp != NULL) {
			xmlAddChild(pParentNode, ntmp);
			ntmp = ntmp->next; //next fratello
	}
	return;
}

//Restituisce (creando un nuovo buffer con malloc) una porzione di testo di srctesto
//NEW
char	*utilGetPartialText(char *ptesto,int pindexstart, int plen)
{
	char *buff=utilCreaBuffer(plen);
	char *ptemp=ptesto + pindexstart; //Avanzamento del buffer di testo
	strncpy(buff,ptemp,plen);
	return buff;
}


/******************************************************************************/
/************************************************************ CERCA SPAZIO ****/
/******************************************************************************/
char * cercaSpazio(char *s) {
	for (; *s; s++)
		if (isspace(*s))
			return s;
	return NULL;
}


/******************************************************************************/
/************************************************************ CONTA CHAR ******/
/******************************************************************************/
int contaChar(char *s, char c) {
	int n=0;
	for (; *s; s++)
		if (*s == c) n++;
	return n;
}


/******************************************************************************/
/********************************************************* LATIN TO ARABIC ****/
/******************************************************************************/
int latinToArabic(char *s) {
	int ret = 0;
	if (strstr(s,"un")) ret = 1 ;
	else if (strstr(s, "bis")||strstr(s,"duo")) ret = 2;
	else if (strstr(s, "quater")) 	ret = 4;
	else if (strstr(s, "ter")) 		ret = 3;
	else if (strstr(s, "quinquies"))	ret = 5;
	else if (strstr(s, "sexies")) 	ret = 6;
	else if (strstr(s, "septies")) 	ret = 7;
	else if (strstr(s, "octies")) 	ret = 8;
	else if (strstr(s, "novies")) 	ret = 9;
	
	if (strstr(s,"decies")) ret=ret+10;
	else if (strstr(s,"venies")) ret=ret+20;

	return ret;
}


/******************************************************************************/
/********************************************************* ARABIC TO LATIN ****/
/******************************************************************************/
char * arabicToLatin(int n) {
	char * r;
	switch (n % 10) {
		case 1: r = "un";	break;
		case 2: r = "bis";	break;
		case 3: r = "ter";	break;
		case 4: r = "quater";	break;
		case 5: r = "quinquies";break;
		case 6: r = "sexies";	break;
		case 7: r = "septies";	break;
		case 8: r = "octies";	break;
		case 9: r = "novies";	break;
		default:r = "";	break;
	}
	return r;
}

/******************************************************************************/
/************************************************************ CONV LETTERA ****/
/******************************************************************************/
/*
int convLetteraToInt(char *s) {
	int r;
	if (isalpha(*s))  //// Con 3 cifre!
		r = tolower(*s) - 'a' + 1;
	if (isalpha(*(s+1)))
		r += 26;
	return r;
}
*/
//Sostituita (lunghezza > 2, deve gestire anche il caso "z aa ab ac...")
//(non gestisce "aac aab...")
//Converti a partire da una stringa (Null Terminated array di char).
int convNTLetteraToInt(char *s) {
	int num = -1;
	utilStringToLower(s);
	//Non scambiare "Art." per l'inizio di una lettera...
	//(serve ad evitare che venga messo il messaggio di warning)
	if(strlen(s) == 3 && strncmp(s, "art", 3) == 0)	return -1;

	if(strlen(s) == 1 || s[0]==s[1])
		num = 26*(strlen(s)-1) + (s[0] - 'a' + 1);
	else
		num = 26 + (s[1] - 'a' + 1);

	return num;
}

//Tira fuori la stringa che rappresenta la lettera
//(lettera_ptr è un puntatore al primo carattere...)
char *getLettera(char *lettera_ptr) {
	char *current_lettera = NULL;
	int len=0, i=0;
	while(isalpha(lettera_ptr[len])) len++;
	current_lettera = (char *)malloc((len+1)*sizeof(char));
	for(i=0;i<len;i++)
		current_lettera[i] = lettera_ptr[i];
	current_lettera[len] = '\0';
	return current_lettera;
}

/******************************************************************* 
Nome:			sostStr
Scopo:		SOSTITUISCE IN <STRINGA> OGNI OCCORRENZA DELLA STRINGA <OLD>
				CON LA STRINGA <NEW>                                              
Parametri:	STRINGA = STRINGA SULLA QUALE AVVIENE IL CAMBIAMENTO             
				OLD     = SOTTOSTRINGA DA SOSTITUIRE                             
				NEW     = SOTTOSTRINGA SOSTITUITA A <OLD>          P.L.S.   
 ********************************************************************/ 
void sostStr(char *stringa, char *old, char *new) {

	int   lold, lnew, ldif, k1, i;
	char  *pun;
	
	lold=strlen(old);

	if (strlen(stringa)>0 && lold>0) {
	
		lnew=strlen(new);
		ldif=lnew-lold;
	
		pun=strstr(stringa,old);   /* cerco old in stringa */
		while (pun != NULL)        /* finche' trovo old */
			{
			if (ldif>0)             /* si inseriscono car. */
				{
				k1=pun-stringa;      /* ind. ultimo car. da spostare */
	
				for(i=strlen(stringa)+ldif; i>k1; i--)    /* creo */ 
					stringa[i]=stringa[i-ldif];            /* posto */
				}
	
			else if (ldif<0)        /* accorcio */ 
				strcpy(pun+lold+ldif,pun+lold);
	
			strncpy(pun,new,lnew);     /* sostituisco */
	
			pun=strstr(pun+lnew,old);  /* cerco prossimo old */
	
		} /* fine while */
	
	} /* fine if */

} /* fine sostStr */

/*
//Converti il numero nella lettera corrispondente
//(aggiunto poichè id delle lettere per convenzione non sono più del tipo
//art1-com1-let1 ma art1-com1-leta).
//Sequenza: a b c ... z aa bb cc ... zz aaa bbb ...)
char *convIntToLettera(int num) {
	if(num<1) {
		printf("\nWARNING -convIntToLettera- num value is not valid!\n");
		return NULL;
	}
	
	int div = num/27 + 1; //è la lunghezza della stringa da ritornare
	int resto = num%26;
	if(resto==0) resto=26; //num non parte da 0 !
	int intlet = 'a' + resto - 1;
	char *ret= (char *)malloc((div+1)*sizeof(char));
	int i;
	
	for(i=0;i<div;i++)
		ret[i]=(char)intlet;
	ret[div]='\0';
	//printf("\n ID LETTERA - ret:%s div:%d resto:%d intlet:%d\n",ret,div,resto,intlet);
	return ret;
}
*/
